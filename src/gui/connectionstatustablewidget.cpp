#include "connectionstatustablewidget.h"

ConnectionStatusTableWidget::ConnectionStatusTableWidget(QWidget *parent)
  : QTableWidget(parent)
{
  createTableHeaders();
  m_timeStatusUpdate.start();
}


void
ConnectionStatusTableWidget::createTableHeaders()
{
  QStringList headers;
  headers.append("ID");
  headers.append("Connection Status");
  headers.append("Downloaded");
  headers.append("Uploaded");
  headers.append("Down Speed");
  headers.append("Up Speed");

  setEditTriggers(QAbstractItemView::NoEditTriggers);

  setColumnCount(headers.size());
  setRowCount(0);
  setHorizontalHeaderLabels(headers);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  verticalHeader()->setDefaultSectionSize(24);

  setColumnWidth(0,30);
  setColumnWidth(1,150);
}


void
ConnectionStatusTableWidget::updateTable(const std::vector<unsigned int>& threadIds,
                                         std::map<Server::ConnectionId, ConnectionStatus> connectionStatuses)
{
  m_connectionStatuses = connectionStatuses;

  if (m_timeStatusUpdate.elapsed() > 980)
  {
    std::cout << "ServerWindow::updateTable() - elapsed: " << m_timeStatusUpdate.elapsed() << " ms" << std::endl;
    m_timeStatusUpdate.restart();
    m_connectionStatusesTwoSecondAgo = m_connectionStatusesOneSecondAgo;
    m_connectionStatusesOneSecondAgo = m_connectionStatuses;
  }

  setRowCount(threadIds.size());

  for (unsigned int i = 0; i < threadIds.size(); ++i)
  {
    updateTableRow(i, threadIds[i]);
  }
}


void
ConnectionStatusTableWidget::updateTableRow(int row, unsigned int id)
{
  double totalDown_MiB = 0;
  double totalUp_MiB = 0;

  double downSpeed_B = 0;
  double upSpeed_B = 0;

  double downSpeed_KiB = 0;
  double upSpeed_KiB = 0;

  double downSpeed_MiB = 0;
  double upSpeed_MiB = 0;

  ConnectionStatus::EnumConnectionStatus status = ConnectionStatus::unavailable;

  if (m_connectionStatuses.find(id) != m_connectionStatuses.end())
  {
    status = m_connectionStatuses.at(id).generalStatus;
  }

  if (m_connectionStatuses.find(id) != m_connectionStatuses.end()
      && m_connectionStatusesOneSecondAgo.find(id) != m_connectionStatusesOneSecondAgo.end()
      && m_connectionStatusesTwoSecondAgo.find(id) != m_connectionStatusesTwoSecondAgo.end())
  {
    totalDown_MiB = m_connectionStatuses.at(id).totalDataDown_byte / 1024.0 / 1024.0;
    totalUp_MiB = m_connectionStatuses.at(id).totalDataUp_byte / 1024.0 / 1024.0;

    downSpeed_B = (m_connectionStatusesOneSecondAgo.at(id).totalDataDown_byte
                   - m_connectionStatusesTwoSecondAgo.at(id).totalDataDown_byte);
    downSpeed_KiB = downSpeed_B / 1024.0;
    downSpeed_MiB = downSpeed_KiB / 1024.0;

    upSpeed_B = (m_connectionStatusesOneSecondAgo.at(id).totalDataUp_byte
                 - m_connectionStatusesTwoSecondAgo.at(id).totalDataUp_byte);
    upSpeed_KiB = upSpeed_B / 1024.0;
    upSpeed_MiB = upSpeed_KiB / 1024.0;
  }

  std::string statusString = "unknown";
  if (status == ConnectionStatus::connected)
  {
    statusString = "connected";
  }
  else if (status == ConnectionStatus::listening)
  {
    statusString = "listening";
  }
  else if (status == ConnectionStatus::unavailable)
  {
    statusString = "unavailable";
  }

  m_idItem = new QTableWidgetItem( QString::number(id) );
  m_statusItem = new QTableWidgetItem( QString::fromStdString(statusString) );
  m_totalDownItem = new QTableWidgetItem( QString::number(totalDown_MiB, char('f'), 2) + " MiB");
  m_totalUpItem = new QTableWidgetItem( QString::number(totalUp_MiB, char('f'), 2) + " MiB");

  if (downSpeed_B > 1024*1024)
  {
    m_downSpeedItem = new QTableWidgetItem( QString::number(downSpeed_MiB, char('f'), 1) + " MiB/s");
  }
  else if (downSpeed_B > 1024)
  {
    m_downSpeedItem = new QTableWidgetItem( QString::number(downSpeed_KiB, char('f'), 1) + " KiB/s");
  }
  else
  {
    m_downSpeedItem = new QTableWidgetItem( QString::number(downSpeed_B, char('f'), 1) + " B/s");
  }

  if (upSpeed_B > 1024*1024)
  {
    m_upSpeedItem = new QTableWidgetItem( QString::number(upSpeed_MiB, char('f'), 1) + " MiB/s");
  }
  else if (upSpeed_B > 1024)
  {
    m_upSpeedItem = new QTableWidgetItem( QString::number(upSpeed_KiB, char('f'), 1) + " KiB/s");
  }
  else
  {
    m_upSpeedItem = new QTableWidgetItem( QString::number(upSpeed_B, char('f'), 1) + " B/s");
  }

  setItem(row, 0, m_idItem);
  setItem(row, 1, m_statusItem);
  setItem(row, 2, m_totalDownItem);
  setItem(row, 3, m_totalUpItem);
  setItem(row, 4, m_downSpeedItem);
  setItem(row, 5, m_upSpeedItem);
}
