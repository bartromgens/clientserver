#include "serverwindow.h"
#include "ui_serverwindow.h"

#include "server/serverjson.h"


ServerWindow::ServerWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ServerWindow),
  m_server( new Server() ),
  m_serverJSON( new ServerJSON( m_server ) ),
  m_timer(new QTimer(this)),
  m_timeStatusUpdate()
{
  ui->setupUi(this);
  createActions();

  m_timeStatusUpdate.start();

  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

  slotStartServer();

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
  m_serverThread.reset(new std::thread(&Server::startServer, m_server));
  m_serverThread->detach();
}


void
ServerWindow::slotStopServer()
{
  m_server->stopServer();
}

