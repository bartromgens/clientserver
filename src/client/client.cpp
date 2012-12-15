#include "client.h"

#include <cmath>
#include <thread>

using boost::asio::ip::tcp;


Client::Client(const std::string& ip, unsigned short port, const std::string& name)
  : m_io_service(new boost::asio::io_service()),
    m_socket(),
    m_port(),
    m_ip(),
    m_name(name),
    m_mutex()
{
  std::cout << "Client::Client()" << std::endl;

  setPort(port);
  setIP(ip);
}


Client::~Client()
{
  std::cout << "Client::~Client()" << std::endl;
  if (m_socket->is_open())
  {
    disconnect();
  }
  if (m_io_service)
  {
    m_io_service->stop();
  }
}


void
Client::createSocket()
{
  m_socket.reset(new tcp::socket(*m_io_service));
}


void
Client::connect()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  createSocket();

  tcp::resolver resolver(*m_io_service);
  tcp::resolver::query query(m_ip, m_port);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;

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

  std::cout << "Client::connect() - status: " << error.message() << std::endl;
}


bool
Client::tryConnect(int nTimes, int interval_ms)
{
  for (int i = 0; i < nTimes; ++i)
  {
    try
    {
      connect();
      std::cout << "Client::tryConnect() - CONNECTED!: " << m_name << std::endl;
      return true;
    }
    catch(boost::system::system_error& error)
    {
      std::cout << "Client::tryConnect() - failed: " <<  error.what() << std::endl;
    }
    usleep(interval_ms*1000);
  }
  return false;
}


void
Client::disconnect()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  boost::system::error_code error;
  m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
  if (error)
  {
    std::cerr << "Client::disconnect() - shutdown ERROR: " << error.message() << std::endl;
  }

  std::cout << "Client::disconnect() - socket.shutdown(): " << error.message() << std::endl;

  m_socket->close();
  m_io_service->stop();
}


std::string
Client::sendCommand(const std::string& command,
                    const std::vector<std::string>& arguments,
                    std::string separationCharacter) const
{
  std::lock_guard<std::mutex> lock(m_mutex);

//  std::cout << "Client::sendCommand() - start" << std::endl;
  std::string reply;

  try
  {
    std::array<char, 2048> buf;
    std::size_t len;

    boost::system::error_code error;

    // create message to send to server
    std::string message = command;
    for (std::size_t i = 0; i < arguments.size(); ++i)
    {
      message += separationCharacter + arguments[i];
    }

    assert(m_socket->is_open());

    // send to server
    boost::asio::write(*m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), error);
    if (error)
    {
      throw boost::system::system_error(error);
    }

    // receive reply
    len = m_socket->read_some(boost::asio::buffer(buf), error);

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
    assert(0);
    throw;
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

const
std::string &Client::getName() const
{
  return m_name;
}
