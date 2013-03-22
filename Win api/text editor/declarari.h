#ifndef declarari_H
#define declarari_H
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream.h>
#include <Richedit.h>
#include "resource.h"
#include <TCHAR.h>
#if defined __MINGW_H
#define _WIN32_IE 0x0400
#endif
#include <commctrl.h>
extern HWND textbox,textbox2,Properties,h,frame;
extern HWND btn1,btn2, checkbox, check_size, check_color, set, set_size, check_type, set_type;
extern CHARFORMAT2A cf,cf2;
extern int lin,salvare;
extern long stilul_main;
extern char szFile[MAX_PATH];
extern int i;

typedef struct tag_dlghdr {
    HWND hwndTab;       // tab control
    HWND hwndDisplay;   // current child dialog box
    RECT rcDisplay;     // display rectangle for the tab control
    HWND apRes[3];
    HWND apNumber[3];
} DLGHDR;

#endif
