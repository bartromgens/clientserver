#ifndef SERVERSTATUSWINDOW_H
#define SERVERSTATUSWINDOW_H

#include <QMainWindow>

namespace Ui {
class ServerStatusWindow;
}

class ServerStatusWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ServerStatusWindow(QWidget *parent = 0);
  ~ServerStatusWindow();
  
private:
  Ui::ServerStatusWindow *ui;
};

#endif // SERVERSTATUSWINDOW_H
