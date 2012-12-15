#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

#include <memory>

#include "client/client.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ClientWindow(QWidget *parent = 0);
  ~ClientWindow();

  void createActions();

private:
  void runClientTest(int id);

private slots:
  void update();
  void slotCreateClient();
  void slotConnectAllClients();
  void slotDisconnectAllClients();
  void slotCrash() const;
  void slotGetSumAllClients();
  
private:
  Ui::ClientWindow* ui;
  std::vector< std::unique_ptr<Client> > m_clients;
  QTimer* m_timer;
  QTime m_time;
};

#endif // CLIENTWINDOW_H
