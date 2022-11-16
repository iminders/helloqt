#include "mainwindow.h"
#include <QApplication>
#include <iostream>

const QString VERSION = APP_VERSION;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if ((!strncmp(argv[1], "--version", 9)))
        {
            std::cout << "std:helloqt " << VERSION.toStdString() << "\n";
            return 0;
        }
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
