#include "server.h"

#include <iostream>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_port(2020)
{
}


Server::~Server()
{
}


void
Server::startServing()
{
  for (;;)
  {
    // start server loop
    try
    {
      boost::asio::io_service io_service;

      tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), m_port));

      boost::array<char, 2048> bufIncoming;
      boost::system::error_code error;

      std::cout << "Server:startServing() - create socket" << std::endl;
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      for (;;)
      {
//      std::cout << "Server:startServing() - read some: " << std::endl;

        // receive a command
        size_t len = socket.read_some(boost::asio::buffer(bufIncoming), error);

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
        std::vector<std::string> newState_str(12);

        boost::split(newState_str, bufString, boost::is_any_of("@"));

        std::string command = newState_str[0];

//      std::cout << "Server:startServing() - command: " << command << std::endl;

        // incoming server commands are processed and delegated
        if (command == "add")
        {
          int a = atoi(newState_str[1].c_str());
          int b = atoi(newState_str[2].c_str());
          int sum = a + b;
          std::string message = std::to_string(sum);
          message += "\0";

          boost::system::error_code ignored_error;
          // send the forces to the client socket
//          std::cout << "Server:startServing() - send message: " << message << std::endl;
          boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
        }
        else
        {
          std::string message = "server: nothing to do!";
          message += "\0";

          boost::system::error_code ignored_error;
          // send the forces to the client socket
//          std::cout << "Server:startServing() - send message: " << message << std::endl;
          boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
        }
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
Server::setPort(int port)
{
  m_port = port;
}
