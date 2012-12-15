#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ServerWindow),
  m_server(new Server()),
  m_timer(new QTimer(this)),
  m_timeStatusUpdate(),
  m_calculator(m_server)
{
  ui->setupUi(this);
  createActions();

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
ServerWindow::update()
{
  updateTable();
}


void
ServerWindow::updateTable()
{
  std::vector<unsigned int> threadIds = m_server->getOpenThreadIds();

  ui->connectionStatusTableWidget->updateTable(threadIds, m_server->getConnectionStatuses());
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

