#include <QApplication>

#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtmindmap);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
