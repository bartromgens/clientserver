#ifndef OLIB_MB_CLIENT_H
#define OLIB_MB_CLIENT_H

#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "shared/clientserverdata.h"

#include <mutex>
#include <vector>

class Message;

/**
 * Client side of a synchronous network connection, based on a tcp socket connection.
 *
 * @param Requirements:
 * - Connect to a socket (server) with given IP and port.
 * - Send string based command and arguments to the server.
 * - Receive a reply from the server after sending a message.
 */
class Client : boost::noncopyable
{
public:
  /**
   * Constructor.
   *
   * @param ip the IP address of the server to connect to
   * @param port the port number of the server to connect to
   * @param name the name of the client
   */
  explicit Client(const std::string& ip = ClientServerData::defaultIp,
                  unsigned short port = ClientServerData::defaultPort,
                  const std::string& name = ClientServerData::defaultClientName);

  /**
   * Destructor.
   */
  virtual ~Client();

  /**
   * Connect to the server.
   * @throws boost::system::system_error connection error
   */
  bool connect();

  /**
   * Disconnect the connection with the server.
   * The socket is closed, shutdown send and io_service stopped
   */
  void disconnect();

  bool isConnected() const;

  /**
   * Sends a command with arguments to the server.
   * @param command the command
   * @param arguments the arguments
   * @throws boost::system::system_error read or write error
   * @return the server reply message
   */
  std::string sendMessage(const Message &message) const;

  /**
   * Returns the set server ip address.
   */
  const std::string& getIp() const;

  /**
   * Returns the set server port number
   */
  unsigned short getPort() const;

  /**
   * Returns the client name.
   */
  const std::string& getName() const;

private:
  /**
   * Open the client by creating a io_service and new socket.
   * When it does not yet exist, a new io_service is created
   * A new socket is created (the old one deleted)
   */
  void createSocket();

  /**
   * Set the IP address of the server, 127.0.0.1 for localhome (default)
   * @param nIP server IP address
   */
  void setIP(const std::string& nIP);

  /**
   * Set the port of the server
   * @param port server port number
   */
  void setPort(unsigned short port);

private:
  /** provides the core I/O functionality for users of the synchronous I/O objects, including the tcp::socket and tcp::acceptor. */
  std::unique_ptr<boost::asio::io_service> m_io_service;
  /** socket that provides blocking stream-oriented socket functionality*/
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;

  /** Server port number */
  unsigned short m_port;
  /** IP address of the server to connect to */
  std::string m_ip;
  /** Client name*/
  std::string m_name;

  mutable std::mutex m_mutex;
};

#endif // OLIB_MB_CLIENT_H
