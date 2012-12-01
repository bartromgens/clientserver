#include "server.h"

#include <iostream>
#include <sstream>
#include <vector>

using boost::asio::ip::tcp;


Server::Server()
  : m_isInitialised(false),
    m_isModelLoaded(false),
    m_isServing(false),
    m_port(2020)
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
    try
    {
      boost::asio::io_service io_service;

      tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), m_port));

      boost::array<char, 2048> bufIncoming;
      boost::system::error_code error;

      m_isServing = true;

      std::cout << "Server:startServing() - create socket" << std::endl;
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      // start server loop
      for (;;)
      {

        // receive a command
        //      std::cout << "Server:startServing() - read some: " << std::endl;
        if (io_service.stopped())
        {
          std::cout << "Server:startServing() - socket not open" << std::endl;
        }

        size_t len = socket.read_some(boost::asio::buffer(bufIncoming), error);
        if (error == boost::asio::error::eof)
        {
          std::cout << "Server:startServing() - connection was closed by the peer." << std::endl;
          break;
        }

        if (len < 1)
        {
          usleep(100000);
          continue;
        }

        std::string bufString = bufIncoming.data();
        bufString.resize(len);
        std::vector<std::string> newState_str(12);

        boost::split(newState_str, bufString, boost::is_any_of("@"));

        std::string command = newState_str[0];

        std::cout << "Server:startServing() - command: " << command << std::endl;

        // incoming server commands are processed and delegated
        if (command == "setState")
        {
          int id = atoi(newState_str[1].c_str());
          std::cout << "OLIB_MB_SERVER::startServing() - id: " << id << std::endl;

          boost::array<double, 6> forces;
          forces.fill(0.0);
          sendForces(socket, forces);
        }
        else
        {
          std::string message = "server: nothing to do!";
          message += "\0";

          boost::system::error_code ignored_error;
          // send the forces to the client socket
          std::cout << "Server:startServing() - send message: " << message << std::endl;
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
Server::sendForces(boost::asio::ip::tcp::socket &socket, const boost::array<double, 6> &forces) const
{
  // create message to send back to the client
  std::string message = "force";
  for (size_t i = 0; i < forces.size(); ++i)
  {
    message += "@" + std::to_string(forces[i]);
  }
  message += "\0";

  boost::system::error_code ignored_error;
  // send the forces to the client socket
  boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
}


void
Server::sendStatus(boost::asio::ip::tcp::socket &socket, std::string statusMessage) const
{
  // create message to send back to the client
  std::string message = "";

  message += statusMessage;
  message += "\0";

  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
}


void
Server::setPort(int port)
{
  m_port = port;
}
