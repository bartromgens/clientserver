#ifndef OLIB_MB_CLIENT_H
#define OLIB_MB_CLIENT_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vector>


class Client
{
public:
  /**
   * Constructor
   * @param id the unique client ID mapping it to the Orpheus module analysis
   */
  Client();

  /**
   * Destructor
   */
  virtual ~Client();

  /**
   * Sends the state to the server, let the server solve for this state and get the forces and moments back
   * @param command the server command
   * @param arguments the arguments for the command
   * @returns the reply from the server
   */
  std::string sendCommand(const std::string &command, const std::vector<std::string> &arguments) const;

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

  const std::string& getName() const;

  void setName(const std::string& name);

  bool tryConnect(int nTimes, int interval_ms);
  void connect();
  void disconnect();

private:
  void open();

private:
  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;

  /** The unique client/bearing ID*/
  int m_id;
  /** Server network port number in string format */
  std::string m_port;
  /** Server IP address */
  std::string m_ip;

  std::string m_name;


};

#endif // OLIB_MB_CLIENT_H
