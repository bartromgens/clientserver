#ifndef OLIB_MB_CLIENT_H
#define OLIB_MB_CLIENT_H

#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "shared/clientserverdata.h"

#include <mutex>
#include <vector>


class Client
{
public:
  /**
   * Constructor.
   *
   * @param ip the IP address of the server to connect to
   * @param port the port number of the server to connect to
   * @param name the name of the client
   */
  Client(const std::string& ip = ClientServerData::defaultIp,
         unsigned short port = ClientServerData::defaultPort,
         const std::string& name = ClientServerData::defaultClientName);

  /**
   * Destructor.
   */
  virtual ~Client();

  bool tryConnect(int nTimes = 1, int interval_ms = 0);

  void connect();

  void disconnect();

  /**
   * Sends a command with arguments to the server.
   * @param command the command
   * @param arguments the arguments
   * @throws boost::system::system_error read or write error
   * @return the server reply message
   */
  std::string sendCommand(const std::string& command,
                          const std::vector<std::string>& arguments,
                          std::string separationCharacter = ClientServerData::separationCharacter) const;

  /**
   * Set the IP address of the server, 127.0.0.1 for localhome (default)
   * @param nIP server IP address
   */
  void setIP(const std::string &nIP);

  /**
   * Set the port of the server, (2020 default)
   * @param port server port number
   */
  void setPort(const std::string &port);

  /**
   * Set the port of the server, (2020 default)
   * @param port server port number
   */
  void setPort(int port);

  /**
   * Returns the set server ip address
   */
  const std::string& getIp() const;

  /**
   * Returns the set server port number
   */
  const std::string& getPort() const;

  /**
   * Returns the client name.
   */
  const std::string& getName() const;

private:
  void createSocket();

private:
  /** provides the core I/O functionality for users of the synchronous I/O objects, including the tcp::socket and tcp::acceptor. */
  std::unique_ptr<boost::asio::io_service> m_io_service;
  /** socket that provides blocking stream-oriented socket functionality*/
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;

  /** Server port number */
  std::string m_port;
  /** IP address of the server to connect to */
  std::string m_ip;
  /** Client name*/
  std::string m_name;

  mutable std::mutex m_mutex;
};

#endif // OLIB_MB_CLIENT_H
