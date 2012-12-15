#include "clientwindow.h"
#include "ui_clientwindow.h"

#include <thread>

ClientWindow::ClientWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientWindow),
  m_clients(),
  m_timer(new QTimer(this))
{
  ui->setupUi(this);

  createActions();

  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start(1000);
}


ClientWindow::~ClientWindow()
{
  delete ui;
}


void
ClientWindow::createActions()
{
  QAction* actionStartClient = new QAction("New Client", this);
  ui->mainToolBar->addAction(actionStartClient);
  connect(actionStartClient, SIGNAL(triggered()), this, SLOT(slotCreateClient()));

  QAction* actionConnectClients = new QAction("Connect Clients", this);
  ui->mainToolBar->addAction(actionConnectClients);
  connect(actionConnectClients, SIGNAL(triggered()), this, SLOT(slotConnectAllClients()));

  QAction* actionDisonnectClients = new QAction("Disconnect Clients", this);
  ui->mainToolBar->addAction(actionDisonnectClients);
  connect(actionDisonnectClients, SIGNAL(triggered()), this, SLOT(slotDisconnectAllClients()));

  QAction* actionCrash = new QAction("Crash", this);
  ui->mainToolBar->addAction(actionCrash);
  connect(actionCrash, SIGNAL(triggered()), this, SLOT(slotCrash()));

  QAction* actionRunClients = new QAction("Run Clients", this);
  ui->mainToolBar->addAction(actionRunClients);
  connect(actionRunClients, SIGNAL(triggered()), this, SLOT(slotGetSumAllClients()));
}


void
ClientWindow::update()
{
}


void
ClientWindow::slotCreateClient()
{
  m_clients.push_back(std::unique_ptr<Client>(new Client()));
}


void
ClientWindow::slotConnectAllClients()
{
  for (std::size_t i = 0; i < m_clients.size(); ++i)
  {
    try
    {
      m_clients[i]->connect();
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }
}


void
ClientWindow::slotDisconnectAllClients()
{
  for (std::size_t i = 0; i < m_clients.size(); ++i)
  {
    m_clients[i]->disconnect();
  }
}


void
ClientWindow::slotCrash() const
{
  std::vector<int> vector(0);
  vector[1] = 0;
}


void
ClientWindow::slotGetSumAllClients()
{
  for (unsigned int i = 0; i < m_clients.size(); ++i)
  {
    std::thread t1(&ClientWindow::runClientTest, this, i);
    t1.detach();
  }
}


void
ClientWindow::runClientTest(int id)
{
  for (std::size_t i = 0; i < 500000; ++i)
  {
    std::vector<std::string> arguments;
    arguments.push_back(std::to_string(i));
    arguments.push_back(std::to_string(2*i));

    try
    {
      std::string reply = m_clients[id]->sendCommand("add", arguments);
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      break;
    }
//    std::cout << "ClientWindow::slotGetSumAllClients() - client: " << id << ", reply: " << reply << std::endl;
  }
}
