#include "declarari.h"
#include "functii.h"



HWND textbox,textbox2,Properties,h,frame;
HWND btn1,btn2, checkbox, check_size, check_color, set, set_size, check_type, set_type;
long stilul_main;
CHARFORMAT2A cf,cf2;
int iYCurrentScroll=0;
int lin=0,salvare;
char szFile[MAX_PATH];
int i=1;
char **a;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, LPSTR lpcmdline, int nCmdShow){
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC=0;
    InitCommonControlsEx(&iccx);
    LoadLibrary(TEXT("Riched20.dll"));
    HMENU hmenu;
    WNDCLASSEX w;
    w.cbSize = sizeof(WNDCLASSEX);
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    w.hCursor = LoadCursor(NULL,IDC_ARROW);
    w.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    w.lpszMenuName = "hmenu";
    w.lpszClassName = "win32";
    w.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_APPLICATION));
    RegisterClassEx(&w);
    hmenu = LoadMenu(hInstance, MAKEINTRESOURCE(ID_Menu));
    h = CreateWindow("win32","Notepad", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT , CW_USEDEFAULT , 500, 500, NULL , hmenu , hInstance , NULL);
    DragAcceptFiles(h,TRUE);
    ShowWindow(h , nCmdShow);
    UpdateWindow(h);

    MSG msg;
    while(GetMessage(&msg,NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd , UINT message, WPARAM wParam , LPARAM lParam) {
    long style;
    char* file;
    bool k=false;
    RECT r;
    switch(message){
        case WM_CREATE:
            RECT r;
            GetClientRect(hWnd, &r);
            DLGHDR *p = (DLGHDR*) LocalAlloc(LPTR,sizeof(DLGHDR));
            p->hwndTab = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS , 0, 0, r.right - r.left, r.bottom-r.top, hWnd, NULL, NULL, NULL);
            //textbox2 = CreateWindow("RichEdit20A","", WS_CHILD | WS_DISABLED | WS_VISIBLE | ES_MULTILINE , 0, 1,0, r.bottom-r.top+10, textbox, NULL, NULL, NULL);

            TCITEM tie;
            char abc[20];
            tie.mask = TCIF_TEXT;
            tie.pszText = "primul";
            TabCtrl_InsertItem(p->hwndTab, 0, &tie);
            p->apRes[0] = CreateWindow("RichEdit20A","Hallo \cf1 America \cf0 !", WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_MULTILINE ,0,22, r.right - r.left, r.bottom-r.top, p->hwndTab, NULL, NULL, NULL);
            p->apNumber[0] = CreateWindow("RichEdit20A","", WS_CHILD | WS_DISABLED | WS_VISIBLE | ES_MULTILINE , 0, 1,0, r.bottom-r.top+10, p->apRes[0], NULL, NULL, NULL);

            SetTimer(hWnd,NULL,1000,NULL);
            RECT r2;
            GetClientRect(p->apRes[0], &r2);
            r2.left = r2.left+32;
            SendMessage(p->apRes[0], EM_SETRECT, 0, (LPARAM)&r2);
            SendMessage(p->apRes[0], EM_SETEVENTMASK, 0, ENM_SCROLL | ENM_SCROLLEVENTS);
            NumarLinii(p->apRes[0],p->apNumber[0]);
            SetWindowText(p->apNumber[0],"asdasd");

            TabCtrl_InsertItem(p->hwndTab, 1, &tie);
/*
            cf.cbSize=sizeof(CHARFORMAT2A);
            cf.dwMask=CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD;
            cf.dwEffects=0;
            cf.crTextColor=RGB(0, 0, 0);
            cf.crBackColor=RGB(0,0,0);
            strcpy(cf.szFaceName,"Calibri");
            cf.bCharSet=ANSI_CHARSET;
            cf.yHeight = (10*1440)/72;
            cf2 = cf;
            SendMessage(textbox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
            SendMessage(textbox2, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
            /*HFONT hFont = CreateFont (16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
            SendMessage(textbox2,WM_SETFONT,(WPARAM)hFont,TRUE);*/
//            SpecialWord("int");
            EnumChildWindows(hWnd,(WNDENUMPROC) EnumChildProc,(LONG)SubClassProc);
            a = GetSpecial();
            break;
       case WM_VSCROLL:
            MessageBox(0,"ddsads","asdsa",MB_OK);
            break;
       case WM_SIZE:
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            size(pt.x,pt.y);
            break;
       case WM_DROPFILES:
            char* nume = new char[100];
            DragQueryFile((HDROP)wParam,0,nume,100);
            SetWindowText(h,nume);
            write(nume);
            break;
       case WM_COMMAND:
            if (HIWORD(wParam) == EN_VSCROLL){
                Text_Scroll();
            }
            switch(wParam){
                case ID_Exit:
                    DestroyWindow(hWnd);
                    break;
                case ID_ls:{
                    if (k){
                        RECT r;
                        GetClientRect(textbox, &r);
                        r.left = r.left-100;
                        SendMessage(textbox, EM_SETRECT, 0, (LPARAM)&r);
                        k = false;
                    }
                    ShowWindow(textbox2,1);
                    }
                    break;
                case ID_lh:
                    {
                    RECT r;
                    GetClientRect(textbox, &r);
                    r.left = r.left-32;
                    SendMessage(textbox, EM_SETRECT, 0, (LPARAM)&r);
                    k = true;
                    ShowWindow(textbox2,0);
                    break;
                    }
                case ID_New:
                    ShowScrollBar(textbox,SB_VERT, FALSE);
                    ShowScrollBar(textbox,SB_HORZ, FALSE);
                    SetWindowText(textbox,"");
                    SetWindowText(textbox2,"");
                    SetWindowText(h,"Notepad");
                    break;
                case ID_Open:
                    {
                    file = OpenFile(hWnd);
                    SetWindowText(h,file);
                    write(file);
                    }
                    break;
                case ID_Save:
                    {
                        Salvare();
                    }
                    break;
                case ID_SaveAs:
                    {
                        char* nume = SaveFile(h);
                        SaveTextFileFromEdit(textbox,nume);
                        SetWindowText(h,nume);
                    }
                    break;
                case ID_Pro:
                    WNDCLASSEX w2;
                    w2.cbSize = sizeof(WNDCLASSEX);
                    w2.style = CS_HREDRAW | CS_VREDRAW;
                    w2.lpfnWndProc = Prop_proc;
                    w2.cbClsExtra = 0;
                    w2.cbWndExtra = 0;
                    w2.hCursor = LoadCursor(NULL,IDC_ARROW);
                    w2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
                    w2.lpszMenuName = "hmenu";
                    w2.lpszClassName = "Properties";
                    RegisterClassEx(&w2);
                    Properties = CreateWindow("Properties","Properties", WS_VISIBLE | WS_OVERLAPPED, 200, 200, 400, 300, hWnd, NULL, NULL, NULL);

                    long style = GetWindowLong(hWnd, GWL_STYLE);
                    stilul_main = style;
                    style = stilul_main | WS_DISABLED;
                    SetWindowLong(h,GWL_STYLE, style);
                    break;
            }
            break;
       case WM_TIMER:
            break;
       case WM_CLOSE:
            /*int quit = MessageBox(0,"Iesiti din program?","Mesaj",MB_YESNO | MB_ICONSTOP);
            if (quit == 6){
                int save = MessageBox(0,"Vreti sa salvati?","Mesaj",MB_YESNO | MB_ICONQUESTION);
                if (save == 6) Salvare();
                PostQuitMessage(0);
            }*/
            PostQuitMessage(0);
            break;
       case WM_DESTROY:
            PostQuitMessage(0);
            break;
       default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }
    return 0;
}
