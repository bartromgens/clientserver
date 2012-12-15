#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ServerWindow),
  m_server(new Server()),
  m_timer(new QTimer(this)),
  m_calculator(m_server)
{
  ui->setupUi(this);
  createActions();
  createTableWidget();

  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start(200);
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
  std::vector<unsigned int> ids = m_server->getOpenSocketIds();
  std::vector<unsigned int> threadIds = m_server->getOpenThreadIds();

  ui->tableWidget->setRowCount(threadIds.size());

  for (unsigned int i = 0; i < threadIds.size(); ++i)
  {
    updateTableRow(i, threadIds[i]);
  }
}


void
ServerWindow::updateTableRow(int row, Server::ConnectionId id)
{
  const std::map<Server::ConnectionId, ConnectionStatus>& connectionStatuses =  m_server->getConnectionStatuses();
  double totalDown_MiB = 0;
  double totalUp_MiB = 0;

  if (connectionStatuses.find(id) != connectionStatuses.end())
  {
    totalDown_MiB = connectionStatuses.at(id).totalDataDown_byte / 1024.0 / 1024.0;
    totalUp_MiB = connectionStatuses.at(id).totalDataUp_byte / 1024.0 / 1024.0;
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
  m_downSpeedItem = new QTableWidgetItem( QString::number(totalDown_MiB, char('g'), 3) + " MiB");
  m_upSpeedItem = new QTableWidgetItem( QString::number(totalUp_MiB, char('g'), 3) + " MiB");

  ui->tableWidget->setItem(row, 0, m_idItem);
  ui->tableWidget->setItem(row, 1, m_statusItem);
  ui->tableWidget->setItem(row, 2, m_downSpeedItem);
  ui->tableWidget->setItem(row, 3, m_upSpeedItem);
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

