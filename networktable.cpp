#include "networktable.h"
#include <QNetworkInterface>
#include <QProcess>
#include <QDateTime>
#include <QRegularExpression>
#include <QVBoxLayout>

NetworkTable::NetworkTable(QWidget *parent) : QWidget(parent), previousInBytes(0), previousOutBytes(0),
                                              previousInPackets(0), previousOutPackets(0) {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({
        "Name", "Network", "Address", "Ipkts", "Ibytes", "Opkts", "Obytes"});

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

void NetworkTable::updateNetworkInfo() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    tableWidget->setRowCount(0);

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

    previousInBytes = totalInBytes;
    previousOutBytes = totalOutBytes;
    previousInPackets = totalInPackets;
    previousOutPackets = totalOutPackets;
}

QList<QList<qint64>> NetworkTable::getNetworkData() {
    QList<QList<qint64>> networkData;

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        QList<qint64> rowData;
        for (int j = 0; j < tableWidget->columnCount(); ++j) {
            bool ok;
            qint64 value = tableWidget->item(i, j)->text().toLongLong(&ok);
            if (ok) {
                rowData.append(value);
            } else {
                rowData.append(0);
            }
        }
        networkData.append(rowData);
    }

    return networkData;
}