/*
 * Sango2 1.05+ Editor Copyright (C) 2020 allenzhoukp
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it under certain conditions.
 * See LICENSE for details.
 * */

#include "licensewindow.h"
#include "ui_licensewindow.h"

LicenseWindow::LicenseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseWindow)
{
    ui->setupUi(this);
    this->ui->textBrowser->setText(this->licenseText);
}

LicenseWindow::~LicenseWindow()
{
    delete ui;
}
