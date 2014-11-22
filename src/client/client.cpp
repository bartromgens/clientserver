#include "client.h"



Client::Client(const std::string& ip, unsigned short port, const std::string& name)
  : m_io_service(new boost::asio::io_service()),
    m_socket(),
    m_port(port),
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
  m_socket.reset(new boost::asio::ip::tcp::socket(*m_io_service));
}


bool
Client::connect()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  createSocket();

  boost::asio::ip::tcp::resolver resolver(*m_io_service);
  boost::asio::ip::tcp::resolver::query query(m_ip, std::to_string(m_port));
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator end;

  boost::system::error_code error = boost::asio::error::host_not_found;

  // keeps the client program independent of a specific IP version.
  while (error && endpoint_iterator != end)
  {
    m_socket->close();
    m_socket->connect(*endpoint_iterator++, error);
  }
  if (error)
  {
    std::cerr << "Client::connect() - error while trying to connect: " << error.message() << std::endl;
    return false;
  }

  std::cout << "Client::connect() - connected: " << error.message() << std::endl;
  return true;
}


void
Client::disconnect()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (!m_socket)
  {
    return;
  }

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


bool
Client::isConnected() const
{
  if (m_socket)
  {
    return m_socket->is_open();
  }

  return false;
}


std::string
Client::sendCommand(const std::string& command,
                    const std::vector<std::string>& arguments,
                    std::string separationCharacter) const
{
  std::lock_guard<std::mutex> lock(m_mutex);

  std::string reply;

  try
  {
    if (!isConnected())
    {
      throw boost::system::system_error(ECONNABORTED, boost::system::system_category());
    }

    std::array<char, ClientServerData::defaultBufferSize> buf;
    std::size_t len;

    boost::system::error_code error;

    std::string message = createMessage(command, arguments, separationCharacter);

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
    }
    else if (error)
    {
      throw boost::system::system_error(error); // Some other error.
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    throw;
  }

  return reply;
}


std::string
Client::createMessage(const std::string& command, const std::vector<std::string>& arguments, std::string separationCharacter) const
{
  std::string message = command;
  for (std::size_t i = 0; i < arguments.size(); ++i)
  {
    message += separationCharacter + arguments[i];
  }

  return message;
}


void
Client::setIP(const std::string& ipAddress)
{
  assert(!ipAddress.empty());

  // verify IP address
  boost::asio::ip::address ipv4Addr = boost::asio::ip::address::from_string(ipAddress);
  if (!ipv4Addr.is_v4())
  {
    std::cerr << "Client::setIP() : not a correct IPv4 address." << std::endl;
    assert(0);
    return;
  }

  m_ip = ipAddress;
}


const std::string&
Client::getIp() const
{
  return m_ip;
}


void
Client::setPort(unsigned short port)
{
  m_port = port;
}


unsigned short
Client::getPort() const
{
  return m_port;
}


const std::string&
Client::getName() const
{
  return m_name;
}
