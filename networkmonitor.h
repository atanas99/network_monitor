#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QMainWindow>
#include <QTableWidget>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QLabel>
#include <QCheckBox>
#include <QTimer>
#include <QQueue>
#include <QPair>
#include <QVBoxLayout> // Include for QVBoxLayout


class NetworkMonitor : public QMainWindow {
Q_OBJECT

public:
    NetworkMonitor(QWidget *parent = nullptr);

private slots:

    void updateNetworkInfo();

    void toggleSeriesVisibility();

private:
    QTableWidget *tableWidget;
    QChart *chartData;
    QChart *chartPackets;
    QLineSeries *seriesIn;
    QLineSeries *seriesOut;
    QLineSeries *seriesInPackets;
    QLineSeries *seriesOutPackets;
    QDateTimeAxis *axisXData;
    QDateTimeAxis *axisXPackets;
    QValueAxis *axisYData;
    QValueAxis *axisYPackets;
    QLabel *labelIn;
    QLabel *labelOut;
    QLabel *labelInPackets;
    QLabel *labelOutPackets;
    QCheckBox *checkBoxIn;
    QCheckBox *checkBoxOut;
    QCheckBox *checkBoxInPackets;
    QCheckBox *checkBoxOutPackets;
    QTimer *timer;
    QWidget *dataWidget;
    QWidget *packetsWidget;
    QHBoxLayout *mainLayout;

    qint64 previousInBytes;
    qint64 previousOutBytes;
    qint64 previousInPackets;
    qint64 previousOutPackets;

    // Queues to store the packet data over time
    QQueue<QPair<qint64, qreal>> inPacketsQueue;  // Pair of <time, data MB>
    QQueue<QPair<qint64, qreal>> outPacketsQueue; // Pair of <time, data MB>
    QQueue<QPair<qint64, qreal>> inPacketsCountQueue;  // Pair of <time, packets>
    QQueue<QPair<qint64, qreal>> outPacketsCountQueue; // Pair of <time, packets>
};

#endif // NETWORKMONITOR_H