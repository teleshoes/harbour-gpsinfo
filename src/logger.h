#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QObject>

class Logger : public QObject {
    Q_OBJECT
public:
    bool ready = false;
    QString logFilePath;
    QFile logFile;
    qint64 logFileSize;

    explicit Logger(QObject *parent = nullptr) : QObject(parent) {}

    ~Logger() {
        stop();
    }

    void setLogFilePath(const QString &filePath) {
        logFilePath = filePath;
    }

    Q_INVOKABLE void start() {
        stop();
        logFile.setFileName(logFilePath);
        ready = true;
        qDebug() << "logging to " << logFilePath;
    }

    Q_INVOKABLE void stop() {
        if (logFile.isOpen()) {
            logFile.close();
        }
        logFileSize = 0;
        ready = false;
    }

    Q_INVOKABLE void log(const QString &msg) {
        if (ready) {
            if (logFile.isOpen()) {
                QFileInfo fileInfo(logFilePath);
                if (!fileInfo.exists() || fileInfo.size() < logFileSize) {
                    qDebug() << "WARNING: log file replaced, reopening";
                    logFile.close();
                    logFileSize = 0;
                }
            }

            if (!logFile.isOpen()) {
                logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
            }

            if (logFile.isOpen()) {
                QDateTime now = QDateTime::currentDateTime();
                QTextStream(&logFile)
                  << "" << now.toMSecsSinceEpoch()
                  << "," << now.toString(Qt::ISODate)
                  << "," << msg
                  << "\n";
                logFile.flush();
                logFileSize = logFile.size();
            }

            qDebug() << "appending log: " << msg;
        }else{
            qDebug() << "not logging: " << msg;
        }
    }
};

#endif // LOGGER_H
