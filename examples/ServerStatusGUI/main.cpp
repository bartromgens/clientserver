#include <QApplication>
#include "serverstatuswindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ServerStatusWindow w;
  w.show();
  
  return a.exec();
}
