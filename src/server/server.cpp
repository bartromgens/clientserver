#include "server.h"

#include "src/server/dummyapplication.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_io_service(),
    m_socket(),
    m_acceptor(0),
    m_port(2020),
    m_application(0)
{
}


Server::~Server()
{
}


void
Server::open()
{
  if (!m_io_service)
  {
    m_io_service.reset(new boost::asio::io_service());
  }
  if (!m_acceptor)
  {
    m_acceptor = new boost::asio::ip::tcp::acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port));
  }
  m_socket.reset(new tcp::socket(*m_io_service));
}


void
Server::setApplication(DummyApplication *app)
{
  m_application = app;
}

void
Server::startServing()
{
  for (;;)
  {
    // start server loop
    try
    {
      open();

      m_acceptor->accept(*m_socket);

      std::thread t1(&Server::readLoop, this);
      t1.join();
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      throw;
    }
  }
}


void Server::readLoop()
{
  for (;;)
  {
    boost::array<char, 2048> bufIncoming;
    boost::system::error_code error;

    // receive a command
    size_t len = m_socket->read_some(boost::asio::buffer(bufIncoming), error);

    if (error == boost::asio::error::eof)
    {
      std::cout << "Server:startServing() - connection was closed by the peer." << std::endl;
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

    processIncomingData(newState_str);
  }
}


void
Server::processIncomingData(const std::vector<std::string>& incomingData) const
{
  m_application->processIncomingData(incomingData);
}


void
Server::write(const std::vector<std::string>& messageStrings)
{
  std::string message;
  for (std::size_t i = 0; i < messageStrings.size(); ++i)
  {
    message += "@";
    message += messageStrings[i];
  }
  message += "\0";
  write(message);
}


void
Server::write(const std::string& message)
{
  boost::system::error_code ignored_error;
  boost::asio::write(*m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
}


void
Server::setPort(int port)
{
  m_port = port;
}
