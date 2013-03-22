void NumarLinii(){
    SendMessage(textbox,EM_HIDESELECTION,TRUE,0);
    string text;
    char set[10];
    int i,j=0,k=0,count,linie,n,cautare,v_linie=0;
    count = SendMessage(textbox,EM_GETLINECOUNT,0,0);
    while(j<count){
        i = SendMessage(textbox,EM_LINELENGTH,k,0);
        linie = SendMessage(textbox,EM_LINEFROMCHAR, k, 0);
        while (linie <= v_linie) linie++;
        k+=i+1;
        itoa(linie,set,10);
        text += " ";
        text += set;
        text += "\n";
        j++;
        v_linie = linie;
    }
    SetWindowText(textbox2,text.c_str());
    SendMessage(textbox,EM_HIDESELECTION,FALSE,0);
}


/*char* line;
    int i=0,j=0,count;
    count = SendMessage(textbox,EM_GETLINECOUNT,0,0);
    while(j<count){
    line = new char[1000];
    memset(line,1000, sizeof(char));
    SendMessage(textbox,EM_GETLINE,(WPARAM)j,(LPARAM)line);
    cout<<line;
    delete[] line;
    j++;
    }
    delete[] line;*/
    /*memset(line, 1000, sizeof(char));
    SendMessage(textbox,EM_GETLINE,1,(LPARAM)line);
    std::cout<<line;
    /*SendMessage(textbox,EM_GETLINE,2,(LPARAM)line);
    std::cout<<line;
    SendMessage(textbox,EM_GETLINE,3,(LPARAM)line);
    std::cout<<line;
    //delete[] line;
    //line = new char[1000];
    //memset(line, 1000, sizeof(char));
    //i++;
    //}
    //delete[] line;*/

    /*int i = 1;
    char* line;
    while(i != 0){
    line = new char[200];
    i = SendMessage(textbox,WM_GETTEXT,200,(LPARAM)line);
    cout<<line<<"\n";
    delete[] line;
    }*/


//numarLinii
length = GetWindowTextLength(textbox2);
SendMessage(textbox2,EM_SETSEL,(WPARAM)length,(LPARAM)length);
SendMessage(textbox2,EM_REPLACESEL,FALSE,(LPARAM)"\n");


void NumarLinii(){
    char* line,set[10];
    string linie="";
    int i=100,j,k,tbox=0,length;
    line = new char[100];
    SendMessage(textbox,EM_SETSEL,0,100);
    //linie = " 1\n";
    while (SendMessage(textbox,EM_GETSELTEXT, 0, (LPARAM)line)){
        j = 0;
        while(line[j]){
            if (line[j] == '\r'){
                SendMessage(textbox,EM_SETSEL,j+i-100,j+i-100);
                k=SendMessage(textbox,EM_LINEFROMCHAR,-1,0);
                if(k == tbox && k > 1) k++;
                while(k>tbox){
                    linie += "\n";
                    tbox++;
                }
                cout<<k<<endl;
                length++;
                linie += " ";
                linie += itoa((tbox+1),set,10);
            }
            j++;
        }
        delete line;
        line = new char[100];
        SendMessage(textbox,EM_SETSEL,i,i+100);
        i=i+99;
    }
    SetWindowText(textbox2,linie.c_str());
    delete line;
}


void NumarLinii(){
    string text;
    char* line,set[10];
    int i,j=0,k=0,count,linie,n,cautare;
    count = SendMessage(textbox,EM_GETLINECOUNT,0,0);
    while(j<count){
        i = SendMessage(textbox,EM_LINELENGTH,k,0);
        linie = SendMessage(textbox,EM_LINEFROMCHAR, k, 0);
        line = new char[1000];
        memset(line, i, 2);
        n = SendMessage(textbox,EM_GETLINE,(WPARAM)linie,(LPARAM)line);
        line[n] = '\0';
        k+=i+1;

        itoa(linie,set,10);
        text += " ";
        text += set;
        text += "\n";
        j++;
        delete line;
    }
    SetWindowText(textbox2,text.c_str());
}



void SpecialWord(){
    char a[10] = "int";
    char linie[1000];
    memset(linie, 1000, sizeof(char));
    int i=10,capacity,line=0,length;
    RGAB b[6];
    DWORD sel;
    SendMessage(textbox,EM_GETSEL,(WPARAM)&sel,NULL);
    capacity = GetWindowTextLength(textbox);
    b[0].add(0,0,255);

    CHARFORMAT2 c;
    c.cbSize=sizeof(CHARFORMAT2A);
    c.dwMask=CFM_COLOR | CFM_BOLD;
    c.dwEffects=CFE_BOLD;

    CHARRANGE chr;
    chr.cpMin = sel;
    chr.cpMax = capacity;

    FINDTEXT f;
    f.chrg = chr;
       c.crTextColor=RGB(0,0,255);
       f.lpstrText = a;
       while(i<capacity && i>0){
            i = SendMessage(textbox,EM_FINDTEXT,FR_DOWN,(LPARAM)&f);
            line = SendMessage(textbox,EM_LINELENGTH,-1,0);
            length = SendMessage(textbox,EM_GETLINE,0,(LPARAM)linie);
            if (i>=0){
            chr.cpMin = i+1;
            f.chrg = chr;
            SendMessage(textbox,EM_SETSEL,i,i+strlen(a));
            SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
            }
        }
    SendMessage(textbox,EM_SETSEL,sel,sel);
}


/*
    case WM_KEYDOWN:
        switch(wParam){
        case VK_BACK:
            lin = GetWindowTextLength(textbox);
            q = limit(lin);
            if (q>salvare) q = q-2;
            if(q<25){
               ShowScrollBar(textbox,SB_VERT, FALSE);
            }
            break;
        }
        break;
    }*/


/*if (!limit(i)){
            ShowScrollBar(textbox,SB_VERT, TRUE);
        }
        SendMessage(textbox, EM_SETSEL, (WPARAM)i, (LPARAM)i);
        SendMessage(textbox,EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)a.c_str());
        i = GetWindowTextLength(textbox);
        SendMessage(textbox, EM_SETSEL, (WPARAM)i, (LPARAM)i);
        SendMessage(textbox,EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)"\r\n");*/


    /*if (limit(i)){
            ShowScrollBar(textbox,SB_VERT, TRUE);
    }*/


/*void write(char* file){
    char* t;
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long j = ftell(f);
    fseek(f, 0, SEEK_SET);
    t = new char[j];

    t[j-1] = '\0';
    SetWindowText(textbox,t);
    delete[] t;
    //t = new char[5000];
}
*/


void SpecialWord_After(){
    int i=10,capacity,line=0,j=0,length;
    DWORD sel;
    SendMessage(textbox,EM_GETSEL,(WPARAM)&sel,NULL);
    capacity = SendMessage(textbox,EM_LINELENGTH,-1,0);

    CHARFORMAT2 c;
    c.cbSize=sizeof(CHARFORMAT2A);
    c.dwMask=CFM_COLOR | CFM_BOLD;
    c.dwEffects=CFE_BOLD;
    CHARRANGE chr;
    chr.cpMin = LastSpace();
    chr.cpMax = sel + capacity;

    FINDTEXT f;
    f.chrg = chr;
       c.crTextColor=RGB(0,0,255);
       f.lpstrText = a[4];
       while(i<sel && i>0){
        i = SendMessage(textbox,EM_FINDTEXT,FR_DOWN | FR_WHOLEWORD ,(LPARAM)&f);
        if (i>=0){
            chr.cpMin = i+1;
            f.chrg = chr;
            SendMessage(textbox,EM_SETSEL,i,i+strlen(a[4]));
            SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
            }
         }
    SendMessage(textbox,EM_SETSEL,sel,sel);
    c.crTextColor=RGB(0,0,0);
    c.dwEffects=0;
    SendMessage(textbox, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&c);
    cout<<a[4];
}
