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
  m_timer->start(1000);
}


ServerWindow::~ServerWindow()
{
  delete m_server;
  delete ui;
}


void
ServerWindow::createActions()
{
  QAction* actionStart = new QAction("Sart Server", this);
  ui->mainToolBar->addAction(actionStart);
  connect(actionStart, SIGNAL(triggered()), this, SLOT(slotStartServer()));
}


void
ServerWindow::createTableWidget()
{
  QStringList headers;
  headers.append("ID");
  headers.append("Connection Status");

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
ServerWindow::slotStartServer()
{
  m_server->startServerThread();
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
ServerWindow::updateTableRow(int row, int id)
{
  Server::ConnectionStatus status = m_server->getConnectionStatus(id);
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
//  m_designationItem = new QTableWidgetItem( QString::fromStdString(designation) );

  ui->tableWidget->setItem(row, 0, m_idItem);
  ui->tableWidget->setItem(row, 1, m_statusItem);
//  ui->tableWidget->setItem(row, 2, m_designationItem);
}
