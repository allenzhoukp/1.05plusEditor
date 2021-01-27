#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contentloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectPathBtn_clicked();

    void on_loadFileBtn_clicked();

    void on_saveFileBtn_clicked();

    void on_a19_chkbox_stateChanged(int arg1);

    void on_a5_chkbox_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QString filepath;
    char* content;
    int file_length;
    ExeType exe_type;
    ContentLoader contentloader;

    QVector<Update> cheatcodes;

    int load_value_status(const Update& update);
    bool load_toggle_status(const QVector<Update>& updates);
    void load_status();

    void save_value_status(const QVector<Update>& updates, int value);
    void save_value_status(const QVector<Update>& updates, QString value);
    void save_toggle_status(const QVector<Update>& updates, bool toggled);

};
#endif // MAINWINDOW_H
