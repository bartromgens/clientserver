#include "server.h"
#include "serverobserver.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


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

  // create io_service and acceptor if they don't exist yet
  if (!m_io_service)
  {
    m_io_service.reset(new boost::asio::io_service());
  }
  if (!m_acceptor)
  {
    m_acceptor = std::unique_ptr<boost::asio::ip::tcp::acceptor>(new boost::asio::ip::tcp::acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port)));
  }

  // add a new socket to the map
  m_sockets[id].reset(new boost::asio::ip::tcp::socket(*m_io_service));

  std::cout << "Server::open() - id: " << id << std::endl;
  std::cout << "Server::open() - open sockets: " << getNOpenSockets() << std::endl;
  std::cout << "Server::open() - running threads: " << m_threads.size() << std::endl;

  return id;
}


void
Server::closeConnection(ConnectionId id)
{
  //  std::cout << "Server::close() - id: " << id << std::endl;
  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);

    std::cout << "Server::close() - socket.shutdown(): " << error.message() << std::endl;

    m_sockets[id]->close();

    m_sockets.erase(id);
    m_threads.erase(id);
  }
}


boost::asio::ip::tcp::socket*
Server::getRawSocket(ConnectionId id) const
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

  try
  {
    boost::asio::ip::tcp::socket* socket = getRawSocket(id);
    m_acceptor->accept(*socket);

    // after connection is accepted, start a new server thread
    startServerThread();

    // wait for incoming data
    for (;;)
    {
      std::array<char, 2048> bufIncoming;
      boost::system::error_code error;

      // receive a command
      size_t len = socket->read_some(boost::asio::buffer(bufIncoming), error);

      if (error == boost::asio::error::eof)
      {
        std::cout << "Server:startServing() - connection was closed by the peer." << std::endl;
        closeConnection(id);
        break;
      }
      else if (error)
      {
        throw boost::system::system_error(error);
      }

      if (len < 1)
      {
        usleep(1000);
        continue;
      }

      std::vector<std::string> newState_str = convertArrayToStringVector(bufIncoming, len);

      notifyObservers(newState_str, id);
    }
  }
  // boost::system::system_error is derived from std::exception
  catch (std::exception& e)
  {
    std::cerr << "Server::startServing() - ERROR: " << e.what() << std::endl;
    closeConnection(id);
    throw;
  }
}


void
Server::write(const std::vector<std::string>& messageStrings, ConnectionId id)
{
  std::string message;
  for (std::size_t i = 0; i < messageStrings.size(); ++i)
  {
    message += "@";
    message += messageStrings[i];
  }
  message += "\0";
  write(message, id);
}


void
Server::write(const std::string& message, ConnectionId id)
{
  boost::asio::ip::tcp::socket* socket = getRawSocket(id);

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
  std::lock_guard<std::mutex> lock(m_mutex);
  m_observers.push_back(observer);
}


void
Server::unregisterObserver(ServerObserver* observer)
{
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

  for (std::size_t i = 0; i < observers.size(); ++i)
  {
    if (observers[i])
    {
      observers[i]->notifyReceivedData(dataStrings, id);
    }
  }
}


std::vector<std::string>
Server::convertArrayToStringVector(std::array<char, 2048> bufIncoming, size_t len)
{
  std::string bufString = bufIncoming.data();
  bufString.resize(len);
  std::vector<std::string> newState_str;
  boost::split(newState_str, bufString, boost::is_any_of("@"));

  return newState_str;
}


void
Server::setPort(int port)
{
  m_port = port;
}


int
Server::getNOpenSockets() const
{
  int nOpen(0);

  for (std::map<ConnectionId, std::unique_ptr<boost::asio::ip::tcp::socket> >::const_iterator iter = m_sockets.begin();
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


int
Server::getNThreads() const
{
  return m_threads.size();
}
