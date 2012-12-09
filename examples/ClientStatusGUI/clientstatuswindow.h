#ifndef CLIENTSTATUSWINDOW_H
#define CLIENTSTATUSWINDOW_H

#include <QMainWindow>

#include <map>
#include <mutex>

class Client;

namespace Ui {
class ClientStatusWindow;
}

class ClientStatusWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ClientStatusWindow(QWidget *parent = 0);
  ~ClientStatusWindow();
  
  void createActions();

  void createNewClient();
  void connectAllClients();
  void disconnectAllClients();
  void runAllClients();
private:
  bool runClient(Client *client);

private slots:
  void slotNewClient();
  void slotConnectAllClients();
  void slotRunAllClients();
  void slotDisconnectAllClients();

private:
  Ui::ClientStatusWindow *ui;
  int m_nClientsCreated;
  std::map<int, Client*> m_clients;
  std::mutex m_mutex;
};

#endif // CLIENTSTATUSWINDOW_H
