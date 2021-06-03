/*
 * Sango2 1.05+ Editor Copyright (C) 2020 allenzhoukp
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it under certain conditions.
 * See LICENSE for details.
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "licensewindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    content = nullptr;
    file_length = 0;
    filepath= "/";

    this->limitWindow = new LimitWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_selectPathBtn_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("选择路径"),
                                                     this->filepath,
                                                     "三国群英传2主程序(*.exe)",
                                                     0);
    if (!file_name.isNull()) {
        this->filepath = file_name;
        this->ui->inputFileBox->setText(file_name);
        this->on_loadFileBtn_clicked();
    }
}

void MainWindow::on_loadFileBtn_clicked()
{
    // init: set save btn to disabled;
    this->ui->saveFileBtn->setEnabled(false);

    QByteArray bt = this->filepath.toLocal8Bit();
    char* file_name = bt.data();

    bool load_success = true;
    FILE* file = fopen(file_name, "rb");
    if (file == nullptr)
        load_success = false;
    else {
        if (this->content == nullptr)
            this->content = new char[10485760];

        this->file_length = fread(this->content, 1, 10485760, file);
        if (this->file_length <= 0) {
            this->file_length = 0;
            load_success = false;
        }
        fclose(file);
    }
    if (!load_success) {
        QMessageBox::critical(NULL, "错误", "文件打开失败",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // exe type from the timestamp in the PE Header
    int pe_header_offset = *(int *)(this->content + 0x3C);
    int timestamp = *(int *)(this->content + pe_header_offset + 8);
    QString exe_type_str;

    switch(timestamp) {
    case 0x370C560C:
        exe_type_str = "1.05简体";
        exe_type = ExeType::simp105;
        break;
    case 0x370C51E3:
        exe_type_str = "1.05繁体（暂不支持）";
        exe_type = ExeType::trad105;
        break;
    case 0x4FA37C19:
        exe_type_str = "Steam 1.05简体";
        exe_type = ExeType::steam_simp105;
        break;
    case 0x58182261:
        exe_type_str = "Steam 1.05繁体";
        exe_type = ExeType::steam_trad105;
        break;
    case 0x36B7FEB7:
        exe_type_str = "1.0简体（暂不支持）";
        exe_type = ExeType::simp10;
        break;
    default:
        exe_type_str = "其它EXE（暂不支持）";
        exe_type = ExeType::others;
    }

    if (exe_type == ExeType::steam_trad105) {
        if (this->content[0x2A0] == '.') {
            exe_type_str = "Steam 繁体1.05加壳（暂不支持）";
            exe_type = ExeType::steam_simpshelled;
        }
    } else if (exe_type == ExeType::steam_simp105) {
        if (this->content[0x2B0] == '.') {
            exe_type_str = "Steam 简体1.05加壳（暂不支持）";
            exe_type = ExeType::steam_tradshelled;
        }
    }

    // show the exe type
    this->ui->exeTypeLabel->setText("EXE类型：" + exe_type_str);

    switch (exe_type){
    case ExeType::steam_simpshelled:
    case ExeType::steam_tradshelled:
        QMessageBox::critical(NULL, "错误", "由于奥汀官方对EXE进行了加壳，本修改器不对EXE进行脱壳处理。"
                                          "请使用我们提供的Steam非官方更新档覆盖原EXE后，再使用本修改器进行修改。",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    case ExeType::trad105:
    case ExeType::simp10:
    case ExeType::others:
        QMessageBox::critical(NULL, "错误", "暂未支持的EXE类型：" + exe_type_str,
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    default:
        break;
    }

    // Now the file is saveable.
    this->ui->saveFileBtn->setEnabled(true);

    // disable some options according to type
    switch (exe_type){
    case ExeType::simp105:

        this->ui->b9_chkbox->setEnabled(true);
        this->ui->b13_chkbox->setEnabled(true);
        this->ui->a2_chkbox->setEnabled(true);

        this->ui->a3_togg_1->setEnabled(true);
        this->ui->a3_togg_2->setEnabled(true);
        this->ui->a3_togg_3->setEnabled(true);

        this->ui->a6_chkbox->setEnabled(true);
        this->ui->a7_chkbox->setEnabled(true);
        this->ui->a8_chkbox->setEnabled(true);
        this->ui->a9_chkbox->setEnabled(true);
        this->ui->a10_chkbox->setEnabled(true);
        this->ui->a11_chkbox->setEnabled(true);
        this->ui->a12_chkbox->setEnabled(true);
        this->ui->a13_chkbox->setEnabled(true);
        this->ui->a14_chkbox->setEnabled(true);
        this->ui->a15_chkbox->setEnabled(true);
        this->ui->a19_chkbox->setEnabled(true);
        this->ui->a19_label->setEnabled(true);
        this->ui->a20_chkbox->setEnabled(true);
        this->ui->a21_chkbox->setEnabled(true);
        this->ui->a22_chkbox->setEnabled(true);
        this->ui->a23_chkbox->setEnabled(true);
        this->ui->a24_chkbox->setEnabled(true);
        // this->ui->limit_window_btn->setEnabled(true);
        break;

    case ExeType::steam_simp105:
    case ExeType::steam_trad105:
        this->ui->b9_chkbox->setChecked(false);
        this->ui->b9_chkbox->setEnabled(false);
        this->ui->b13_chkbox->setChecked(false);
        this->ui->b13_chkbox->setEnabled(false);
        this->ui->a2_chkbox->setChecked(false);
        this->ui->a2_chkbox->setEnabled(false);

        this->ui->a3_togg_1->setEnabled(false);
        this->ui->a3_togg_2->setEnabled(false);
        this->ui->a3_togg_3->setEnabled(false);
        this->ui->a3_togg_1->setChecked(true);
        this->ui->a3_togg_2->setChecked(false);
        this->ui->a3_togg_3->setChecked(false);

        this->ui->a6_chkbox->setChecked(false);
        this->ui->a6_chkbox->setEnabled(false);
        this->ui->a7_chkbox->setChecked(false);
        this->ui->a7_chkbox->setEnabled(false);
        this->ui->a8_chkbox->setChecked(false);
        this->ui->a8_chkbox->setEnabled(false);
        this->ui->a9_chkbox->setChecked(false);
        this->ui->a9_chkbox->setEnabled(false);
        this->ui->a10_chkbox->setChecked(false);
        this->ui->a10_chkbox->setEnabled(false);
        this->ui->a11_chkbox->setChecked(false);
        this->ui->a11_chkbox->setEnabled(false);
        this->ui->a12_chkbox->setChecked(false);
        this->ui->a12_chkbox->setEnabled(false);
        this->ui->a13_chkbox->setChecked(false);
        this->ui->a13_chkbox->setEnabled(false);
        this->ui->a14_chkbox->setChecked(false);
        this->ui->a14_chkbox->setEnabled(false);
        this->ui->a15_chkbox->setChecked(false);
        this->ui->a15_chkbox->setEnabled(false);
        this->ui->a19_chkbox->setChecked(false);
        this->ui->a19_chkbox->setEnabled(false);
        this->ui->a19_label->setEnabled(false);
        this->ui->a20_chkbox->setChecked(false);
        this->ui->a20_chkbox->setEnabled(false);
        this->ui->a21_chkbox->setChecked(false);
        this->ui->a21_chkbox->setEnabled(false);
        this->ui->a22_chkbox->setChecked(false);
        this->ui->a22_chkbox->setEnabled(false);
        this->ui->a23_chkbox->setChecked(false);
        this->ui->a23_chkbox->setEnabled(false);
        this->ui->a24_chkbox->setChecked(false);
        this->ui->a24_chkbox->setEnabled(false);
        this->ui->limit_window_btn->setEnabled(false);
        break;
    }

    // load all status
    this->load_status();

    // enable forced override button
    this->ui->btn_forced_override->setEnabled(true);
}

int MainWindow::load_value_status(const Update& update) {
    int address = update.address;
    int length = update.length;
    int value;
    if (length == 1)
        value = *(char *)(content + address);
    else if (length == 2)
        value = *(short *)(content + address);
    else if (length == 4)
        value = *(int *)(content + address);
    else value = -1;

    if (update.value_added > 0)
        value -= update.value_added;

    return value;
}

int MainWindow::load_toggle_status(const QVector<Update>& updates) {
    int outcome = 1;
    for (Update update : updates) {
        if (!(update.isCheatCode)) {
            int address = update.address;
            QVector<unsigned char> bytes = update.bytes;
            for (int i = 0; i < bytes.size(); i++) {
                if (bytes[i] != *(unsigned char *)(content + address + i))
                    outcome = -1;
            }
        }
    }
    if (outcome == 1) return outcome;
    for (Update update : updates) {
        if (!(update.isCheatCode)) {
            int address = update.address;
            QVector<unsigned char> bytes = update.origins;
            for (int i = 0; i < bytes.size(); i++) {
                if (bytes[i] != *(unsigned char *)(content + address + i))
                    outcome = 0;
            }
        }
    }

    return outcome;
}

void MainWindow::load_toggle_box(QAbstractButton* box, const QVector<Update>& updates) {
    int outcome = load_toggle_status(updates);
    if (outcome == 0)
        box->setEnabled(false);
    else
        box->setEnabled(true);
    if (outcome == 1)
        box->setChecked(true);
    else
        box->setChecked(false);
}

void MainWindow::load_status() {
    for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
        QString key = it.key();
        if (!it.value().content.contains(this->exe_type))
            continue;
        QVector<Update> updates = it.value().content[this->exe_type];
        Update firstUpdate = updates[0];

        if (key == "b1_1_box") {
            this->ui->b1_1_box->setText(QString::number(load_value_status(firstUpdate)));
        } else if (key == "b1_2_box") {
            this->ui->b1_2_box->setText(QString::number(load_value_status(firstUpdate)));

        } else if (key == "b2_1_box") {
            this->ui->b2_1_box->setText(QString::number(load_value_status(firstUpdate)));
        } else if (key == "b2_2_box") {
            this->ui->b2_2_box->setText(QString::number(load_value_status(firstUpdate)));
        } else if (key == "b2_3_box") {
            this->ui->b2_3_box->setText(QString::number(load_value_status(firstUpdate)));
        } else if (key == "b2_4_box") {
            this->ui->b2_4_box->setText(QString::number(load_value_status(firstUpdate)));

        } else if (key == "b4_chkbox") {
            if (this->ui->b4_chkbox->isEnabled())
                load_toggle_box(this->ui->b4_chkbox, updates);

        } else if (key == "b5_chkbox") {
            if (this->ui->b5_chkbox->isEnabled())
                load_toggle_box(this->ui->b5_chkbox, updates);

        } else if (key == "b7_chkbox") {
            if (this->ui->b7_chkbox->isEnabled())
                load_toggle_box(this->ui->b7_chkbox, updates);

        } else if (key == "b8_chkbox") {
            if (this->ui->b8_chkbox->isEnabled())
                load_toggle_box(this->ui->b8_chkbox, updates);

        } else if (key == "b9_chkbox") {
            if (this->ui->b9_chkbox->isEnabled())
                load_toggle_box(this->ui->b9_chkbox, updates);

        } else if (key == "b10_chkbox") {
            if (this->ui->b10_chkbox->isEnabled())
                load_toggle_box(this->ui->b10_chkbox, updates);

        } else if (key == "b12_chkbox") {
            if (this->ui->b12_chkbox->isEnabled())
                load_toggle_box(this->ui->b12_chkbox, updates);

        } else if (key == "b13_chkbox") {
            if (this->ui->b13_chkbox->isEnabled())
                load_toggle_box(this->ui->b13_chkbox, updates);

        } else if (key == "b16_chkbox") {
            if (this->ui->b16_chkbox->isEnabled())
                load_toggle_box(this->ui->b16_chkbox, updates);

        } else if (key == "a2_chkbox") {
            if (this->ui->a2_chkbox->isEnabled())
                load_toggle_box(this->ui->a2_chkbox, updates);

        } else if (key == "a3_chkbox") {
            if (this->ui->a3_chkbox->isEnabled())
                load_toggle_box(this->ui->a3_chkbox, updates);

        } else if (key == "a3_togg_2") {
            if (this->ui->a3_togg_2->isEnabled())
                load_toggle_box(this->ui->a3_togg_2, updates);

        } else if (key == "a3_togg_3") {
            if (this->ui->a3_togg_3->isEnabled())
                load_toggle_box(this->ui->a3_togg_3, updates);

        } else if (key == "a4_box") {
            this->ui->a4_box->setText(QString::number(load_value_status(firstUpdate)));

        } else if (key == "a5_1_box") {
            this->ui->a5_1_box->setText(QString::number(load_value_status(firstUpdate)));

        } else if (key == "a5_2_box") {
            this->ui->a5_2_box->setText(QString::number(load_value_status(firstUpdate)));

        } else if (key == "a5_chkbox") {
            if (this->ui->a5_chkbox->isEnabled())
                load_toggle_box(this->ui->a5_chkbox, updates);

        } else if (key == "a6_chkbox") {
            if (this->ui->a6_chkbox->isEnabled())
                load_toggle_box(this->ui->a6_chkbox, updates);

        } else if (key == "a7_chkbox") {
            if (this->ui->a7_chkbox->isEnabled())
                load_toggle_box(this->ui->a7_chkbox, updates);

        } else if (key == "a8_chkbox") {
            if (this->ui->a8_chkbox->isEnabled())
                load_toggle_box(this->ui->a8_chkbox, updates);

        } else if (key == "a9_chkbox") {
            if (this->ui->a9_chkbox->isEnabled())
                load_toggle_box(this->ui->a9_chkbox, updates);

        } else if (key == "a10_chkbox") {
            if (this->ui->a10_chkbox->isEnabled())
                load_toggle_box(this->ui->a10_chkbox, updates);

        } else if (key == "a11_chkbox") {
            if (this->ui->a11_chkbox->isEnabled())
                load_toggle_box(this->ui->a11_chkbox, updates);

        } else if (key == "a12_chkbox") {
            if (this->ui->a12_chkbox->isEnabled())
                load_toggle_box(this->ui->a12_chkbox, updates);

        } else if (key == "a13_chkbox") {
            if (this->ui->a13_chkbox->isEnabled())
                load_toggle_box(this->ui->a13_chkbox, updates);

        } else if (key == "a14_chkbox") {
            if (this->ui->a14_chkbox->isEnabled())
                load_toggle_box(this->ui->a14_chkbox, updates);

        } else if (key == "a15_chkbox") {
            if (this->ui->a15_chkbox->isEnabled())
                load_toggle_box(this->ui->a15_chkbox, updates);

        } else if (key == "a19_chkbox") {
            if (this->ui->a19_chkbox->isEnabled()){
                load_toggle_box(this->ui->a19_chkbox, updates);

                if (this->ui->a19_chkbox->isChecked() && this->ui->a19_chkbox->isEnabled()) {
                    char offset = *(this->content + 0x3157F);
                    this->ui->a19_box->setText(QString::number(offset, 16));
                }
            }
            this->ui->a19_box->setEnabled(this->ui->a19_chkbox->isEnabled());

        } else if (key == "a20_chkbox") {
            if (this->ui->a20_chkbox->isEnabled())
                load_toggle_box(this->ui->a20_chkbox, updates);

        } else if (key == "a21_chkbox") {
            if (this->ui->a21_chkbox->isEnabled())
                load_toggle_box(this->ui->a21_chkbox, updates);

        } else if (key == "a22_chkbox") {
            if (this->ui->a22_chkbox->isEnabled())
                load_toggle_box(this->ui->a22_chkbox, updates);

        } else if (key == "a23_chkbox") {
            if (this->ui->a23_chkbox->isEnabled())
                load_toggle_box(this->ui->a23_chkbox, updates);

        } else if (key == "a24_chkbox") {
            if (this->ui->a24_chkbox->isEnabled())
                load_toggle_box(this->ui->a24_chkbox, updates);
        }
    }
    if (this->ui->a3_togg_1->isChecked() || this->ui->a3_togg_2->isChecked() || this->ui->a3_togg_3->isChecked()) {
        this->ui->a3_togg_1->setEnabled(true);
        this->ui->a3_togg_2->setEnabled(true);
        this->ui->a3_togg_3->setEnabled(true);
    }
    if (this->ui->a3_togg_2->isEnabled() && this->ui->a3_togg_3->isEnabled() &&
        !this->ui->a3_togg_2->isChecked() && !this->ui->a3_togg_3->isChecked()) {
        this->ui->a3_togg_1->setEnabled(true);
        this->ui->a3_togg_1->setChecked(true);
    }

    if (this->ui->limit_window_btn->isEnabled()) {
        this->limitWindow->loadContent(this->content);
        this->limitWindow->onStatusLoad(this, contentloader, this->exe_type);
    }
}

void MainWindow::save_value_status(const QVector<Update>& updates, int value){
    for (Update update : updates) {
        int address = update.address;
        int length = update.length;
        int neu = value;
        if (update.value_added > 0)
            neu += update.value_added;
        if (length == 1) {
            if (neu > 0x7F)
                throw QString("超出上限：最高127");
            *(char *)(this->content + address) = (char) neu;
        } else if (length == 2) {
            if (neu > 0x7FFF)
                throw QString("超出上限：最高32767");
            *(short *)(this->content + address) = (short) neu;
        } else if (length == 4)
            *(int *)(this->content + address) = neu;
    }
}

void MainWindow::save_value_status(const QVector<Update>& updates, QString value){
    bool ok;
    int intval = value.toInt(&ok);
    if (!ok)
        throw QString("不是合法的数字");
    save_value_status(updates, intval);
}

void MainWindow::save_toggle_status(const QVector<Update>& updates, bool toggled){
    for (Update update : updates) {
        int address = update.address;
        if (update.isCheatCode) {
            if (toggled)
                cheatcodes.push_back(update);
        } else {
            for (int i = 0; i < update.bytes.size(); i++) {
                if (toggled)
                    *(unsigned char *)(this->content + address + i) = update.bytes[i];
                else if (!isForcedOverride)
                    *(unsigned char *)(this->content + address + i) = update.origins[i];
            }
        }
    }
}

void MainWindow::on_saveFileBtn_clicked()
{
    // write back-up file
    QByteArray bt_bak = (this->filepath.left(this->filepath.length() - 4) + ".bak.exe").toLocal8Bit();
    char* file_name_bak = bt_bak.data();
    FILE* bak = fopen(file_name_bak, "wb");
    if (bak != nullptr) {
        fwrite(this->content, 1, this->file_length, bak);
        fclose(bak);
    } else {
        QMessageBox::information(NULL, "注意", "备份文件创建失败。",
                                 QMessageBox::Ok, QMessageBox::Ok);
    }

    try {
        for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
            QString key = it.key();
            if (!it.value().content.contains(this->exe_type))
                continue;
            QVector<Update> updates = it.value().content[this->exe_type];

            if (key == "b1_1_box" && this->ui->b1_1_box->isEnabled()) {
                save_value_status(updates, this->ui->b1_1_box->text());
            } else if (key == "b1_2_box" && this->ui->b1_2_box->isEnabled()) {
                save_value_status(updates, this->ui->b1_2_box->text());

            } else if (key == "b2_1_box" && this->ui->b2_1_box->isEnabled()) {
                save_value_status(updates, this->ui->b2_1_box->text());
            } else if (key == "b2_2_box" && this->ui->b2_2_box->isEnabled()) {
                save_value_status(updates, this->ui->b2_2_box->text());
            } else if (key == "b2_3_box" && this->ui->b2_3_box->isEnabled()) {
                save_value_status(updates, this->ui->b2_3_box->text());
            } else if (key == "b2_4_box" && this->ui->b2_4_box->isEnabled()) {
                save_value_status(updates, this->ui->b2_4_box->text());

            } else if (key == "b4_chkbox" && this->ui->b4_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b4_chkbox->isChecked());

            } else if (key == "b5_chkbox" && this->ui->b5_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b5_chkbox->isChecked());

            } else if (key == "b7_chkbox" && this->ui->b7_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b7_chkbox->isChecked());

            } else if (key == "b8_chkbox" && this->ui->b8_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b8_chkbox->isChecked());

            } else if (key == "b9_chkbox" && this->ui->b9_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b9_chkbox->isChecked());

            } else if (key == "b10_chkbox" && this->ui->b10_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b10_chkbox->isChecked());

            } else if (key == "b12_chkbox" && this->ui->b12_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b12_chkbox->isChecked());

            } else if (key == "b13_chkbox" && this->ui->b13_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b13_chkbox->isChecked());

            } else if (key == "b16_chkbox" && this->ui->b16_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->b16_chkbox->isChecked());

            } else if (key == "a2_chkbox" && this->ui->a2_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a2_chkbox->isChecked());

            } else if (key == "a3_chkbox" && this->ui->a3_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a3_chkbox->isChecked());
            } else if (key == "a3_togg_2" && !this->ui->a3_togg_3->isChecked() && this->ui->a3_togg_2->isEnabled()) {
                save_toggle_status(updates, this->ui->a3_togg_2->isChecked());
            } else if (key == "a3_togg_3" && !this->ui->a3_togg_2->isChecked() && this->ui->a3_togg_3->isEnabled()) {
                save_toggle_status(updates, this->ui->a3_togg_3->isChecked());

            } else if (key == "a4_box" && this->ui->a4_box->isEnabled()) {
                bool ok;
                int value = this->ui->a4_box->text().toInt(&ok);
                if (!ok)
                    throw QString("不是合法的数字");
                if (value > 0x7F)
                    throw QString("超出上限：最高127");
                for (int i = 0; i < updates.size(); i++) {
                    Update update = updates[i];
                    int address = update.address;
                    int length = update.length;
                    if (i == 2) value--;
                    if (length == 1)
                        *(char *)(this->content + address) = (char) value;
                    else if (length == 2)
                        *(short *)(this->content + address) = (short) value;
                    else if (length == 4)
                        *(int *)(this->content + address) = value;
                }

            } else if (key == "a5_1_box" && this->ui->a5_1_box->isEnabled()) {
                save_value_status(updates, this->ui->a5_1_box->text().toInt());

            } else if (key == "a5_2_box" && this->ui->a5_2_box->isEnabled()) {
                save_value_status(updates, this->ui->a5_2_box->text().toInt());

            } else if (key == "a5_chkbox" && this->ui->a5_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a5_chkbox->isChecked());

            } else if (key == "a6_chkbox" && this->ui->a6_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a6_chkbox->isChecked());

            } else if (key == "a7_chkbox" && this->ui->a7_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a7_chkbox->isChecked());

            } else if (key == "a8_chkbox" && this->ui->a8_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a8_chkbox->isChecked());

            } else if (key == "a9_chkbox" && this->ui->a9_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a9_chkbox->isChecked());

            } else if (key == "a10_chkbox" && this->ui->a10_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a10_chkbox->isChecked());

            } else if (key == "a11_chkbox" && this->ui->a11_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a11_chkbox->isChecked());

            } else if (key == "a12_chkbox" && this->ui->a12_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a12_chkbox->isChecked());

            } else if (key == "a13_chkbox" && this->ui->a13_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a13_chkbox->isChecked());

            } else if (key == "a14_chkbox" && this->ui->a14_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a14_chkbox->isChecked());

            } else if (key == "a15_chkbox" && this->ui->a15_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a15_chkbox->isChecked());

            } else if (key == "a19_chkbox" && this->ui->a19_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a19_chkbox->isChecked());

                if (this->ui->a19_chkbox->isChecked()) {
                    bool ok;
                    int offset = this->ui->a19_box->text().toInt(&ok, 16);
                    if (!ok)
                        throw QString("不是合法的数字");
                    if (offset > 127)
                        throw QString("A17 - 数字超过上限：最大0x7F");
                    *(this->content + 0x3157F) = (char) offset;
                    *(this->content + 0x32EB2) = (char) offset;
                }


            } else if (key == "a20_chkbox" && this->ui->a20_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a20_chkbox->isChecked());

            } else if (key == "a21_chkbox" && this->ui->a21_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a21_chkbox->isChecked());

            } else if (key == "a22_chkbox" && this->ui->a22_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a22_chkbox->isChecked());

            } else if (key == "a23_chkbox" && this->ui->a23_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a23_chkbox->isChecked());

            } else if (key == "a24_chkbox" && this->ui->a24_chkbox->isEnabled()) {
                save_toggle_status(updates, this->ui->a24_chkbox->isChecked());

            }
        }

        if (this->ui->limit_window_btn->isEnabled())
            this->limitWindow->onStatusWrite(this, contentloader, exe_type);

    } catch (QString err) {
        QByteArray errbytes = err.toUtf8();
        QMessageBox::critical(NULL, "错误", errbytes.data(),
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // write cheatcodes
    int cheatcode_start;
    switch(this->exe_type) {
    case ExeType::simp105:
        cheatcode_start = 0x841F8;
        break;
    case ExeType::steam_simp105:
    case ExeType::steam_trad105:
        cheatcode_start = 0x86008;
        break;
    }

    while (*(int*)(this->content + cheatcode_start)) {
        int current_cheatfunc = *(int*)(this->content + cheatcode_start + 0xC);
        for (int i = 0; i < cheatcodes.size(); i++) {
            int dest = (cheatcodes[i].bytes[0xC] | (cheatcodes[i].bytes[0xD] << 8) |
                        (cheatcodes[i].bytes[0xE] << 16) | (cheatcodes[i].bytes[0xF] << 24));
            if (current_cheatfunc == dest)
                cheatcodes[i].isCheatCode = false;
        }
        cheatcode_start += 16;
    }

    for (auto it = cheatcodes.begin(); it != cheatcodes.end(); ++it) {
        if (it->isCheatCode) {
            for (int i = 0; i < 16; i++) {
                *(unsigned *)(this->content + cheatcode_start + i) = it->bytes[i];
            }
            cheatcode_start += 16;
        }
    }
    // Cheatcodes are collected everytime save btn is clicked.
    // We need to clear it after all the jobs are done.
    cheatcodes.clear();


    // write new file
    QByteArray bt = this->filepath.toLocal8Bit();
    char* file_name = bt.data();
    FILE* foutput = fopen(file_name, "wb");
    if (foutput != nullptr) {
        fwrite(this->content, 1, this->file_length, foutput);
        fclose(foutput);
    } else {
        QMessageBox::critical(NULL, "错误", "文件保存失败。",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QMessageBox::information(NULL, "信息", "文件保存成功。",
                             QMessageBox::Ok, QMessageBox::Ok);

    if (isForcedOverride)
        forcedOverrideButtonEnabled();
}

void MainWindow::on_a5_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->a5_1_box->setEnabled(false);
        this->ui->a5_2_box->setEnabled(false);
    } else {
        this->ui->a5_1_box->setEnabled(true);
        this->ui->a5_2_box->setEnabled(true);
    }
}

void MainWindow::on_a19_chkbox_stateChanged(int arg1)
{
    if (arg1)
        this->ui->a19_box->setEnabled(true);
    else
        this->ui->a19_box->setEnabled(false);

}

void MainWindow::on_pushButton_clicked()
{
    LicenseWindow* lw = new LicenseWindow();
    lw->show();
}

void MainWindow::forcedOverrideButtonEnabled() {
    QVector<QAbstractButton*> togglelist;
    togglelist.push_back(this->ui->b4_chkbox);
    togglelist.push_back(this->ui->b5_chkbox);
    togglelist.push_back(this->ui->b7_chkbox);
    togglelist.push_back(this->ui->b8_chkbox);
    togglelist.push_back(this->ui->b9_chkbox);
    togglelist.push_back(this->ui->b10_chkbox);
    togglelist.push_back(this->ui->b12_chkbox);
    togglelist.push_back(this->ui->b13_chkbox);
    togglelist.push_back(this->ui->b16_chkbox);
    togglelist.push_back(this->ui->a2_chkbox);
    togglelist.push_back(this->ui->a3_chkbox);
    togglelist.push_back(this->ui->a3_togg_1);
    togglelist.push_back(this->ui->a3_togg_2);
    togglelist.push_back(this->ui->a3_togg_2);
    togglelist.push_back(this->ui->a5_chkbox);
    togglelist.push_back(this->ui->a6_chkbox);
    togglelist.push_back(this->ui->a7_chkbox);
    togglelist.push_back(this->ui->a8_chkbox);
    togglelist.push_back(this->ui->a9_chkbox);
    togglelist.push_back(this->ui->a10_chkbox);
    togglelist.push_back(this->ui->a11_chkbox);
    togglelist.push_back(this->ui->a12_chkbox);
    togglelist.push_back(this->ui->a13_chkbox);
    togglelist.push_back(this->ui->a14_chkbox);
    togglelist.push_back(this->ui->a15_chkbox);
    togglelist.push_back(this->ui->a19_chkbox);
    togglelist.push_back(this->ui->a20_chkbox);
    togglelist.push_back(this->ui->a21_chkbox);
    togglelist.push_back(this->ui->a22_chkbox);
    togglelist.push_back(this->ui->a23_chkbox);
    togglelist.push_back(this->ui->a24_chkbox);

    for (auto btn : togglelist) {
        if (btn->isChecked())
            btn->setEnabled(false);
        else
            btn->setEnabled(true);
    }

    // Special for togglebox
    if (this->ui->a3_togg_1->isEnabled() || this->ui->a3_togg_2->isEnabled() || this->ui->a3_togg_3->isEnabled()) {
        this->ui->a3_togg_1->setEnabled(true);
        this->ui->a3_togg_2->setEnabled(true);
        this->ui->a3_togg_3->setEnabled(true);
    }
}


void MainWindow::on_limit_window_btn_clicked()
{
    this->limitWindow->setWindowModality(Qt::ApplicationModal);
    this->limitWindow->show();
}


void MainWindow::on_btn_forced_override_clicked()
{
    QMessageBox message(QMessageBox::Warning, "警告",
        "　　您正在试图进入强制覆盖模式（高级）。\n"
        "　　强制覆盖模式下，所有未选中的勾选框都会成为可用状态。如果勾选某内容，"
        "新功能修改的EXE字节码将会强行覆盖源文件的字节码，即使源文件字节码和修改器期望看到的字节并不相同。\n"
        "　　强制覆盖模式下无法取消勾选源文件已包含的功能。\n"
        "　　强制覆盖模式下，和正常模式不同，未勾选的内容将不会对EXE产生任何影响。\n"
        "　　注意：如果正常模式下勾选框为灰色，说明该内容对应的代码段和原版并不相同，可能已被其它程序修改过，"
        "此时再用强制覆盖模式覆盖，很可能产生意料之外的后果！\n"
        "　　除非您很清楚您的EXE可以进行此类修改，否则强烈建议您避免使用强制覆盖模式。\n"
        "　　进入强制覆盖模式后，无法再次回到正常模式，无法重新载入文件。\n"
        "　　按左侧“Yes”进入强制覆盖模式，按右侧“Cancel”回到正常模式。",
                        QMessageBox::Yes|QMessageBox::Cancel,NULL);
    if (message.exec()==QMessageBox::Yes){
        this->isForcedOverride = true;
        this->ui->btn_forced_override->setText("强制覆盖模式已开启");
        this->ui->btn_forced_override->setStyleSheet("background-color: rgba(255,0,0,200)");
        this->ui->btn_forced_override->setEnabled(false);

        this->ui->loadFileBtn->setEnabled(false);
        this->ui->selectPathBtn->setEnabled(false);

        forcedOverrideButtonEnabled();
    }
}
