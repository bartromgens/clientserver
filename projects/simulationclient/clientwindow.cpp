#include "clientwindow.h"
#include "ui_clientwindow.h"

#include "shared/message.h"

#include "../simulationserver/messagejson.h"

#include <thread>

ClientWindow::ClientWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientWindow),
  m_clients(),
  m_timer(new QTimer(this)),
  m_time()
{
  ui->setupUi(this);
  m_time.start();

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
  connect(actionRunClients, SIGNAL(triggered()), this, SLOT(slotRunClients()));
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
ClientWindow::slotRunClients()
{
  m_time.restart();
  for (unsigned int i = 0; i < m_clients.size(); ++i)
  {
    if (m_clients[i]->isConnected())
    {
      runClientTest(i);
    }
  }
  std::cout << "ClientWindow::slotRunClients() : time elapsed: " << m_time.elapsed() << std::endl;
}


void
ClientWindow::runClientTest(int id)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  GetParameters command;
  std::string json = command.serialize();
  std::cout << json << std::endl;

  Message message(0, 0);
  message.setData( json );

  std::string reply = m_clients[id]->sendMessage( message );
  Parameters parametersMessage;
  parametersMessage.deserialize(reply);

  std::vector<Parameter> parameters = parametersMessage.getParameters();

  std::cout << "parameters received: " << std::endl;
  for (auto iter = parameters.begin(); iter != parameters.end(); ++iter)
  {
    std::cout << iter->id << ", " << iter->name << std::endl;
  }
}
