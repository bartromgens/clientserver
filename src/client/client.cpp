#include "client.h"

#include <cmath>


using boost::asio::ip::tcp;


Client::Client()
  : m_io_service(0),
    m_socket(0),
    m_port("2020"),
    m_ip("127.0.0.1")
{
  std::cout << "Client::Client()" << std::endl;
}


Client::~Client()
{
  std::cout << "Client::~Client()" << std::endl;
  disconnect();
}


bool
Client::tryConnect()
{
  try
  {
    connect();
    return true;
  }
  catch(boost::system::system_error& error)
  {
    std::cout << "Client::tryConnect() - failed: " <<  error.what() << std::endl;
    return false;
  }
}


void
Client::connect()
{
  std::cout << "Client::Client() - start" << std::endl;

  m_io_service = new boost::asio::io_service();

  tcp::resolver resolver(*m_io_service);
  tcp::resolver::query query(m_ip, m_port);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;

  m_socket = new tcp::socket(*m_io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;

  // keeps the client program independent of a specific IP version.
  while (error && endpoint_iterator != end)
  {
    m_socket->close();
    m_socket->connect(*endpoint_iterator++, error);
  }
  if (error)
  {
    throw boost::system::system_error(error);
  }
  std::cout << "Client::Client() - end" << std::endl;
}


void
Client::disconnect()
{
  boost::system::error_code error;
  m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
  std::cout << "Client::disconnect() - socket.shutdown(): " << error.message() << std::endl;

  m_socket->close();
  m_io_service->stop();

  delete m_io_service;
  delete m_socket;
}


std::string
Client::sendCommand(const std::string &command, const std::vector<std::string> &arguments) const
{
//  std::cout << "Client::sendCommand() - start" << std::endl;
  std::string reply;

  try
  {
    boost::array<char, 2048> buf;
    std::size_t len;

    boost::system::error_code error;

    // create message to send to server
    std::string message = command;
    for (std::size_t i = 0; i < arguments.size(); ++i)
    {
      message += "@" + arguments[i];
    }
    message += "\0";

    // send to server
    boost::asio::write(*m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), error);
    if (error != 0)
    {
      std::cout << "Client::sendCommand() - write error: " << error.message() << std::endl;
    }

    // receive reply
    len = m_socket->read_some(boost::asio::buffer(buf), error);

    if (error != 0)
    {
      std::cout << "Client::sendCommand() - read some: " << error.message() << std::endl;
    }

    reply = buf.data();
    reply.resize(len);

    if (error == boost::asio::error::eof)
    {
      std::cout << "Client::sendCommand() - connection closed cleanly by peer." << std::endl;
      // Connection closed cleanly by peer.
    }
    else if (error)
    {
      throw boost::system::system_error(error); // Some other error.
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

//  std::cout << "Client::sendCommand() - end" << std::endl;

  return reply;
}


void
Client::setIP(const std::string &ipAddress)
{
  m_ip = ipAddress;
}


void
Client::setPort(const std::string &port)
{
  m_port = port;
}


void
Client::setPort(int port)
{
  m_port = std::to_string(port);
}


const std::string&
Client::getIp() const
{
  return m_ip;
}


const std::string&
Client::getPort() const
{
  return m_port;
}
