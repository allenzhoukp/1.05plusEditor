/*
 * Sango2 1.05+ Editor Copyright (C) 2020 allenzhoukp
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it under certain conditions.
 * See LICENSE for details.
 * */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QLineEdit>
#include "contentloader.h"
#include "limitwindow.h"

class LimitWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int load_value_status(const Update& update);
    int load_toggle_status(const QVector<Update>& updates);
    void load_toggle_box(QAbstractButton* box, const QVector<Update>& updates);

    void save_value_status(const QVector<Update>& updates, int value);
    void save_value_status(const QVector<Update>& updates, QString value);
    void save_toggle_status(const QVector<Update>& updates, bool toggled);

private slots:
    void on_selectPathBtn_clicked();

    void on_loadFileBtn_clicked();

    void on_saveFileBtn_clicked();

    void on_a19_chkbox_stateChanged(int arg1);

    void on_a5_chkbox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_btn_forced_override_clicked();

    void on_limit_window_btn_clicked();

private:
    Ui::MainWindow *ui;

    LimitWindow* limitWindow;

    QString filepath;
    char* content;
    int file_length;
    ExeType exe_type;
    ContentLoader contentloader;

    QVector<Update> cheatcodes;

    bool isForcedOverride = false;

    void load_status();

    void forcedOverrideButtonEnabled();

};
#endif // MAINWINDOW_H
