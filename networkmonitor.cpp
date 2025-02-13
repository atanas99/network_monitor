#include "networkmonitor.h"
#include <QNetworkInterface>
#include <QProcess>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QTimer>
#include <QPen>
#include <QStringList>
#include <QDateTime>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QFont>

NetworkMonitor::NetworkMonitor(QWidget *parent) : QMainWindow(parent), previousInBytes(0), previousOutBytes(0),
                                                  previousInPackets(0), previousOutPackets(0) {
    // Set up the table
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({
        "Name", "Network", "Address", "Ipkts", "Ibytes", "Opkts", "Obytes"
                                           });

    // Set up the charts
    chartData = new QChart();
    chartPackets = new QChart();

    seriesIn = new QLineSeries();
    seriesOut = new QLineSeries();
    seriesInPackets = new QLineSeries();
    seriesOutPackets = new QLineSeries();

    seriesIn->setName("<b>Received Data (MB)</b>");
    seriesOut->setName("<b>Sent Data (MB)</b>");
    seriesInPackets->setName("<b>Received Packets</b>");
    seriesOutPackets->setName("<b>Sent Packets</b>");

    // Set the color of the series
    QPen greenPen(Qt::darkGreen);
    seriesIn->setPen(greenPen);
    QPen bluePen(Qt::darkMagenta);
    seriesOut->setPen(bluePen);
    QPen redPen(Qt::darkRed);
    seriesInPackets->setPen(redPen);
    QPen yellowPen(Qt::darkCyan);
    seriesOutPackets->setPen(yellowPen);

    chartData->addSeries(seriesIn);
    chartData->addSeries(seriesOut);
    chartPackets->addSeries(seriesInPackets);
    chartPackets->addSeries(seriesOutPackets);

    // Set up the axes for each chart
    axisXData = new QDateTimeAxis();
    axisXPackets = new QDateTimeAxis();

    axisXData->setFormat("hh:mm:ss");
    axisXPackets->setFormat("hh:mm:ss");

    axisXData->setTitleText("Time");
    axisXPackets->setTitleText("Time");

    axisXData->setTickCount(11);
    axisXPackets->setTickCount(11);

    axisXData->setGridLineVisible(false);
    axisXPackets->setGridLineVisible(false);

    axisYData = new QValueAxis();
    axisYPackets = new QValueAxis();
    axisYData->setTitleText("Data (MB)");
    axisYPackets->setTitleText("Packets");
    axisYData->setTickCount(9);
    axisYPackets->setTickCount(9);
    axisYData->setGridLineVisible(false);
    axisYPackets->setGridLineVisible(false);

    chartData->addAxis(axisXData, Qt::AlignBottom);
    chartPackets->addAxis(axisXPackets, Qt::AlignBottom);

    chartData->addAxis(axisYData, Qt::AlignLeft);
    chartPackets->addAxis(axisYPackets, Qt::AlignLeft);

    seriesIn->attachAxis(axisXData);
    seriesOut->attachAxis(axisXData);
    seriesInPackets->attachAxis(axisXPackets);
    seriesOutPackets->attachAxis(axisXPackets);

    seriesIn->attachAxis(axisYData);
    seriesOut->attachAxis(axisYData);
    seriesInPackets->attachAxis(axisYPackets);
    seriesOutPackets->attachAxis(axisYPackets);

    // Set the chart background to dark
    QChart *charts[] = {chartData, chartPackets};
    for (QChart *chart: charts) {
        chart->setBackgroundBrush(QBrush(QColor(53, 53, 53)));
        chart->setTitleBrush(QBrush(Qt::white));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor(35, 35, 35)));
        chart->setPlotAreaBackgroundVisible(true);
        chart->legend()->setLabelBrush(QBrush(Qt::white));
    }

    QFont axisFont;
    axisFont.setPointSize(8); // Set the font size to 8 points

    axisXData->setLabelsBrush(QBrush(Qt::white));
    axisXPackets->setLabelsBrush(QBrush(Qt::white));
    axisYData->setLabelsBrush(QBrush(Qt::white));
    axisYPackets->setLabelsBrush(QBrush(Qt::white));
    axisXData->setTitleBrush(QBrush(Qt::white));
    axisXPackets->setTitleBrush(QBrush(Qt::white));
    axisYData->setTitleBrush(QBrush(Qt::white));
    axisYPackets->setTitleBrush(QBrush(Qt::white));

    axisXData->setLabelsFont(axisFont); // Apply the smaller font size to the X axis labels
    axisXPackets->setLabelsFont(axisFont); // Apply the smaller font size to the X axis labels

    // Create the chart views and add them to the layout
    auto *chartViewData = new QChartView(chartData);
    auto *chartViewPackets = new QChartView(chartPackets);

    chartViewData->setRenderHint(QPainter::Antialiasing);
    chartViewPackets->setRenderHint(QPainter::Antialiasing);

    // Set fixed height for the charts
    chartViewData->setFixedHeight(300);
    chartViewPackets->setFixedHeight(300);

    // Set up labels for received and sent data
    labelIn = new QLabel(this);
    labelOut = new QLabel(this);
    labelInPackets = new QLabel(this);
    labelOutPackets = new QLabel(this);
    labelIn->setStyleSheet("QLabel { color : white; }");
    labelOut->setStyleSheet("QLabel { color : white; }");
    labelInPackets->setStyleSheet("QLabel { color : white; }");
    labelOutPackets->setStyleSheet("QLabel { color : white; }");

    // Set up checkboxes for series visibility
    checkBoxIn = new QCheckBox("Received Data", this);
    checkBoxOut = new QCheckBox("Sent Data", this);
    checkBoxInPackets = new QCheckBox("Received Packets", this);
    checkBoxOutPackets = new QCheckBox("Sent Packets", this);
    checkBoxIn->setChecked(true);
    checkBoxOut->setChecked(true);
    checkBoxInPackets->setChecked(true);
    checkBoxOutPackets->setChecked(true);

    connect(checkBoxIn, &QCheckBox::toggled, this, &NetworkMonitor::toggleSeriesVisibility);
    connect(checkBoxOut, &QCheckBox::toggled, this, &NetworkMonitor::toggleSeriesVisibility);
    connect(checkBoxInPackets, &QCheckBox::toggled, this, &NetworkMonitor::toggleSeriesVisibility);
    connect(checkBoxOutPackets, &QCheckBox::toggled, this, &NetworkMonitor::toggleSeriesVisibility);

    auto *checkBoxLayout = new QHBoxLayout();
    checkBoxLayout->addWidget(checkBoxIn);
    checkBoxLayout->addWidget(checkBoxOut);
    checkBoxLayout->addWidget(checkBoxInPackets);
    checkBoxLayout->addWidget(checkBoxOutPackets);

    auto *labelLayout = new QHBoxLayout();
    labelLayout->addWidget(labelIn);
    labelLayout->addWidget(labelOut);
    labelLayout->addWidget(labelInPackets);
    labelLayout->addWidget(labelOutPackets);

    // Create widgets to contain the charts
    dataWidget = new QWidget(this);
    auto *dataLayout = new QVBoxLayout(dataWidget);
    dataLayout->addWidget(chartViewData);

    packetsWidget = new QWidget(this);
    auto *packetsLayout = new QVBoxLayout(packetsWidget);
    packetsLayout->addWidget(chartViewPackets);

    // Set up the overall layout for the central widget
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget); // Use QVBoxLayout to place charts vertically

    auto *chartLayout = new QHBoxLayout(); // Layout for charts side by side
    chartLayout->addWidget(dataWidget);
    chartLayout->addWidget(packetsWidget);
    mainLayout->addLayout(chartLayout); // Add the chart layout to the main layout

    mainLayout->addLayout(checkBoxLayout); // Add the checkboxes below the charts
    mainLayout->addLayout(labelLayout); // Add the labels below the checkboxes
    mainLayout->addWidget(tableWidget); // Add the table below the labels

    setCentralWidget(centralWidget);

    // Timer for periodic updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkMonitor::updateNetworkInfo);
    timer->start(1000);  // Updates every 1 second
    toggleSeriesVisibility();
}

void NetworkMonitor::toggleSeriesVisibility() {
    QList<QWidget *> widgets = {dataWidget, packetsWidget};
    for (QWidget *widget: widgets) {
        widget->setVisible(false);
    }

    bool showData = checkBoxIn->isChecked() || checkBoxOut->isChecked();
    bool showPackets = checkBoxInPackets->isChecked() || checkBoxOutPackets->isChecked ();

    dataWidget->setVisible(showData);
    packetsWidget->setVisible(showPackets);

    seriesIn->setVisible(checkBoxIn->isChecked());
    seriesOut->setVisible(checkBoxOut->isChecked());
    seriesInPackets->setVisible(checkBoxInPackets->isChecked());
    seriesOutPackets->setVisible(checkBoxOutPackets->isChecked());
}

void NetworkMonitor::updateNetworkInfo() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    tableWidget->setRowCount(0); // Clear the table first

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    QProcess process;
    process.start("netstat", {"-ib"});
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QStringList outputLines = output.split('\n');

    qint64 totalInBytes = 0;
    qint64 totalOutBytes = 0;
    qint64 totalInPackets = 0;
    qint64 totalOutPackets = 0;

    for (int i = 1; i < outputLines.size(); ++i) {
        QString line = outputLines[i].trimmed();
        if (!line.isEmpty()) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.size() >= 11) {
                bool okInBytes, okOutBytes, okInPackets, okOutPackets;
                qint64 inBytes = parts[6].toLongLong(&okInBytes);
                qint64 outBytes = parts[9].toLongLong(&okOutBytes);
                qint64 inPackets = parts[4].toLongLong(&okInPackets);
                qint64 outPackets = parts[7].toLongLong(&okOutPackets);

                if (!okInBytes || !okOutBytes || !okInPackets || !okOutPackets) {
                    continue;
                }

                totalInBytes += inBytes;
                totalOutBytes += outBytes;
                totalInPackets += inPackets;
                totalOutPackets += outPackets;

                int row = tableWidget->rowCount();
                tableWidget->insertRow(row);

                tableWidget->setItem(row, 0, new QTableWidgetItem(parts[0]));
                tableWidget->setItem(row, 1, new QTableWidgetItem(parts[2]));
                tableWidget->setItem(row, 2, new QTableWidgetItem(parts[3]));
                tableWidget->setItem(row, 3, new QTableWidgetItem(parts[4]));
                tableWidget->setItem(row, 4, new QTableWidgetItem(parts[6]));
                tableWidget->setItem(row, 5, new QTableWidgetItem(parts[7]));
                tableWidget->setItem(row, 6, new QTableWidgetItem(parts[9]));
            }
        }
    }

    qint64 diffInBytes = totalInBytes - previousInBytes;
    qint64 diffOutBytes = totalOutBytes - previousOutBytes;
    qint64 diffInPackets = totalInPackets - previousInPackets;
    qint64 diffOutPackets = totalOutPackets - previousOutPackets;

    previousInBytes = totalInBytes;
    previousOutBytes = totalOutBytes;
    previousInPackets = totalInPackets;
    previousOutPackets = totalOutPackets;

    // Convert bytes to MB
    qreal diffInMB = static_cast<qreal>(diffInBytes) / 1048576.0;
    qreal diffOutMB = static_cast<qreal>(diffOutBytes) / 1048576.0;

    inPacketsQueue.enqueue(qMakePair(currentTime, diffInMB));
    outPacketsQueue.enqueue(qMakePair(currentTime, diffOutMB));
    inPacketsCountQueue.enqueue(qMakePair(currentTime, static_cast<qreal>(diffInPackets)));
    outPacketsCountQueue.enqueue(qMakePair(currentTime, static_cast<qreal>(diffOutPackets)));

    while (!inPacketsQueue.isEmpty() && inPacketsQueue.head().first < currentTime - 10000) {
        inPacketsQueue.dequeue();
    }
    while (!outPacketsQueue.isEmpty() && outPacketsQueue.head().first < currentTime - 10000) {
        outPacketsQueue.dequeue();
    }
    while (!inPacketsCountQueue.isEmpty() && inPacketsCountQueue.head().first < currentTime - 10000) {
        inPacketsCountQueue.dequeue();
    }
    while (!outPacketsCountQueue.isEmpty() && outPacketsCountQueue.head().first < currentTime - 10000) {
        outPacketsCountQueue.dequeue();
    }

    seriesIn->clear();
    seriesOut->clear();
    seriesInPackets->clear();
    seriesOutPackets->clear();

    qreal totalReceivedMB = 0;
    qreal totalSentMB = 0;
    qreal totalReceivedPackets = 0;
    qreal totalSentPackets = 0;
    for (const auto &dataPoint: inPacketsQueue) {
        seriesIn->append(dataPoint.first, dataPoint.second);
        totalReceivedMB += dataPoint.second;
    }

    for (const auto &dataPoint: outPacketsQueue) {
        seriesOut->append(dataPoint.first, dataPoint.second);
        totalSentMB += dataPoint.second;
    }
    for (const auto &dataPoint: inPacketsCountQueue) {
        seriesInPackets->append(dataPoint.first, dataPoint.second);
        totalReceivedPackets += dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsCountQueue) {
        seriesOutPackets->append(dataPoint.first, dataPoint.second);
        totalSentPackets += dataPoint.second;
    }

    labelIn->setText(QString("<b>Data received in the last 10 seconds:</b> %1 <b>MB</b>").arg(totalReceivedMB, 0, 'f', 2));
    labelOut->setText(QString("<b>Data sent in the last 10 seconds:</b> %1 <b>MB</b>").arg(totalSentMB, 0, 'f', 2));
    labelInPackets->setText(QString("<b>Packets received in the last 10 seconds:</b> %1").arg(totalReceivedPackets, 0, 'f', 0));
    labelOutPackets->setText(QString("<b>Packets sent in the last 10 seconds:</b> %1").arg(totalSentPackets, 0, 'f', 0));

    qreal maxInMB = 0;
    qreal maxOutMB = 0;
    qreal maxInPackets = 0;
    qreal maxOutPackets = 0;
    for (const auto &dataPoint: inPacketsQueue) {
        if (dataPoint.second > maxInMB) maxInMB = dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsQueue) {
        if (dataPoint.second > maxOutMB) maxOutMB = dataPoint.second;
    }
    for (const auto &dataPoint: inPacketsCountQueue) {
        if (dataPoint.second > maxInPackets) maxInPackets = dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsCountQueue) {
        if (dataPoint.second > maxOutPackets) maxOutPackets = dataPoint.second;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    axisXData->setRange(currentDateTime.addSecs(-10), currentDateTime);
    axisXPackets->setRange(currentDateTime.addSecs(-10), currentDateTime);
    axisYData->setRange(0, std::max(maxInMB, maxOutMB) * 1.1);
    axisYPackets->setRange(0, std::max(maxInPackets, maxOutPackets) * 1.1);
}