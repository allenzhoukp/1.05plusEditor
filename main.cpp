#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("MS Shell Dlg 1");
    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
