#include "limitwindow.h"
#include "ui_limitwindow.h"

LimitWindow::LimitWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LimitWindow)
{
    ui->setupUi(this);
}

LimitWindow::~LimitWindow()
{
    delete ui;
}

void LimitWindow::loadContent(char *content) {
    this->content = content;

    // Just load all headers...
    dos_header = (PIMAGE_DOS_HEADER) content;
    nt_header = (PIMAGE_NT_HEADERS) (content + dos_header->e_lfanew);
    file_header = &(nt_header->FileHeader);
    optional_header = &(nt_header->OptionalHeader);
    pSections = (PIMAGE_SECTION_HEADER)
                (((const BYTE *)&(nt_header)->OptionalHeader) + file_header->SizeOfOptionalHeader);

    // check if there is a .s105p header and load content
    for (int i = 0; i < file_header->NumberOfSections; i++) {
        if (strcmp(pSections[i].Name, ".s105p") == 0) {
            this->ui->section_chkbox->setChecked(true);
            // Virtual address is RVA
            this->ui->section_box1->setText(QString::number(pSections[i].VirtualAddress + 0x400000, 16));
            this->ui->section_box2->setText(QString::number(pSections[i].Misc.VirtualSize, 16));
            break;
        }
    }
}

void LimitWindow::addNewHeader(MainWindow* mainwindow, int addr, int size) {

    // if there has already been a header named .s105p, we just modify it
    PIMAGE_SECTION_HEADER pNewSection = nullptr;
    for (int i = 0; i < file_header->NumberOfSections; i++) {
        if (strcmp(pSections[i].Name, ".s105p") == 0) {
            pNewSection = &pSections[i];
            break;
        }
    }
    if (pNewSection == nullptr)
        pNewSection = &pSections[file_header->NumberOfSections];

    // properties of the new section
    strcpy(pNewSection->Name, ".s105p");    // name
    pNewSection->VirtualAddress = addr - 0x400000;  // RVA
    pNewSection->Misc.VirtualSize = size;   // virtual size
    pNewSection->SizeOfRawData = 0;         // no physical data
    PIMAGE_SECTION_HEADER pOldSec = &pSections[file_header->NumberOfSections - 1];
    pNewSection->PointerToRawData = pOldSec->PointerToRawData + pOldSec->SizeOfRawData;

    // add a section
    ++file_header->NumberOfSections;

    // go through all sections and find the theoretical largest virtual address
    int max = 0;
    for (int i = 0; i < file_header->NumberOfSections; i++) {
        if (pSections[i].VirtualAddress + pSections[i].Misc.VirtualSize > max)
            max = pSections[i].VirtualAddress + pSections[i].Misc.VirtualSize;
    }
    optional_header->SizeOfImage = max;

    // append zeros to the end of file (not necessary)
    //    mainwindow->append_file_length = size;

    // Make sure that the sectors are consecutive...
    pOldSec->Misc.VirtualSize = pNewSection->VirtualAddress - pOldSec->VirtualAddress;

}

void LimitWindow::onStatusLoad(MainWindow* mainwindow, ContentLoader& contentloader, ExeType exe_type) {

    // load checkboxes first (to determine if line edits are enabled)
    for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
        QString key = it.key();
        if (!it.value().content.contains(exe_type))
            continue;
        QVector<Update> updates = it.value().content[exe_type];
        Update firstUpdate = updates[0];

        if (key == "save_main") {
            if (this->ui->save_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->save_chkbox, updates);

        } else if (key == "gen_limit_main") {
            if (this->ui->gen_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->gen_chkbox, updates);

        } else if (key == "item_limit_main") {
            if (this->ui->item_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->item_chkbox, updates);

        } else if (key == "city_limit_main") {
            if (this->ui->city_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->city_chkbox, updates);

        } else if (key == "path_limit_main") {
            if (this->ui->path_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->path_chkbox, updates);

        } else if (key == "pathpoint_limit_main") {
            if (this->ui->pathpoint_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->pathpoint_chkbox, updates);

        } else if (key == "flag_limit_main") {
            if (this->ui->flag_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->flag_chkbox, updates);

        } else if (key == "king_limit_main") {
            if (this->ui->king_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->king_chkbox, updates);

        } else if (key == "magic_limit_main") {
            if (this->ui->magic_chkbox->isEnabled())
                mainwindow->load_toggle_box(this->ui->magic_chkbox, updates);

        }
    }

    // load line edits next
    for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
        QString key = it.key();
        if (!it.value().content.contains(exe_type))
            continue;
        QVector<Update> updates = it.value().content[exe_type];
        Update firstUpdate = updates[0];

        if (key == "item_limit_box") {
            if (this->ui->city_chkbox->isEnabled())
                this->ui->item_box->setText(QString::number(mainwindow->load_value_status(firstUpdate), 16));

        } else if (key == "city_limit_box") {
            if (this->ui->city_box->isEnabled())
                this->ui->city_box->setText(QString::number(mainwindow->load_value_status(firstUpdate), 16));

        } else if (key == "pathpoint_limit_box") {
            if (this->ui->pathpoint_box->isEnabled())
                this->ui->pathpoint_box->setText(QString::number(mainwindow->load_value_status(firstUpdate), 16));

        }
    }

    // Troop limit requires something special...
    if (exe_type == ExeType::simp105) {
        int troop_limit = *(int *)(content + 0x304FC);
        if (troop_limit != 0x80) {
            this->ui->troop_chkbox->setChecked(true);
            this->ui->troop_box->setText(QString::number(troop_limit));
        }
    }

}

void LimitWindow::onStatusWrite(MainWindow* mainwindow, ContentLoader& contentloader, ExeType exe_type) {
    for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
        QString key = it.key();
        if (!it.value().content.contains(exe_type))
            continue;
        QVector<Update> updates = it.value().content[exe_type];

        if (key == "save_main" && this->ui->save_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->save_chkbox->isChecked());

        } else if (key == "gen_limit_main" && this->ui->gen_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->gen_chkbox->isChecked());

        } else if (key == "item_limit_main" && this->ui->item_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->item_chkbox->isChecked());

        } else if (key == "city_limit_main" && this->ui->city_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->city_chkbox->isChecked());

        } else if (key == "path_limit_main" && this->ui->path_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->path_chkbox->isChecked());

        } else if (key == "pathpoint_limit_main" && this->ui->pathpoint_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->pathpoint_chkbox->isChecked());

        } else if (key == "flag_limit_main" && this->ui->flag_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->flag_chkbox->isChecked());

        } else if (key == "king_limit_main" && this->ui->king_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->king_chkbox->isChecked());

        } else if (key == "magic_limit_main" && this->ui->magic_chkbox->isEnabled()) {
            mainwindow->save_toggle_status(updates, this->ui->magic_chkbox->isChecked());

        }
    }

    for (auto it = contentloader.patches.begin(); it != contentloader.patches.end(); ++it) {
        QString key = it.key();
        if (!it.value().content.contains(exe_type))
            continue;
        QVector<Update> updates = it.value().content[exe_type];

        if (key == "item_limit_box") {
            int value = 0x4c3a60;
            if (this->ui->item_box->isEnabled()) {
                bool ok;
                value = this->ui->item_box->text().toInt(&ok, 16);
                if (!ok || value < 0x400000)
                    throw QString("物品上限破解 - 偏移量不合法");
            }
            mainwindow->save_value_status(updates, value);

        } else if (key == "city_limit_box"){
            int value = 0x4C6BE0;
            if (this->ui->city_box->isEnabled()) {
                bool ok;
                value = this->ui->city_box->text().toInt(&ok, 16);
                if (!ok || value < 0x400000)
                    throw QString("城市上限破解 - 偏移量不合法");
            }
            mainwindow->save_value_status(updates, value);

        } else if (key == "pathpoint_limit_box"){
            int value = 0x4BE9C0;
            if (this->ui->pathpoint_box->isEnabled()) {
                bool ok;
                value = this->ui->pathpoint_box->text().toInt(&ok, 16);
                if (!ok || value < 0x400000)
                    throw QString("路径点上限破解 - 偏移量不合法");
            }
            mainwindow->save_value_status(updates, value);

        }
    }

    // and something additional...
    if (exe_type == ExeType::simp105) {

        // Troop limit requires something special...
        int value;
        if (this->ui->troop_chkbox->isChecked()) {
            value = this->ui->troop_box->text().toInt();
        } else {
            value = 0x80;
        }
        *((int*) (this->content + 0x304FC)) = value;
        *((int*) (this->content + 0x3793E)) = value * 0x6C;
        *((int*) (this->content + 0x3796E)) = value * 0x6C;
        *((int*) (this->content + 0x37998)) = value;
        *((int*) (this->content + 0x379EE)) = value * 0x6C - 4;
        *((int*) (this->content + 0x37C52)) = value;

        // Finally, we save the headers...
        if (this->ui->section_chkbox->isChecked()) {
            bool ok;
            int newsec_addr = this->ui->section_box1->text().toInt(&ok, 16);
            if (!ok || newsec_addr % 0x1000 != 0)
                throw QString("各类上限破解 - 新区段地址不合法");

            int newsec_size = this->ui->section_box2->text().toInt(&ok, 16);
            if (!ok)
                throw QString("各类上限破解 - 新区段大小不合法");

            addNewHeader(mainwindow, newsec_addr, newsec_size);
        }
    }
}

void LimitWindow::on_city_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->city_label1->setEnabled(true);
        this->ui->city_box->setEnabled(true);
        if (!this->ui->section_chkbox->isEnabled()) {
            this->ui->section_chkbox->setEnabled(true);
            this->ui->section_chkbox->setChecked(true);
            this->ui->section_box1->setEnabled(true);
            this->ui->section_box2->setEnabled(true);
            this->ui->section_label1->setEnabled(true);
            this->ui->section_label2->setEnabled(true);
            this->ui->section_label3->setEnabled(true);
            this->ui->section_label4->setEnabled(true);
        }
    } else {
        this->ui->city_label1->setEnabled(false);
        this->ui->city_box->setEnabled(false);
        if (!this->ui->pathpoint_chkbox->isChecked() && !this->ui->item_chkbox->isChecked()) {
            this->ui->section_chkbox->setEnabled(false);
            this->ui->section_chkbox->setChecked(false);
            this->ui->section_box1->setEnabled(false);
            this->ui->section_box2->setEnabled(false);
            this->ui->section_label1->setEnabled(false);
            this->ui->section_label2->setEnabled(false);
            this->ui->section_label3->setEnabled(false);
            this->ui->section_label4->setEnabled(false);
        }
    }
}

void LimitWindow::on_item_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->item_label1->setEnabled(true);
        this->ui->item_box->setEnabled(true);
        if (!this->ui->section_chkbox->isEnabled()) {
            this->ui->section_chkbox->setEnabled(true);
            this->ui->section_chkbox->setChecked(true);
            this->ui->section_box1->setEnabled(true);
            this->ui->section_box2->setEnabled(true);
            this->ui->section_label1->setEnabled(true);
            this->ui->section_label2->setEnabled(true);
            this->ui->section_label3->setEnabled(true);
            this->ui->section_label4->setEnabled(true);
        }
    } else {
        this->ui->item_label1->setEnabled(false);
        this->ui->item_box->setEnabled(false);
        if (!this->ui->pathpoint_chkbox->isChecked() && !this->ui->city_chkbox->isChecked()) {
            this->ui->section_chkbox->setEnabled(false);
            this->ui->section_chkbox->setChecked(false);
            this->ui->section_box1->setEnabled(false);
            this->ui->section_box2->setEnabled(false);
            this->ui->section_label1->setEnabled(false);
            this->ui->section_label2->setEnabled(false);
            this->ui->section_label3->setEnabled(false);
            this->ui->section_label4->setEnabled(false);
        }
    }
}

void LimitWindow::on_pathpoint_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->pathpoint_label1->setEnabled(true);
        this->ui->pathpoint_box->setEnabled(true);
        if (!this->ui->section_chkbox->isEnabled()) {
            this->ui->section_chkbox->setEnabled(true);
            this->ui->section_chkbox->setChecked(true);
            this->ui->section_box1->setEnabled(true);
            this->ui->section_box2->setEnabled(true);
            this->ui->section_label1->setEnabled(true);
            this->ui->section_label2->setEnabled(true);
            this->ui->section_label3->setEnabled(true);
            this->ui->section_label4->setEnabled(true);
        }
    } else {
        this->ui->pathpoint_label1->setEnabled(false);
        this->ui->pathpoint_box->setEnabled(false);
        if (!this->ui->item_chkbox->isChecked() && !this->ui->city_chkbox->isChecked()) {
            this->ui->section_chkbox->setEnabled(false);
            this->ui->section_chkbox->setChecked(false);
            this->ui->section_box1->setEnabled(false);
            this->ui->section_box2->setEnabled(false);
            this->ui->section_label1->setEnabled(false);
            this->ui->section_label2->setEnabled(false);
            this->ui->section_label3->setEnabled(false);
            this->ui->section_label4->setEnabled(false);
        }
    }
}

void LimitWindow::on_section_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->section_box1->setEnabled(true);
        this->ui->section_box2->setEnabled(true);
    } else {
        this->ui->section_box1->setEnabled(false);
        this->ui->section_box2->setEnabled(false);
    }
}

void LimitWindow::on_troop_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->troop_box->setEnabled(true);
    } else {
        this->ui->troop_box->setEnabled(false);
    }
}

void LimitWindow::on_section_box1_editingFinished()
{
    bool ok;
    int value = this->ui->section_box1->text().toInt(&ok, 16);
    if (!ok || value <= 0x400000)
        this->ui->section_box1->setText("570000");
    else if (value % 0x1000) {
        value &= 0xFFFFF000;
        this->ui->section_box1->setText(QString("%1").arg(value, 8, 16).trimmed());
    }
}

void LimitWindow::on_save_chkbox_stateChanged(int arg1)
{
    if (arg1) {
        this->ui->gen_chkbox->setEnabled(true);
        this->ui->path_chkbox->setEnabled(true);
        this->ui->flag_chkbox->setEnabled(true);
        this->ui->troop_chkbox->setEnabled(true);
        this->ui->king_chkbox->setEnabled(true);
        this->ui->magic_chkbox->setEnabled(true);
        this->ui->city_chkbox->setEnabled(true);
        this->ui->item_chkbox->setEnabled(true);
        this->ui->pathpoint_chkbox->setEnabled(true);

        this->ui->advanced_label->setEnabled(true);

    } else {
        this->ui->gen_chkbox->setEnabled(false);
        this->ui->path_chkbox->setEnabled(false);
        this->ui->flag_chkbox->setEnabled(false);
        this->ui->troop_chkbox->setEnabled(false);
        this->ui->king_chkbox->setEnabled(false);
        this->ui->magic_chkbox->setEnabled(false);
        this->ui->city_chkbox->setEnabled(false);
        this->ui->item_chkbox->setEnabled(false);
        this->ui->pathpoint_chkbox->setEnabled(false);

        this->ui->advanced_label->setEnabled(false);
    }
}
