#ifndef SERVER_H
#define SERVER_H

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>


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
  void startServing() const;

  /**
   * Set the port number
   * @param port network connection port number
   */
  void setPort(int port);

  void processIncomingData(boost::asio::ip::tcp::socket& socket, size_t len, const boost::array<char, 2048>& bufIncoming) const;

private:

private:

  /** Server network port number */
  int m_port;
};

#endif // SERVER_H
