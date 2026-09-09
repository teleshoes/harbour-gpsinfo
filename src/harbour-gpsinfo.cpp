#include <QtQuick>

#include <sailfishapp.h>
#include <QDateTime>
#include <QTranslator>
#include "gpsdatasource.h"
#include "gpsinfosettings.h"
#include "logger.h"


int main(int argc, char *argv[]) {
    //migrate old configuration
    QDir configdir = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (configdir.cd("gpsinfo")) {
        configdir.rename("gpsinfo.conf", "harbour-gpsinfo.conf");
        configdir.cdUp();
        configdir.rename("gpsinfo", "harbour-gpsinfo");
    }

    qmlRegisterType<GPSDataSource>("Harbour.GPSInfo", 1, 0, "GPSDataSource");
    qmlRegisterType<GPSSatellite>();
    GPSInfoSettings* settings = new GPSInfoSettings();
    QGuiApplication* qGuiAppl = SailfishApp::application(argc, argv);
    QStringList locales;
    QString baseName("/usr/share/harbour-gpsinfo/translations/");
    QDir localesDir(baseName);
    if (localesDir.exists()) {
        locales = localesDir.entryList(QStringList() << "*.qm", QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
    }
    locales.replaceInStrings(".qm", "");
    QString currentLocale = settings->getLocale();
    qDebug() << "loading language" << currentLocale;
    QTranslator* translator = new QTranslator();
    QString fileName = currentLocale.compare("en") == 0 ? "harbour-gpsinfo.qm" : "harbour-gpsinfo_"+currentLocale+".qm";
    translator->load(fileName, baseName);
    QGuiApplication::installTranslator(translator);

    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir(cacheDir).mkpath(".");
    QString logFile = ""
      + cacheDir
      + "/harbour-gpsinfo-" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".log";
    qDebug() << "logging to " << logFile;

    Logger* logger = new Logger();
    logger->init(logFile);

    QQuickView *view = SailfishApp::createView();
    view->rootContext()->setContextProperty("settings", settings);
    view->rootContext()->setContextProperty("logger", logger);
    view->setSource(SailfishApp::pathTo("qml/harbour-gpsinfo.qml"));
    view->showFullScreen();
    return qGuiAppl->exec();
}
