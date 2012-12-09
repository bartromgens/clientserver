#ifndef SERVER_H
#define SERVER_H

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <mutex>
#include <thread>

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
  void startServing(int id);

  /**
   * Set the port number
   * @param port network connection port number
   */
  void setPort(int port);

  int open(int id);

  void write(const std::string& message, int id);
  void write(const std::vector<std::string>& messageStrings, int id);

  void setApplication(DummyApplication* app);

  void close(int id);
  std::vector<std::string> readSome(int id);
  void startServerThread();
  int getNOpenSockets() const;
  int getNThreads() const;
  boost::asio::ip::tcp::socket* getRawSocket(int id);
private:

private:
  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::map<int, std::unique_ptr<std::thread> > m_threads;
  std::map<int, std::unique_ptr<boost::asio::ip::tcp::socket> > m_sockets;
  boost::asio::ip::tcp::acceptor* m_acceptor;

  /** Server network port number */
  int m_port;
  DummyApplication* m_application;
  std::mutex m_mutex;
  int m_nIdCounter;
};

#endif // SERVER_H
