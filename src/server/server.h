#ifndef SERVER_H
#define SERVER_H

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <memory>

class Server
{
public:
  /**
   * Constructor
   */
  Server();

  /**
   * Destructor
   */
  ~Server();

  /**
   * Starts serving
   */
  void startServing();

  /**
   * Set the port number
   * @param port network connection port number
   */
  void setPort(int port);

  void processIncomingData(boost::asio::ip::tcp::socket& socket, std::vector<std::string> incomingData) const;

  void open();

private:

private:
  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;

  /** Server network port number */
  int m_port;
};

#endif // SERVER_H
