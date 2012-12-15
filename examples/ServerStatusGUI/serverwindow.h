#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtGui>
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

  QTableWidgetItem* m_idItem;
  QTableWidgetItem* m_statusItem;
  QTableWidgetItem* m_downSpeedItem;
  QTableWidgetItem* m_upSpeedItem;

  Calculator m_calculator;
};

#endif // SERVERWINDOW_H
