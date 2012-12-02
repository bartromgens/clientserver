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
   * Starts the server after the orpheus side is intialized, a model loaded and analysis opened.
   * It is now ready to receive state, solve and return corresponding forces and moments.
   */
  void startServing();

  /**
   * Set the port number
   * @param port network connection port number
   */
  void setPort(int port);

  void processIncomingData(boost::asio::ip::tcp::socket socket, size_t len, boost::array<char, 2048> bufIncoming);

private:

private:

  /** Server network port number */
  int m_port;
};

#endif // SERVER_H
