#include "server.h"

#include <iostream>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_io_service(),
    m_socket(),
    m_port(2020)
{
}


Server::~Server()
{
}


void
Server::open()
{
  m_io_service.reset(new boost::asio::io_service());
  m_socket.reset(new tcp::socket(*m_io_service));
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

      tcp::acceptor acceptor(*m_io_service, tcp::endpoint(tcp::v4(), m_port));
      acceptor.accept(*m_socket);

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
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      throw;
    }
  }
}


void
Server::processIncomingData(const std::vector<std::string>& incomingData) const
{
  std::string command = incomingData[0];

  // incoming server commands are processed and delegated
  if (command == "add")
  {
    int a = atoi(incomingData[1].c_str());
    int b = atoi(incomingData[2].c_str());
    int sum = a + b;
    std::string message = std::to_string(sum);
    message += "\0";

    boost::system::error_code ignored_error;
    boost::asio::write(*m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
  }
  else
  {
    std::string message = "server: nothing to do!";
    message += "\0";

    boost::system::error_code ignored_error;
    boost::asio::write(*m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
  }
}


void
Server::setPort(int port)
{
  m_port = port;
}
