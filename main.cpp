/*
 * Sango2 1.05+ Editor Copyright (C) 2020 allenzhoukp
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it under certain conditions.
 * See LICENSE for details.
 * */

#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont* font = new QFont;
    font->setFamily("新宋体");
    font->setPointSize(9);
    a.setFont(*font);
    MainWindow w;
    w.show();
    return a.exec();
}
