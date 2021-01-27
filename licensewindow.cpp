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
