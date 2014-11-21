#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include "server/connectionstatus.h"
#include "server/server.h"

#include <QMainWindow>
#include <QtGui>
#include <QTime>
#include <QTimer>

class SimulationServer;

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ServerWindow(QWidget *parent = 0);
  virtual ~ServerWindow();
  
  void createActions();

private:
  void updateTable();

private slots:
  void update();
  void slotStartServer();
  void slotStopServer();

private:
  Ui::ServerWindow *ui;

  std::unique_ptr<std::thread> m_serverThread;

  Server* m_server;
  SimulationServer* m_simulationServer;
  QTimer* m_timer;
  QTime m_timeStatusUpdate;
};

#endif // SERVERWINDOW_H
