#include "clientstatuswindow.h"
#include "ui_clientstatuswindow.h"

#include "client/client.h"

#include <thread>

ClientStatusWindow::ClientStatusWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientStatusWindow),
  m_nClientsCreated(0)
{
  ui->setupUi(this);

  createActions();
}


ClientStatusWindow::~ClientStatusWindow()
{
  for (std::map<int, Client*>::iterator iter = m_clients.begin();
       iter != m_clients.end(); ++iter)
  {
    delete iter->second;
  }
  m_clients.clear();

  delete ui;
}


void
ClientStatusWindow::createActions()
{
  QAction* newClient = new QAction("New Client", this);
  connect(newClient, SIGNAL(triggered()), this, SLOT(slotNewClient()));
  ui->mainToolBar->addAction(newClient);

  QAction* connectAllClients = new QAction("Connect all Clients", this);
  connect(connectAllClients, SIGNAL(triggered()), this, SLOT(slotConnectAllClients()));
  ui->mainToolBar->addAction(connectAllClients);


  QAction* disconnectAllClients = new QAction("Disconnect all Clients", this);
  connect(disconnectAllClients, SIGNAL(triggered()), this, SLOT(slotDisconnectAllClients()));
  ui->mainToolBar->addAction(disconnectAllClients);

  QAction* runAllClients = new QAction("Run all Clients", this);
  connect(runAllClients, SIGNAL(triggered()), this, SLOT(slotRunAllClients()));
  ui->mainToolBar->addAction(runAllClients);
}


void
ClientStatusWindow::createNewClient()
{
  int id = m_nClientsCreated++;
  m_clients[id] = new Client();
}


void
ClientStatusWindow::connectAllClients()
{
  for (std::map<int, Client*>::iterator iter = m_clients.begin();
       iter != m_clients.end(); ++iter)
  {
    iter->second->connect();
  }
}

void
ClientStatusWindow::runAllClients()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (std::map<int, Client*>::iterator iter = m_clients.begin();
       iter != m_clients.end(); ++iter)
  {
    std::thread t1(&ClientStatusWindow::runClient, this, iter->second);
    t1.detach();
  }
}


void
ClientStatusWindow::disconnectAllClients()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (std::map<int, Client*>::iterator iter = m_clients.begin();
       iter != m_clients.end(); ++iter)
  {
    iter->second->disconnect();
  }
}


void
ClientStatusWindow::slotNewClient()
{
  createNewClient();
}


void
ClientStatusWindow::slotConnectAllClients()
{
  std::thread t1(&ClientStatusWindow::connectAllClients, this);
  t1.detach();
}


void
ClientStatusWindow::slotRunAllClients()
{
  std::thread t1(&ClientStatusWindow::runAllClients, this);
  t1.detach();
}


void
ClientStatusWindow::slotDisconnectAllClients()
{
  std::thread t1(&ClientStatusWindow::disconnectAllClients, this);
  t1.detach();
}


bool
ClientStatusWindow::runClient(Client* client)
{
  std::cout << "ClientStatusWindow::runClient()" << std::endl;

  int nRounds = 100*1000;
  for (int i = 0; i < nRounds; i++)
  {
    try
    {
      std::vector<std::string> arguments;
      arguments.push_back(std::to_string(i*2));
      arguments.push_back(std::to_string(i));
      std::string reply = client->sendCommand("add", arguments);
      int sum = atoi(reply.c_str());
      assert(sum == i*3);
//      std::cout << "ClientTestGroup::runClient() - " << client.getName() << ", reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "ClientTestGroup::runClient() - ERROR sending command: " << e.what() << std::endl;
      assert(0);
      throw;
    }
  }

  std::cout << "ClientStatusWindow::runClient()" << std::endl;
  return true;
}
