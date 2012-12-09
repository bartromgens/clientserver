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
    m_acceptor(0),
    m_port(2020),
    m_application(0),
    m_mutex()
{
}


Server::~Server()
{
  // todo delete sockets
  delete m_acceptor;
}


void
Server::close(int id)
{
  m_mutex.lock();

  std::cout << "Server::close()" << std::endl;

  if (m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    std::cout << "Server::close() - socket.shutdown(): " << error.message() << std::endl;
    m_sockets[id]->close();
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
    m_acceptor = new boost::asio::ip::tcp::acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port));
  }

  m_sockets[id] = new boost::asio::ip::tcp::socket(*m_io_service);

  std::cout << "Server::open() - already open: " << getNOpenSockets() << std::endl;
  std::cout << "Server::open() - threads: " << m_threads.size() << std::endl;

  m_mutex.unlock();
  return id;
}


void
Server::setApplication(DummyApplication *app)
{
  m_mutex.lock();

  m_application = app;

  m_mutex.unlock();
}



void
Server::startServerThread()
{
  m_mutex.lock();
  int id = m_sockets.size()+1;

  m_threads[id] = std::unique_ptr<std::thread>(new std::thread(&Server::startServing, this, id));
  m_threads[id]->detach();

  m_mutex.unlock();
}


void
Server::startServing(int id)
{
  std::cout << "Server::startServing()" << std::endl;
//  for (;;)
//  {
    // start server loop
    open(id);

    try
    {
      m_acceptor->accept(*m_sockets[id]);

      startServerThread();

      for (;;)
      {
        boost::array<char, 2048> bufIncoming;
        boost::system::error_code error;

        // receive a command
        size_t len = m_sockets[id]->read_some(boost::asio::buffer(bufIncoming), error);

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

//        processIncomingData(newState_str);
        m_application->processIncomingData(newState_str, id);
      }
    }
    catch (std::exception& e)
    {
      std::cerr << "Server::startServing() - ERROR: " << e.what() << std::endl;
      close(id);
      throw;
    }
    close(id);
//  }
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
  boost::system::error_code ignored_error;
  boost::asio::write(*m_sockets[id], boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
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

  for (std::map<int, boost::asio::ip::tcp::socket*>::const_iterator iter = m_sockets.begin();
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
