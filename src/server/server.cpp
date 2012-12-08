#include "server.h"

#include "src/server/dummyapplication.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_io_service(),
//    m_socket(),
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
  if (m_sockets[id]->is_open())
  {
    boost::system::error_code error;
    m_sockets[id]->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    std::cout << "Server::close() - socket.shutdown(): " << error.message() << std::endl;
    m_sockets[id]->close();
  }
  m_mutex.unlock();
}


int
Server::open()
{
  m_mutex.lock();

  std::cout << "Server::open()" << std::endl;
  if (!m_io_service)
  {
    m_io_service.reset(new boost::asio::io_service());
  }
  if (!m_acceptor)
  {
    m_acceptor = new boost::asio::ip::tcp::acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port));
  }
  int id = m_sockets.size()+1;
  m_sockets[id] = new boost::asio::ip::tcp::socket(*m_io_service);

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
Server::startServing()
{
  std::cout << "Server::startServing()" << std::endl;
  for (;;)
  {
    // start server loop

    try
    {
      int id = open();

      std::cout << "Server::startServing() - waiting for accept, id: " << id << std::endl;
      m_acceptor->accept(*m_sockets[id]);
      std::cout << "Server::startServing() - accepted, id: " << id << std::endl;

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
      throw;
    }
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
  boost::system::error_code ignored_error;
  boost::asio::write(*m_sockets[id], boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
}


void
Server::setPort(int port)
{
  m_port = port;
}
