#include "server.h"

#include "src/server/dummyapplication.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_io_service(),
    m_threads(),
    m_sockets(),
    m_acceptor(),
    m_port(2020),
    m_application(0),
    m_mutex(),
    m_nIdCounter(0)
{
}


Server::~Server()
{
}


void
Server::close(int id)
{
  m_mutex.lock();

  std::cout << "Server::close() - id: " << id << std::endl;

  if (m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    std::cout << "Server::close() - socket.shutdown(): " << error.message() << std::endl;
    m_sockets[id]->close();

    m_sockets.erase(id);
    m_threads.erase(id);
  }

  m_mutex.unlock();
}


int
Server::open(int id)
{
  m_mutex.lock();

  if (!m_io_service)
  {
    m_io_service.reset(new boost::asio::io_service());
  }
  if (!m_acceptor)
  {
    m_acceptor = std::unique_ptr<boost::asio::ip::tcp::acceptor>(new boost::asio::ip::tcp::acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port)));
  }

  m_sockets[id].reset(new boost::asio::ip::tcp::socket(*m_io_service));

  std::cout << "Server::open() - already open: " << getNOpenSockets() << std::endl;
  std::cout << "Server::open() - threads: " << m_threads.size() << std::endl;

  m_mutex.unlock();
  return id;
}


boost::asio::ip::tcp::socket*
Server::getRawSocket(int id) const
{
  m_mutex.lock();
  boost::asio::ip::tcp::socket* socket = m_sockets.at(id).get();
  m_mutex.unlock();
  return socket;
}


void
Server::startServerThread()
{
  m_mutex.lock();
  int id = m_nIdCounter++;

  m_threads[id] = std::unique_ptr<std::thread>(new std::thread(&Server::startServing, this, id));
  m_threads[id]->detach();

  m_mutex.unlock();
}


void
Server::startServing(int id)
{
  std::cout << "Server::startServing()" << std::endl;
  open(id);

  try
  {
    boost::asio::ip::tcp::socket* socket = getRawSocket(id);
    m_acceptor->accept(*socket);
    std::cout << "Server::startServing() - socket accepted" << std::endl;

    startServerThread();

    for (;;)
    {
      boost::array<char, 2048> bufIncoming;
      boost::system::error_code error;

      // receive a command
      size_t len = socket->read_some(boost::asio::buffer(bufIncoming), error);

      if (error == boost::asio::error::eof)
      {
        std::cout << "Server:startServing() - connection was closed by the peer." << std::endl;
        close(id);
        break;
      }

      if (len < 1)
      {
        usleep(1000);
        continue;
      }

      std::string bufString = bufIncoming.data();
      bufString.resize(len);

      std::vector<std::string> newState_str;
      boost::split(newState_str, bufString, boost::is_any_of("@"));

      m_application->processIncomingData(newState_str, id);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Server::startServing() - ERROR: " << e.what() << std::endl;
    close(id);
    throw;
  }
}


void
Server::write(const std::vector<std::string>& messageStrings, int id)
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
Server::write(const std::string& message, int id)
{
  boost::asio::ip::tcp::socket* socket = getRawSocket(id);

  boost::system::error_code ignored_error;
  boost::asio::write(*socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
}


void
Server::setApplication(DummyApplication *app)
{
  m_mutex.lock();

  m_application = app;

  m_mutex.unlock();
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

  for (std::map<int, std::unique_ptr<boost::asio::ip::tcp::socket> >::const_iterator iter = m_sockets.begin();
       iter != m_sockets.end(); ++iter)
  {
    if (iter->second->is_open())
    {
      nOpen++;
    }
  }

  return nOpen;
}


int
Server::getNThreads() const
{
  return m_threads.size();
}



