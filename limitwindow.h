#ifndef LIMITWINDOW_H
#define LIMITWINDOW_H

#include <QDialog>
#include "contentloader.h"
#include "mainwindow.h"

class MainWindow;

namespace Ui {
class LimitWindow;
}

class LimitWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LimitWindow(QWidget *parent = nullptr);
    ~LimitWindow();

    Ui::LimitWindow *ui;

    void loadContent(char* content);
    void addNewHeader(MainWindow* mainwindow, int addr, int size);
    void onStatusLoad(MainWindow* mainwindow, ContentLoader& contentloader, ExeType exe_type);
    void onStatusWrite(MainWindow* mainwindow, ContentLoader& contentloader, ExeType exe_type);

private slots:

    void on_item_chkbox_stateChanged(int arg1);

    void on_pathpoint_chkbox_stateChanged(int arg1);

    void on_section_chkbox_stateChanged(int arg1);

    void on_troop_chkbox_stateChanged(int arg1);

    void on_city_chkbox_stateChanged(int arg1);

    void on_section_box1_editingFinished();

    void on_save_chkbox_stateChanged(int arg1);

private:
    char* content;

    // header-file relate stuff
    typedef char BYTE;
    typedef short WORD;
    typedef int DWORD;

    // all header structures
    typedef struct _IMAGE_FILE_HEADER {
      WORD  Machine;
      WORD  NumberOfSections;
      DWORD TimeDateStamp;
      DWORD PointerToSymbolTable;
      DWORD NumberOfSymbols;
      WORD  SizeOfOptionalHeader;
      WORD  Characteristics;
    } IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

    typedef struct _IMAGE_DATA_DIRECTORY {
      DWORD VirtualAddress;
      DWORD Size;
    } IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

    typedef struct _IMAGE_OPTIONAL_HEADER {

      /* Standard fields */

      WORD  Magic; /* 0x10b or 0x107 */	/* 0x00 */
      BYTE  MajorLinkerVersion;
      BYTE  MinorLinkerVersion;
      DWORD SizeOfCode;
      DWORD SizeOfInitializedData;
      DWORD SizeOfUninitializedData;
      DWORD AddressOfEntryPoint;		/* 0x10 */
      DWORD BaseOfCode;
      DWORD BaseOfData;

      /* NT additional fields */

      DWORD ImageBase;
      DWORD SectionAlignment;		/* 0x20 */
      DWORD FileAlignment;
      WORD  MajorOperatingSystemVersion;
      WORD  MinorOperatingSystemVersion;
      WORD  MajorImageVersion;
      WORD  MinorImageVersion;
      WORD  MajorSubsystemVersion;		/* 0x30 */
      WORD  MinorSubsystemVersion;
      DWORD Win32VersionValue;
      DWORD SizeOfImage;
      DWORD SizeOfHeaders;
      DWORD CheckSum;			/* 0x40 */
      WORD  Subsystem;
      WORD  DllCharacteristics;
      DWORD SizeOfStackReserve;
      DWORD SizeOfStackCommit;
      DWORD SizeOfHeapReserve;		/* 0x50 */
      DWORD SizeOfHeapCommit;
      DWORD LoaderFlags;
      DWORD NumberOfRvaAndSizes;
      IMAGE_DATA_DIRECTORY DataDirectory[16]; /* 0x60 */
      /* 0xE0 */
    } IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

    typedef struct _IMAGE_NT_HEADERS {
      DWORD Signature; /* "PE"\0\0 */	/* 0x00 */
      IMAGE_FILE_HEADER FileHeader;		/* 0x04 */
      IMAGE_OPTIONAL_HEADER OptionalHeader;	/* 0x18 */
    } IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

    typedef struct _IMAGE_DOS_HEADER {       //DOS .EXE header                                    位置
        WORD e_magic;                       //Magic number;                                      0x00
        WORD e_cblp;                        //Bytes on last page of file                         0x02
        WORD e_cp;                          //Pages in file                                      0x04
        WORD e_crlc;                        //Relocations                                        0x06
        WORD e_cparhdr;                     //Size of header in paragraphs                       0x08
        WORD e_minalloc;                    //Minimum extra paragraphs needed                    0x0A
        WORD e_maxalloc;                    //Maximum extra paragraphs needed                    0x0C
        WORD e_ss;                          //Initial (relative) SS value                        0x0E
        WORD e_sp;                          //Initial SP value                                   0x10
        WORD e_csum;                        //Checksum                                           0x12
        WORD e_ip;                          //Initial IP value                                   0x14
        WORD e_cs;                          //Initial (relative) CS value                        0x16
        WORD e_lfarlc;                      //File address of relocation table                   0x18
        WORD e_ovno;                        //Overlay number                                     0x1A
        WORD e_res[4];                      //Reserved words                                     0x1C
        WORD e_oemid;                       //OEM identifier (for e_oeminfo)                     0x24
        WORD e_oeminfo;                     //OEM information; e_oemid specific                  0x26
        WORD e_res2[10];                    //Reserved words                                     0x28
        DWORD e_lfanew;                      //File address of new exe header                     0x3C
    } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

    typedef struct _IMAGE_SECTION_HEADER {
      BYTE  Name[8];
      union {
        DWORD PhysicalAddress;
        DWORD VirtualSize;
      } Misc;
      DWORD VirtualAddress;
      DWORD SizeOfRawData;
      DWORD PointerToRawData;
      DWORD PointerToRelocations;
      DWORD PointerToLinenumbers;
      WORD  NumberOfRelocations;
      WORD  NumberOfLinenumbers;
      DWORD Characteristics;
    } IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

    PIMAGE_DOS_HEADER dos_header;
    PIMAGE_NT_HEADERS nt_header;
    PIMAGE_FILE_HEADER file_header;
    PIMAGE_OPTIONAL_HEADER optional_header;
    PIMAGE_SECTION_HEADER pSections;

};

#endif // LIMITWINDOW_H
