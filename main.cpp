/*
 * Sango2 1.05+ Editor Copyright (C) 2020 allenzhoukp
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it under certain conditions.
 * See LICENSE for details.
 * */

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
