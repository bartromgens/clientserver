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

private:
  /**
   * Sends the forces back to the client
   * @param socket the boost socket
   * @param forces vector with interface forces and moments
   */
  void sendForces(boost::asio::ip::tcp::socket &socket, const boost::array<double, 6> &forces) const;

private:

  /** State flags */
  bool m_isInitialised;
  bool m_isModelLoaded;
  bool m_isServing;

  /** Server network port number */
  int m_port;
};

#endif // SERVER_H
