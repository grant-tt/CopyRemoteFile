#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LOG DIAGNOSE TOOL");
    w.setWindowIcon(QIcon("://img//istar_logo.ico"));
    w.show();
    return a.exec();
}
