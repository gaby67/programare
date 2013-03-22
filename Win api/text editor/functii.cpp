#include "declarari.h"
using namespace std;
void Text_Scroll();
void NumarLinii(HWND&,HWND&);
void SpecialWord(char* a,COLORREF,int a=0);
void SpecialWord_After();
BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);
int LastSpace();
extern int marime;
extern char** a;
int limit(int n){
    int i,line;
    SendMessage(textbox,EM_GETSEL,(WPARAM)&i,NULL);
    line = SendMessage(textbox,EM_LINEFROMCHAR,(WPARAM)i,NULL);
    return line;
}


void size(int width, int height){
    SetWindowPos(textbox,NULL,0,0,width,height,NULL);
    SetWindowPos(textbox2,NULL,-1,0,30,height+10,NULL);
    NumarLinii(textbox,textbox2);
}

int CALLBACK Enum(ENUMLOGFONTEX *lp, NEWTEXTMETRICEX *l2, DWORD FontType, LPARAM lParam)
{
    if (lp->elfFullName[0] != '@')
        SendMessage(checkbox, LB_ADDSTRING,  NULL, (LPARAM)lp->elfFullName);
    return 1;
}


void checkc(){
        CHOOSECOLOR color;//the struct for the dialog
        COLORREF ccref[16];//custom colors
        COLORREF selcolor=0x000000;//the default selected color
        memset(&color,0,sizeof(color));
        color.lStructSize=sizeof(CHOOSECOLOR);
        color.hwndOwner=Properties;
        color.lpCustColors=ccref;
        color.rgbResult=selcolor;
        color.Flags=CC_RGBINIT;

        CHARFORMAT2 c3;
        c3.cbSize=sizeof(CHARFORMAT2A);
        c3.dwMask=CFM_COLOR;
        c3.dwEffects=0;
    if(ChooseColor(&color))
    {
        cf.crTextColor=color.rgbResult;
        c3.crBackColor=color.rgbResult;
        ::SendMessage(textbox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
        //::SendMessage(check_color, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&c3);
        //redraw with the new color
    }
}

void checkb(){
    int a[] = { 0, 0, 0 };
    LOGFONT logFont;
    ZeroMemory(&logFont, sizeof(LOGFONT));
    logFont.lfCharSet = ANSI_CHARSET;
    EnumFontFamiliesEx(GetDC(textbox), &logFont, (FONTENUMPROC)Enum, NULL, NULL);

}

void checks(HWND check_size){
    char *t;
    for (i=0;i<25;i++){
        t = new char[10];
        itoa(i,t,10);
        SendMessage(check_size, LB_ADDSTRING,  NULL, (LPARAM)t);
        delete[] t;
    }
}

BOOL CALLBACK EnumChildProc(HWND handle,LPARAM lParam)
{
        WNDPROC Sub;
        Sub=(WNDPROC) SetWindowLong(handle,GWL_WNDPROC,(LONG)lParam);
        SetWindowLong(handle,GWL_USERDATA,(LONG)Sub);
        return TRUE;
}

char* OpenFile(HWND hWnd){
    OPENFILENAME o;
    ZeroMemory(&o,sizeof(o));
    o.lStructSize = sizeof(o);
    o.hwndOwner = hWnd;
    szFile[0]='\0';
    o.lpstrFile = szFile;
    o.nMaxFile = MAX_PATH;
    o.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&o)){
        return szFile;
    }
}

char* SaveFile(HWND hWnd){
    OPENFILENAME o;
    ZeroMemory(&o,sizeof(o));
    o.lStructSize = sizeof(o);
    o.hwndOwner = hWnd;
    szFile[0]='\0';
    o.lpstrFile = szFile;
    o.nMaxFile = MAX_PATH;
    o.Flags = OFN_OVERWRITEPROMPT;
    if (GetSaveFileName(&o)){
        return szFile;
    }
}


int size(char *nr){
	int i = 0;
	while(nr[i]) i++;
	return i;
}

int comparare(char *alfa, char *beta){
	int i = 0;
	int n = size(alfa);
	int m = size(beta);
	if (n != m ) return 0;
	while(i<n){
        if (alfa[i] != beta[i]) return 0;
        i++;
	}
	return 1;
}

struct Selectie{
    int beg;
    int end;
    COLORREF c;
};

struct Vector{
    Selectie* e;
    int n;
    int capacity;
    Vector(){
        e = new Selectie[10];
        n = 0;
        capacity = 10;
    }
    void add(Selectie elem){
        int i;
        Selectie *aux;
        if (n < capacity){
            e[n++] = elem;
        }
        else{
            aux = new Selectie [capacity*2];
            for(i=0;i<n;i++)
                aux[i] = e[i];
            aux[n] = elem;
            n++;
            delete[] e;
            e = aux;
            capacity *= 2;
        }
    }
    Selectie operator[](int i){
        return e[i];
    }
};

void InCuvinte(Vector& v, string line,int global){
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    char z[25]={';','(',')',':','.','!','[',']','+','-','*','\\','{','}','&',',','-','<','>',' ','"','=','\'','+',9};
    string word;
    Selectie s;
    int i=0,k,n=0,cursor=0;
    int m = size((char*)line.c_str());
    while(line[i] != '\n'){
        k = 1;
        for(int j=0;j<25;j++)
            if (line[i] == z[j]){
                k = 0;
                for(int j2=0;j2<marime;j2++)
                if (comparare((char*)word.c_str(),a[j2])) {
                    s.beg = cursor+global;
                    s.end = i+global;
                    s.c = RGB(0,0,100);
                    v.add(s);
                }
                word = "";
                s.beg = i+global;
                s.end = i+1+global;
                s.c = RGB(200,0,0);
                v.add(s);
                cursor = i+1;
                break;
        }
        if (k) word += line[i];
        i++;
    }
}

void write(char* file){
    SendMessage(textbox,WM_SETREDRAW, false, 0);
    int eventMask = SendMessage(textbox, EM_SETEVENTMASK, 0, 0);
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    Vector v;
    std::ifstream f(file);
    std::string b,line;

    CHARFORMAT2 c;
    c.cbSize=sizeof(CHARFORMAT2A);
    c.dwMask=CFM_COLOR | CFM_BOLD;
        b += line;
    c.dwEffects=CFE_BOLD;
    c.crTextColor=RGB(0,0,100);
    int global = 0;
    while(getline(f,line)){
        line +="\n";
        InCuvinte(v,line,global);
        b += line;
        global += size((char*)line.c_str());
    }
    SetWindowText(textbox,b.c_str());
    NumarLinii(textbox,textbox2);
    for(int i=0;i<v.n;i++){
        SendMessage(textbox,EM_SETSEL,v.e[i].beg,v.e[i].end);
        c.crTextColor = v.e[i].c;
        SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&c);
    }
    SendMessage(textbox,WM_SETREDRAW, true, 0);
    SendMessage(textbox, EM_SETEVENTMASK, 0, eventMask);
    InvalidateRect(textbox, 0, true);
    SendMessage(textbox,EM_SETSEL,0,0);
}

LRESULT CALLBACK Prop_proc(HWND hWnd , UINT message, WPARAM wParam , LPARAM lParam) {
    switch(message){
        case WM_CREATE:
            RECT r;
            GetClientRect(hWnd,&r);
            btn1 = CreateWindow("button","Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , (r.right - r.left)/2 , r.bottom - 25 , 100, 25, hWnd, (HMENU)btn_ex, NULL, NULL);
            btn2 = CreateWindow("button","OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , (r.right - r.left)/2 - 110 , r.bottom - 25 , 100, 25, hWnd, (HMENU)btn_ok, NULL, NULL);
            HWND edit = CreateWindow("edit","FONT:", WS_CHILD | WS_VISIBLE , r.left + 10, 5 , 45, 30, hWnd , NULL, NULL, NULL);
            //WS_EX_STATICEDGE
            set = CreateWindowEx(WS_EX_CLIENTEDGE,"edit","  Comic Sans MS", WS_CHILD | WS_VISIBLE , r.left + 46, 4 , 150, 17, hWnd , NULL, NULL, NULL);
            checkbox = CreateWindowEx(WS_EX_STATICEDGE,"LISTBOX","",WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_SORT, r.left+10 , 30 , 190, r.bottom - r.top-80, hWnd, NULL, NULL, NULL);

            HWND edit_size = CreateWindow("edit","SIZE:", WS_CHILD | WS_VISIBLE, r.left + 210, 5 , 40, 30, hWnd , NULL, NULL, NULL);
            set_size = CreateWindowEx(WS_EX_CLIENTEDGE,"edit","11", WS_CHILD | WS_VISIBLE, r.left + 240, 4 , 30, 17, hWnd , NULL, NULL, NULL);
            check_size = CreateWindowEx(WS_EX_STATICEDGE,"LISTBOX","",WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY, r.left+210, 30 , 60, r.bottom - r.top-80, hWnd, NULL, NULL, NULL);

            HWND edit_type = CreateWindow("edit","COLOR:", WS_CHILD | WS_VISIBLE , r.left+285 , 5, 50, 30, hWnd, NULL, NULL, NULL);
            set_type = CreateWindowEx(WS_EX_STATICEDGE,"edit","apa",WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, r.left+330, 4 , 60, 17, hWnd, NULL, NULL, NULL);
            check_type = CreateWindow("LISTBOX","",WS_CHILD | WS_VISIBLE |  WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY, r.left+285, 30 , 104, r.bottom-90, hWnd, NULL, NULL, NULL);


            //HWND edit_color = CreateWindow("edit","COLOR:", WS_CHILD | WS_VISIBLE , r.left+285 , 5, 50, 30, hWnd, NULL, NULL, NULL);
            //check_color = CreateWindow(WC_COMBOBOX,"",WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, r.left+330, 0 , 60, 80, hWnd, NULL, NULL, NULL);

            long style = GetWindowLong(edit, GWL_STYLE);
            style = style | WS_DISABLED;
            SetWindowLong(edit,GWL_STYLE, style);
            SetWindowLong(edit_size,GWL_STYLE, style);
            SetWindowLong(edit_type,GWL_STYLE, style);
            //SetWindowLong(set,GWL_STYLE, style);
            //SetWindowLong(set_size,GWL_STYLE, style);
            SetWindowLong(set_type,GWL_STYLE, style);

            HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(btn1, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(btn2, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(edit, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(edit_size, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(edit_type, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(checkbox, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(check_size, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(check_color, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(set, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(set_size, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(set_type, WM_SETFONT, (WPARAM)hFont, TRUE);

            SendMessage(check_color,CB_ADDSTRING, NULL,(LPARAM)"apa");
            SendMessage(check_color, CB_SETCURSEL, 0, NULL);
            checkb();
            checks(check_size);
            break;
        case WM_COMMAND:
            switch(wParam){
                case btn_ex:
                    SendMessage(textbox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf2);
                    SetWindowLong(h,GWL_STYLE, stilul_main);
                    DestroyWindow(hWnd);
                    break;
                case btn_ok:
                    cf2 = cf;
                    SetWindowLong(h,GWL_STYLE, stilul_main);
                    DestroyWindow(hWnd);
                    break;
                case VK_RETURN:
                    ::MessageBox(NULL,"a","b",MB_OK);

            }
            switch(HIWORD(wParam)){
                case CBN_SELCHANGE:
                    if ((HWND)lParam == check_size){
                       i = SendMessage((HWND)lParam, LB_GETCURSEL, NULL, NULL);
                       if (i>0){
                           cf.yHeight = (i*1440)/72;
                           ::SendMessage(textbox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
                           ::SendMessage(textbox2, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
                           char s[10];
                           itoa(i,s,10);
                           SetWindowText(set_size,s);
                       }
                    }
                    if ((HWND)lParam == checkbox){
                        i = SendMessage((HWND)lParam, LB_GETCURSEL, NULL, NULL);
                        if (i>0){
                            char s[20];
                            SendMessage(checkbox, LB_GETTEXT, i, (LPARAM)s);
                            strcpy(cf.szFaceName,s);
                            SendMessage(textbox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
                            ::SendMessage(textbox2, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
                            SetWindowText(set,s);
                       }
                    }
                    break;
                case CBN_SETFOCUS:
                    if ((HWND)lParam == check_color){
                            checkc();
                    }
                    break;
            }
            break;
        case WM_DESTROY:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }
}

void Salvare(){
    char nume[100];
    int i;
    i = GetWindowTextLength(h);
    GetWindowText(h,nume,i+1);
    if (comparare(nume,"Notepad")){
        char* x = SaveFile(h);
        SaveTextFileFromEdit(textbox,x);
        SetWindowText(h,x);
    }
    else
        SaveTextFileFromEdit(textbox,nume);
}


LRESULT CALLBACK SubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int q,m;
    char * file;
    char b[15][2]={";","(",")",":",".","!","[","]","+","-","*","\\","{","}","&"};
    switch(message){
    case WM_CHAR:
        switch(wParam){
        case VK_RETURN:
            NumarLinii(textbox,textbox2);
            break;
    }
        break;
    case WM_KEYUP:{
        int z = GetKeyState(VK_LCONTROL);
        if (wParam == (int)'O' && z < 0){
            file = OpenFile(h);
            SetWindowText(h,file);
            write(file);
            NumarLinii(textbox,textbox2);
            ShowScrollBar(textbox,SB_VERT, FALSE);
            for(i=0;i<marime;i++)
                SpecialWord(a[i],RGB(0,0,120),1);
            for(i=0;i<16;i++)
                SpecialWord(b[i],RGB(200,0,0));
        }
        if (wParam == (int)'S' && z < 0){
            Salvare();
        }
        for(char i='A';i<='Z';i++){
            if(wParam == (int)i && z > -127){
                SpecialWord_After();
                NumarLinii(textbox,textbox2);
            }
        }
        }
        break;
    case WM_VSCROLL:
        Text_Scroll();
        break;
    }
    return CallWindowProc((WNDPROC)GetWindowLong(hWnd,GWL_USERDATA),hWnd,message,wParam,lParam);
}

void Text_Scroll(){
    RECT r;
    int poz=0;
    GetWindowRect(textbox,&r);
    SCROLLINFO si = {0};
    memset(&si, 0, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(textbox, SB_VERT, &si);
    poz = (si.nTrackPos) ? si.nTrackPos : si.nPos;
    MoveWindow(textbox2, 0, -poz, 30, r.bottom-r.top+poz, true);
}

void NumarLinii(HWND& tbox,HWND& tbox2){
    string text;
    int i,j=0,k=0,count,linie,n,cautare,v_linie=0;
    count = SendMessage(tbox,EM_GETLINECOUNT,0,0);
    char b[10];
    while(j<count){
        k+=i+1;
        itoa(j+1,b,10);
        text += " ";
        if (j<9) text += '0';
        text += b;
        text += "\n";
        j++;
    }
    SetWindowText(tbox2,text.c_str());
}

int LastSpace(){
    DWORD sel,kapa,i=1;
    char line[3];
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    SendMessage(textbox,EM_GETSEL,(WPARAM)&sel,NULL);
    while(line[0] != '\0'){
    kapa = sel;
    SendMessage(textbox,EM_SETSEL,kapa-i-1,kapa-i);
    SendMessage(textbox,EM_GETSELTEXT,kapa-i,(LPARAM)&line);
    if (line[0] == ' ') line[0] = '\0';
    i++;
    }
    SendMessage(textbox,EM_SETSEL,sel,sel);
    return kapa-i;
}

void SpecialWord_After(){
    DWORD sel;
    char line[20];
    int last = LastSpace();
    bool k,q;
    CHARFORMAT2 c;
    c.cbSize=sizeof(CHARFORMAT2A);
    c.dwMask=CFM_COLOR | CFM_BOLD;
    c.dwEffects=CFE_BOLD;
    c.crTextColor=RGB(0,0,100);

    int j,s,s_a;
    SendMessage(textbox,EM_GETSEL,(WPARAM)&sel,NULL);
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    SendMessage(textbox,EM_SETSEL,last+1,sel);
    int n = SendMessage(textbox,EM_GETSELTEXT, 0, (LPARAM)line);
    q = false;
    for(i=0;i<marime;i++){
        k = true;
        j = 0;
        while(a[i][j] || line[j]){
            if(line[j] != a[i][j]) k= false;
            j++;
        }
        if(k){
            SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
            q = true;
        }
    }

    c.dwEffects=0;
    c.crTextColor=RGB(0,0,0);
    cout<<q;
    if (!q){
        c.dwEffects=0;
        c.crTextColor=RGB(0,0,0);
        SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
    }
    SendMessage(textbox,EM_SETSEL,sel,sel);
    SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
    SendMessage(textbox,EM_HIDESELECTION,FALSE,0);
}

void SpecialWord(char* a, COLORREF cl , int da){
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    int i=10,capacity,line=0,length;
    DWORD sel;
    SendMessage(textbox,EM_GETSEL,(WPARAM)&sel,NULL);
    capacity = GetWindowTextLength(textbox);

    CHARFORMAT2 c;
    c.cbSize=sizeof(CHARFORMAT2A);
    c.dwMask=CFM_COLOR | CFM_BOLD;
    c.dwEffects=CFE_BOLD;

    CHARRANGE chr;
    chr.cpMin = sel;
    chr.cpMax = capacity;

    FINDTEXT f;
    f.chrg = chr;
       c.crTextColor=cl;
       f.lpstrText = a;
       while(i<capacity && i>0){
            if (da){
            i = SendMessage(textbox,EM_FINDTEXT,FR_DOWN | FR_WHOLEWORD,(LPARAM)&f);
            }
            else{
            i = SendMessage(textbox,EM_FINDTEXT,FR_DOWN ,(LPARAM)&f);
            }
            if (i>=0){
            chr.cpMin = i+1;
            f.chrg = chr;
            SendMessage(textbox,EM_SETSEL,i,i+strlen(a));
            SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
            }
        }
    SendMessage(textbox,EM_SETSEL,sel,sel);
    SendMessage(textbox,EM_HIDESELECTION,FALSE,0);
}

int marime;

char** GetSpecial(){
    char **z;
    string line;
    z = new char*[100];
    std::ifstream f("cpp");
    int i=0,j;
    while(getline(f,line)){
        j = 0;
        line +=" ";
        z[i] = new char[20];
        while(line[j] != ' '){
            z[i][j] = line[j];
            j++;
        }
        z[i][j] = '\0';
        i++;
    }
    marime = i;
    return z;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwTextLength;

        dwTextLength = GetWindowTextLength(hEdit);
        // No need to bother if there's no text.
        if(dwTextLength > 0)
        {
            char* pszText;
            DWORD dwBufferSize = dwTextLength + 1;

            pszText = (char *)GlobalAlloc(GPTR, dwBufferSize);
            if(pszText != NULL)
            {
                if(GetWindowText(hEdit, pszText, dwBufferSize))
                {
                    DWORD dwWritten;

                    if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
                        bSuccess = TRUE;
                }
                GlobalFree(pszText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}


