#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ServerWindow),
  m_server(new Server()),
  m_timer(new QTimer(this)),
  m_timeStatusUpdate(),
  m_calculator(m_server),
  m_connectionStatuses(),
  m_connectionStatusesOneSecondAgo(),
  m_connectionStatusesTwoSecondAgo()
{
  ui->setupUi(this);
  createActions();
  createTableWidget();

  m_timeStatusUpdate.start();

  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start(200);

  m_server->startServerThread();
}


ServerWindow::~ServerWindow()
{
  delete m_server;
  delete ui;
}


void
ServerWindow::createActions()
{
  QAction* actionStart = new QAction("Start Server", this);
  ui->mainToolBar->addAction(actionStart);
  connect(actionStart, SIGNAL(triggered()), this, SLOT(slotStartServer()));

  QAction* actionStopServer = new QAction("Stop Server", this);
  ui->mainToolBar->addAction(actionStopServer);
  connect(actionStopServer, SIGNAL(triggered()), this, SLOT(slotStopServer()));
}


void
ServerWindow::createTableWidget()
{
  QStringList headers;
  headers.append("ID");
  headers.append("Connection Status");
  headers.append("Downloaded");
  headers.append("Uploaded");
  headers.append("Down Speed");
  headers.append("Up Speed");

  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

  ui->tableWidget->setColumnCount(headers.size());
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setHorizontalHeaderLabels(headers);
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);

  ui->tableWidget->setColumnWidth(0,30);
  ui->tableWidget->setColumnWidth(1,150);
}


void
ServerWindow::update()
{
  updateTable();
}


void
ServerWindow::updateTable()
{
//  int nThreads = m_server->getNThreads();
  std::vector<unsigned int> threadIds = m_server->getOpenThreadIds();

  m_connectionStatuses = m_server->getConnectionStatuses();

  if (m_timeStatusUpdate.elapsed() > 950)
  {
    std::cout << "ServerWindow::updateTable() - elapsed: " << m_timeStatusUpdate.elapsed() << " ms" << std::endl;
    m_timeStatusUpdate.restart();
    m_connectionStatusesTwoSecondAgo = m_connectionStatusesOneSecondAgo;
    m_connectionStatusesOneSecondAgo = m_connectionStatuses;
  }

  ui->tableWidget->setRowCount(threadIds.size());

  for (unsigned int i = 0; i < threadIds.size(); ++i)
  {
    updateTableRow(i, threadIds[i]);
  }
}


void
ServerWindow::updateTableRow(int row, Server::ConnectionId id)
{
  double totalDown_MiB = 0;
  double totalUp_MiB = 0;
  double downSpeed_KiB = 0;
  double upSpeed_kiB = 0;

//  if (m_connectionStatusesOneSecondAgo.find(id) == m_connectionStatusesOneSecondAgo.end()
//      || m_connectionStatuses.find(id) == m_connectionStatuses.end())
//  {
//    return;
//  }

  if (m_connectionStatuses.find(id) != m_connectionStatuses.end()
      && m_connectionStatusesOneSecondAgo.find(id) != m_connectionStatusesOneSecondAgo.end()
      && m_connectionStatusesTwoSecondAgo.find(id) != m_connectionStatusesTwoSecondAgo.end())
  {
    totalDown_MiB = m_connectionStatuses.at(id).totalDataDown_byte / 1024.0 / 1024.0;
    totalUp_MiB = m_connectionStatuses.at(id).totalDataUp_byte / 1024.0 / 1024.0;
    downSpeed_KiB = m_connectionStatusesOneSecondAgo.at(id).totalDataDown_byte / 1024.0 - m_connectionStatusesTwoSecondAgo.at(id).totalDataDown_byte / 1024.0;
    upSpeed_kiB = m_connectionStatusesOneSecondAgo.at(id).totalDataUp_byte / 1024.0 - m_connectionStatusesTwoSecondAgo.at(id).totalDataUp_byte / 1024.0;
  }

  Server::EnumConnectionStatus status = m_server->getConnectionStatus(id);
  std::string statusString = "unknown";
  if (status == Server::connected)
  {
    statusString = "connected";
  }
  else if (status == Server::listening)
  {
    statusString = "listening";
  }
  else if (status == Server::unavailable)
  {
    statusString = "unavailable";
  }

  m_idItem = new QTableWidgetItem( QString::number(id) );
  m_statusItem = new QTableWidgetItem( QString::fromStdString(statusString) );
  m_totalDownItem = new QTableWidgetItem( QString::number(totalDown_MiB, char('f'), 2) + " MiB");
  m_totalUpItem = new QTableWidgetItem( QString::number(totalUp_MiB, char('f'), 2) + " MiB");
  m_downSpeedItem = new QTableWidgetItem( QString::number(downSpeed_KiB, char('g'), 2) + " KiB/s");
  m_upSpeedItem = new QTableWidgetItem( QString::number(upSpeed_kiB, char('f'), 2) + " KiB/s");

  ui->tableWidget->setItem(row, 0, m_idItem);
  ui->tableWidget->setItem(row, 1, m_statusItem);
  ui->tableWidget->setItem(row, 2, m_totalDownItem);
  ui->tableWidget->setItem(row, 3, m_totalUpItem);
  ui->tableWidget->setItem(row, 4, m_downSpeedItem);
  ui->tableWidget->setItem(row, 5, m_upSpeedItem);
}


void
ServerWindow::slotStartServer()
{
  m_server->startServerThread();
}


void
ServerWindow::slotStopServer()
{
  m_server->stopServer();
}

