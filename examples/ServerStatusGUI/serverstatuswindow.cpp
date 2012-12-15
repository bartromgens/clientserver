#include "serverstatuswindow.h"
#include "ui_serverstatuswindow.h"

ServerStatusWindow::ServerStatusWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ServerStatusWindow)
{
  ui->setupUi(this);
}

ServerStatusWindow::~ServerStatusWindow()
{
  delete ui;
}
