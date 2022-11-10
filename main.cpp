#include "mainwindow.h"

#include <QApplication>

const QString VERSION = APP_VERSION;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if ((!strncmp(argv[1], "--version", 9)))
        {
            qInfo() << QString("helloqt ") << VERSION;
            return 0;
        }
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
