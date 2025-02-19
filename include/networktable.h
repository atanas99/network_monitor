#ifndef NETWORKTABLE_H
#define NETWORKTABLE_H

#include <QWidget>
#include <QTableWidget>

class NetworkTable : public QWidget {
Q_OBJECT

public:
    explicit NetworkTable(QWidget *parent = nullptr);
    void updateNetworkInfo();
    QList<QList<qint64>> getNetworkData();  // Returns network data for charts

private:
    QTableWidget *tableWidget;
    qint64 previousInBytes;
    qint64 previousOutBytes;
    qint64 previousInPackets;
    qint64 previousOutPackets;
};

#endif // NETWORKTABLE_H