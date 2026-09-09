#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QObject>

class Logger : public QObject {
    Q_OBJECT
public:
    bool ready = false;
    QFile logFile;
    explicit Logger(QObject *parent = nullptr) : QObject(parent) {}

    void init(const QString &fileName) {
        logFile.setFileName(fileName);
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            ready = true;
        }
    }

    Q_INVOKABLE void log(const QString &msg) {
        if (ready) {
            QDateTime now = QDateTime::currentDateTime();
            QTextStream(&logFile)
              << "" << now.toMSecsSinceEpoch()
              << " : " << now.toString(Qt::ISODate)
              << " : " << msg
              << "\n";
        }
        qDebug() << "C++ received message:" << msg;
    }
};

#endif // LOGGER_H
