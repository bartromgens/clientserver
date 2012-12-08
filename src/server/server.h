#ifndef SERVER_H
#define SERVER_H

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <memory>

class DummyApplication;

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

  void processIncomingData(const std::vector<std::string>& incomingData) const;

  void open();

  void write(const std::string& message);
  void write(const std::vector<std::string>& messageStrings);

  void setApplication(DummyApplication* app);

  void readLoop();
private:

private:
  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
  boost::asio::ip::tcp::acceptor* m_acceptor;

  /** Server network port number */
  int m_port;
  DummyApplication* m_application;
};

#endif // SERVER_H
