#include "server.h"
#include "serverobserver.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>


Server::Server(unsigned short port)
  : m_port(port),
    m_io_service( new boost::asio::io_service() ),
    m_acceptor( new boost::asio::ip::tcp::acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port)) ),
    m_threads(),
    m_sockets(),
    m_nIdCounter(0),
    m_mutex()
{
}


Server::~Server()
{
  stopServer();
  m_io_service->stop();
}


int
Server::openConnection(ConnectionId id)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  // add a new socket to the map
  m_sockets[id].reset(new boost::asio::ip::tcp::socket(*m_io_service));

  std::cout << "Server::openConnection() - id: " << id
            << ", open sockets: " << getNOpenSockets()
            << ", running threads: " << m_threads.size()
            << std::endl;

  return id;
}


void
Server::closeConnection(ConnectionId id)
{
  //  std::cout << "Server::close() - id: " << id << std::endl;
  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_sockets.find(id) == m_sockets.end())
  {
    std::cerr << "Server::closeConnection - connection with id " << id << " does not exist." << std::endl;
    return;
  }

  if (m_sockets[id] && m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    std::cout << "Server::closeConnection() - socket.shutdown() id : " << id << ", " << error.message() << std::endl;

    m_sockets[id]->close();

    m_sockets.erase(id);
    m_threads.erase(id);
  }
}


boost::asio::ip::tcp::socket*
Server::getSocket(ConnectionId id) const
{
  std::lock_guard<std::mutex> lock(m_mutex);

  return m_sockets.at(id).get();
}


void
Server::startServerThread()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  int id = m_nIdCounter++;

  m_threads[id] = std::unique_ptr<std::thread>(new std::thread(&Server::startServing, this, id));
  m_threads[id]->detach();
}


void
Server::stopServer()
{
  std::vector<Server::ConnectionId> openIds = getOpenSocketIds();
  for (std::size_t i = 0; i < openIds.size(); ++i)
  {
    closeConnection(openIds[i]);
  }
}


void
Server::startServing(ConnectionId id)
{
  std::cout << "Server::startServing()" << std::endl;

  openConnection(id);

  boost::asio::ip::tcp::socket* socket = getSocket(id);
  m_acceptor->accept(*socket);
  std::cout << "OUTIL_NETServer::startServing() - connection accepted!" << std::endl;

  // IMPORTANT: after connection is accepted, start a new server thread
  startServerThread();

  bool isOk = true;

  while (isOk)
  {
    std::array<char, ClientServerData::defaultBufferSize> bufIncoming;
    boost::system::error_code error;

    // receive a command
    size_t len = socket->read_some(boost::asio::buffer(bufIncoming), error);

    if (error == boost::asio::error::eof)
    {
      std::cout << "Server:startServing() - connection was closed by the peer." << std::endl;
      break;
    }
    else if (error)
    {
      std::cerr << "Server:startServing() - error reading from connection:" << error.message() << std::endl;
      break;
    }

    std::vector<std::string> incomingStringVec = convertCharArrayToStringVector(bufIncoming, len);

    notifyObservers(incomingStringVec, id);
  }

  closeConnection(id);
}


void
Server::write(const std::vector<std::string>& messageStrings, ConnectionId id, std::string separationChar)
{
  std::string message;
  for (std::size_t i = 0; i < messageStrings.size(); ++i)
  {
    message += separationChar;
    message += messageStrings[i];
  }
  write(message, id);
}


void
Server::write(const std::string& message, ConnectionId id)
{
  boost::asio::ip::tcp::socket* socket = getSocket(id);

  assert(socket->is_open());

  boost::system::error_code ignored_error;
  boost::asio::write(*socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);

  if (ignored_error)
  {
    std::cerr << "Server::write() - : " << ignored_error.message() << std::endl;
    throw boost::system::system_error(ignored_error);
  }
}


void
Server::registerObserver(ServerObserver* observer)
{
  if (!observer)
  {
    assert(0);
    return;
  }

  std::lock_guard<std::mutex> lock(m_mutex);
  m_observers.push_back(observer);
}


void
Server::unregisterObserver(ServerObserver* observer)
{
  if (!observer)
  {
    assert(0);
    return;
  }

  std::lock_guard<std::mutex> lock(m_mutex);
  m_observers.erase( std::find(m_observers.begin(), m_observers.end(), observer) );
}


void
Server::notifyObservers(std::vector<std::string> dataStrings, ConnectionId id)
{
  std::vector<ServerObserver*> observers;
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    observers = m_observers;
  }

  std::for_each(observers.begin(), observers.end(),
                [&dataStrings, &id](ServerObserver* observer)
                {
                  observer->notifyReceivedData(dataStrings, id);
                });
}


std::vector<std::string>
Server::convertCharArrayToStringVector(const std::array<char, ClientServerData::defaultBufferSize>& bufIncoming,
                                       std::size_t len,
                                       std::string separationChar) const
{
  std::string bufString = bufIncoming.data();
  bufString.resize(len);
  std::vector<std::string> newState_str;
  boost::split(newState_str, bufString, boost::is_any_of(separationChar));

  return newState_str;
}


unsigned int
Server::getNOpenSockets() const
{
  int nOpen = 0;

  for (auto iter = m_sockets.begin();
       iter != m_sockets.end(); ++iter)
  {
    if (iter->second->is_open())
    {
      nOpen++;
    }
  }

  return nOpen;
}


std::vector<Server::ConnectionId>
Server::getOpenSocketIds() const
{
  std::vector<ConnectionId> socketIds;

  for (auto iter = m_sockets.begin();
       iter != m_sockets.end(); ++iter)
  {
    if (iter->second->is_open())
    {
      socketIds.push_back(iter->first);
    }
  }

  return socketIds;
}


std::vector<Server::ConnectionId>
Server::getOpenThreadIds() const
{
  std::vector<ConnectionId> threadIds;

  for (auto threadMapIterator = m_threads.begin(); threadMapIterator != m_threads.end(); ++threadMapIterator)
  {
    threadIds.push_back(threadMapIterator->first);
  }

  return threadIds;
}


Server::ConnectionStatus
Server::getConnectionStatus(ConnectionId id) const
{
  if (m_sockets.find(id) != m_sockets.end())
  {
    if (m_sockets.at(id)->is_open())
    {
      return connected;
    }
    else
    {
      return listening;
    }
  }

  return unavailable;
}


std::size_t
Server::getNThreads() const
{
  return m_threads.size();
}
