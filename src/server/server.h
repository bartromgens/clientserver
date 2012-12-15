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

  typedef unsigned int ConnectionId;

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

  void write(const std::string& message, ConnectionId id);
  void write(const std::vector<std::string>& messageStrings, ConnectionId id);

  std::vector<std::string> convertArrayToStringVector(std::array<char, 2048> bufIncoming, size_t len);

  void unregisterObserver(ServerObserver* observer);
  void registerObserver(ServerObserver* observer);
  void notifyObservers(std::vector<std::string> dataStrings, ConnectionId id);

private:
  /**
   * Starts serving
   */
  void startServing(ConnectionId id);

  int openConnection(ConnectionId id);
  void closeConnection(ConnectionId id);
  boost::asio::ip::tcp::socket* getRawSocket(ConnectionId id) const;
  std::vector<std::string> readSome(ConnectionId id);

  int getNThreads() const;
  int getNOpenSockets() const;

private:
  /** Server network port number */
  unsigned short m_port;

  /** provides the core I/O functionality for users of the synchronous I/O objects, including the tcp::socket and tcp::acceptor. */
  std::unique_ptr<boost::asio::io_service> m_io_service;
  /** provides the ability to accept new socket connections */
  std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

  /** map of connection threads, key is the connection ID */
  std::map<ConnectionId, std::unique_ptr<std::thread> > m_threads;
  /** map of sockets that provides blocking stream-oriented socket functionality */
  std::map<ConnectionId, std::unique_ptr<boost::asio::ip::tcp::socket> > m_sockets;

  /** server observers that are notified when a new message comes in from a client */
  std::vector<ServerObserver*> m_observers;

  /** Connection id counter, increases with every new connection thread, never decreases */
  ConnectionId m_nIdCounter;

  mutable std::mutex m_mutex;
};

#endif // SERVER_H
