#include "networkmonitor.h"
#include <QVBoxLayout>

NetworkMonitor::NetworkMonitor(QWidget *parent) : QMainWindow(parent) {
    networkTable = new NetworkTable(this);
    networkChart = new NetworkChart(this);

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(networkChart);
    mainLayout->addWidget(networkTable);

    setCentralWidget(centralWidget);

    //Timer for periodic updates update every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkMonitor::updateNetworkInfo);
    timer->start(1000);

    toggleSeriesVisibility();
}

void NetworkMonitor::toggleSeriesVisibility() {
    networkChart->toggleSeriesVisibility();
}

void NetworkMonitor::updateNetworkInfo() {
    networkTable->updateNetworkInfo();
    networkChart->updateNetworkInfo(networkTable->getNetworkData());
}