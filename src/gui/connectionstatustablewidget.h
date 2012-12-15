#ifndef CONNECTIONSTATUSTABLEWIDGET_H
#define CONNECTIONSTATUSTABLEWIDGET_H

#include "server/connectionstatus.h"
#include "server/server.h"

#include <QtGui>
#include <QTableWidget>
#include <QTime>

#include <map>

class ConnectionStatusTableWidget : public QTableWidget
{
  Q_OBJECT

public:
  explicit ConnectionStatusTableWidget(QWidget *parent = 0);
  
  void updateTable(const std::vector<unsigned int>& threadIds,
                   std::map<Server::ConnectionId, ConnectionStatus> connectionStatuses);

signals:
  
public slots:

private:

  void createTableHeaders();
  void updateTableRow(int row, unsigned int id);
  
private:
  QTableWidgetItem* m_idItem;
  QTableWidgetItem* m_statusItem;
  QTableWidgetItem* m_totalDownItem;
  QTableWidgetItem* m_totalUpItem;
  QTableWidgetItem* m_downSpeedItem;
  QTableWidgetItem* m_upSpeedItem;

  QTime m_timeStatusUpdate;

  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatuses;
  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatusesOneSecondAgo;
  std::map<Server::ConnectionId, ConnectionStatus> m_connectionStatusesTwoSecondAgo;
};

#endif // CONNECTIONSTATUSTABLEWIDGET_H
