#include <QApplication>
#include <QMessageBox>
#include "networkmonitor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        NetworkMonitor monitor;
        monitor.resize(600, 400);
        monitor.show();
        return app.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", e.what());
        return -1;
    }
}