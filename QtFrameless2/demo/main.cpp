#include <QApplication>
#include "mainwindow.h"
#include "widget.h"
#include "dialog.h"
#include <QFile>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif
    QApplication a(argc, argv);

    QString qss;
    QFile qssFile(":/skin/qss/skin.qss");
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }


    //MainWindow w;
    //Widget w;
    Dialog w(true);

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    w.resize(800, 600);
#else
    w.resize(800, 550);
#endif
    w.show();
    return a.exec();
}
