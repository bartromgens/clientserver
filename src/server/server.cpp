#include "server.h"
#include "serverobserver.h"

#include "shared/message.h"

#include <boost/bind.hpp>


Server::Server(unsigned short port)
  : m_port(port),
    m_io_service( new boost::asio::io_service() ),
    m_acceptor(),
    m_threads(),
    m_sockets(),
    m_connectionStatuses(),
    m_nIdCounter(0),
    m_mutex()
{
  std::cout << "Server::Server()" << std::endl;
}


Server::~Server()
{
  std::cout << "Server::~Server()" << std::endl;
  stopServer();
}


int
Server::openConnection(ConnectionId id)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  // add a new socket to the map
  m_sockets[id].reset( new boost::asio::ip::tcp::socket(*m_io_service) );

  std::cout << "Server::openConnection() - id: " << id
            << ", open sockets: " << getNOpenSockets()
            << ", running threads: " << m_threads.size()
            << std::endl;

  return id;
}


void
Server::closeConnection(ConnectionId id)
{
  std::cout << "Server::closeConnection() - id: " << id << std::endl;
  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_sockets.find(id) == m_sockets.end())
  {
    std::cerr << "Server::closeConnection - ERROR: connection with id " << id << " does not exist." << std::endl;
    return;
  }

  if (!m_sockets[id])
  {
    return;
  }

  if (m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    std::cout << "Server::closeConnection() - socket.shutdown() id : " << id << ", " << error.message() << std::endl;
    m_sockets[id]->close();
  }
  m_sockets.erase(id);
  m_threads.erase(id);
  m_connectionStatuses.erase(id);
}


std::shared_ptr<boost::asio::ip::tcp::socket>
Server::getSocket(ConnectionId id) const
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_sockets.find(id) != m_sockets.end())
  {
    return m_sockets.at(id);
  }
  return 0;
}


void
Server::stopServer()
{
  std::cout << "Server::stopServer()" << std::endl;

  std::vector<Server::ConnectionId> socketIds = getSocketIds();
  for (std::size_t i = 0; i < socketIds.size(); i++)
  {
    closeConnection(socketIds[i]);
  }

  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_acceptor && m_acceptor->is_open())
  {
    m_acceptor->close();
  }
  m_io_service->stop();
}


void
Server::startServer()
{
  try
  {
    m_acceptor.reset(new boost::asio::ip::tcp::acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port)));
  }
  catch (std::exception& e)
  {
    std::cerr << "Server::startServer() - ERROR: " << e.what() << std::endl;
    return;
  }

  m_io_service->reset();

  startAccepting(getUniqueConnectionId());

  m_io_service->run();
}


void
Server::startAccepting(ConnectionId id)
{
  std::cout << "Server::startAccepting() = id: " << id << std::endl;

  openConnection(id);

  std::shared_ptr<boost::asio::ip::tcp::socket> socket = getSocket(id);

  m_acceptor->async_accept(*socket, boost::bind( &Server::handleAccept, this, boost::asio::placeholders::error, id ) );
}


void
Server::handleAccept(const boost::system::error_code& error, ConnectionId id)
{
  std::cout << "Server::HandleAccept() : " << error.message() << std::endl;
  if (error)
  {
    std::cerr << "Server:HandleAccept() - id: " << id << ", ERROR: " << error.message() << std::endl;
    closeConnection(id);
    return;
  }

  startServerThread(id);

  // IMPORTANT: after a connection is accepted and started on its own thread, create new socket to accept...
  startAccepting( getUniqueConnectionId() );
}


void
Server::startServerThread(ConnectionId id)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_threads[id] = std::unique_ptr<std::thread>(new std::thread(&Server::serverLoop, this, id));
  m_threads[id]->detach();
  m_connectionStatuses[id].setStatus(ConnectionStatus::unavailable);
}


void
Server::serverLoop(ConnectionId id)
{
  std::cout << "Server::serverLoop() - connection accepted - id: " << id << std::endl;

  bool isOk = true;
  std::shared_ptr<boost::asio::ip::tcp::socket> socket = getSocket(id);

  while (isOk)
  {
    if (!socket)
    {
      std::cout << "Server::serverLoop() - socket not valid - id: " << id << std::endl;
      assert(socket);
    }

    std::array<char, ClientServerData::defaultBufferSize> bufIncoming;
    boost::system::error_code error;
    std::size_t len = socket->read_some(boost::asio::buffer(bufIncoming), error);

    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_connectionStatuses[id].addTotalDataDown(len);
    }

    if (error == boost::asio::error::eof)
    {
      std::cout << "Server:serverLoop() - connection was closed by the peer." << std::endl;
      break;
    }
    else if (error)
    {
      std::cerr << "Server:serverLoop() - ERROR: reading from connection:" << error.message() << std::endl;
      break;
    }

    Message message;
    message.fromRawString( bufIncoming.data() );

//    send(message, id);

    notifyObservers(message, id);
  }

  closeConnection(id);

  std::cout << "Server::serverLoop() - end of thread - id: " << id << std::endl;
}


Server::ConnectionId
Server::getUniqueConnectionId()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_nIdCounter++;
}


void
Server::send(const Message& message, ConnectionId id)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::shared_ptr<boost::asio::ip::tcp::socket> socket = getSocket(id);

  if (!socket)
  {
    std::cerr << "Server::write() - ERROR: socket for connection id " << id << " does not exist!" << std::endl;
    return;
  }

  assert(socket->is_open());

  std::string messageString = message.createMessage();

  boost::system::error_code error;
  boost::asio::write(*socket, boost::asio::buffer( messageString ), boost::asio::transfer_all(), error);
  if (error)
  {
    std::cerr << "Server::write() - ERROR: " << error.message() << std::endl;
    throw boost::system::system_error(error);
  }
  else
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connectionStatuses[id].addTotalDataUp(messageString.size());
  }
}


void
Server::registerObserver(ServerObserver* observer)
{
  std::cout << "Server::registerObserver()" << std::endl;
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
Server::notifyObservers(const Message& message, ConnectionId id)
{
  std::vector<ServerObserver*> observers;
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    observers = m_observers;
  }

  std::for_each(observers.begin(), observers.end(),
                [&message, &id](ServerObserver* observer)
                {
                  observer->notifyReceivedData(message, id);
                });
}


unsigned int
Server::getNOpenSockets() const
{
  int nOpen = 0;

  for (auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
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
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<ConnectionId> socketIds;

  for (auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
  {
    if (iter->second->is_open())
    {
      socketIds.push_back(iter->first);
    }
  }

  return socketIds;
}


std::vector<Server::ConnectionId>
Server::getSocketIds() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<ConnectionId> socketIds;

  for (auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
  {
    socketIds.push_back(iter->first);
  }

  return socketIds;
}


std::vector<Server::ConnectionId>
Server::getOpenThreadIds() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<ConnectionId> threadIds;

  for (auto iter = m_threads.begin(); iter != m_threads.end(); ++iter)
  {
    threadIds.push_back(iter->first);
  }

  return threadIds;
}


ConnectionStatus::EnumConnectionStatus
Server::getConnectionStatus(ConnectionId id) const
{
  if (m_sockets.find(id) != m_sockets.end())
  {
    if (m_sockets.at(id)->is_open())
    {
      return ConnectionStatus::connected;
    }
    else
    {
      return ConnectionStatus::listening;
    }
  }

  return ConnectionStatus::unavailable;
}


std::map<Server::ConnectionId, ConnectionStatus>
Server::getConnectionStatuses()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  updateConnectionStatuses();
  return m_connectionStatuses;
}


void
Server::updateConnectionStatuses()
{
  for (auto iter = m_connectionStatuses.begin(); iter != m_connectionStatuses.end(); ++iter)
  {
    iter->second.setStatus( getConnectionStatus(iter->first) );
  }
}


std::size_t
Server::getNThreads() const
{
  return m_threads.size();
}
