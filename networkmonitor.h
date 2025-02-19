#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QMainWindow>
#include <QTimer>
#include "networktable.h"
#include "networkchart.h"

class NetworkMonitor : public QMainWindow {
Q_OBJECT

public:
    NetworkMonitor(QWidget *parent = nullptr);

private slots:

    void updateNetworkInfo();

    void toggleSeriesVisibility();

private:
    NetworkTable *networkTable;
    NetworkChart *networkChart;
    QTimer *timer;
};

#endif // NETWORKMONITOR_H