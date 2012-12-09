#ifndef SERVER_H
#define SERVER_H

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <mutex>
#include <thread>

class ServerObserver;

class Server
{
public:
  /**
   * Constructor
   */
  Server(int port = 2020);

  /**
   * Destructor
   */
  ~Server();

  /**
   * Set the port number
   * @param port network connection port number
   */
  void setPort(int port);

  void startServerThread();

  void write(const std::string& message, int id);
  void write(const std::vector<std::string>& messageStrings, int id);

  std::vector<std::string> convertArrayToStringVector(std::array<char, 2048> bufIncoming, size_t len);

  void unregisterObserver(ServerObserver* observer);
  void registerObserver(ServerObserver* observer);
  void notifyObservers(std::vector<std::string> dataStrings, int id);

private:
  /**
   * Starts serving
   */
  void startServing(int id);

  int openConnection(int id);
  void closeConnection(int id);
  boost::asio::ip::tcp::socket* getRawSocket(int id) const;
  std::vector<std::string> readSome(int id);

  int getNThreads() const;
  int getNOpenSockets() const;

private:
  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

  std::map<int, std::unique_ptr<std::thread> > m_threads;
  std::map<int, std::unique_ptr<boost::asio::ip::tcp::socket> > m_sockets;

  std::vector<ServerObserver*> m_observers;

  /** Server network port number */
  int m_port;
  mutable std::mutex m_mutex;
  int m_nIdCounter;
};

#endif // SERVER_H
