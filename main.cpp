#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Удаление объектов");
    w.setMaximumSize(QSize(432, 305));
    w.setMinimumSize(QSize(432, 305));
    w.show();
    return a.exec();
}
