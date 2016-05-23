#include <QApplication>

#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtmindmap);

    QApplication a(argc, argv);
    QPalette palette = a.palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    a.setPalette(palette);

    MainWindow w;
    w.show();

    return a.exec();
}
