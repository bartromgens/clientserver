#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTime>
#include <QTimer>

#include "calculator.h"
#include "server/connectionstatus.h"
#include "server/server.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ServerWindow(QWidget *parent = 0);
  ~ServerWindow();
  
  void createActions();
  void createTableWidget();

private:
  void updateTable();
  void updateTableRow(int row, Server::ConnectionId id);

private slots:
  void update();
  void slotStartServer();
  void slotStopServer();

private:
  Ui::ServerWindow *ui;

  Server* m_server;
  QTimer* m_timer;
  QTime m_timeStatusUpdate;

  QTableWidgetItem* m_idItem;
  QTableWidgetItem* m_statusItem;
  QTableWidgetItem* m_totalDownItem;
  QTableWidgetItem* m_totalUpItem;
  QTableWidgetItem* m_downSpeedItem;
  QTableWidgetItem* m_upSpeedItem;

  Calculator m_calculator;

  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatuses;
  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatusesOneSecondAgo;
  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatusesTwoSecondAgo;
};

#endif // SERVERWINDOW_H
