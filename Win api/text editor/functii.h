#ifndef functii_H
#define functii_H
extern LRESULT CALLBACK WndProc(HWND hWnd , UINT massage, WPARAM wParam , LPARAM lParam);
extern BOOL CALLBACK EnumChildProc(HWND handle,LPARAM lParam);
extern BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);
extern LRESULT CALLBACK SubClassProc(HWND phWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK Prop_proc(HWND hWnd , UINT message, WPARAM wParam , LPARAM lParam);
extern char* OpenFile(HWND hWnd);
extern void write(char* file);
extern int limit(int n);
extern void check();
extern void size(int width, int height);
extern void checkc();
extern void checkb();
extern void checks(HWND check_size);
extern LRESULT CALLBACK SubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK Prop_proc(HWND hWnd , UINT message, WPARAM wParam , LPARAM lParam);
extern void Text_Scroll();
extern void NumarLinii(HWND&,HWND&);
extern void SpecialWord_After();
extern void SpecialWord(char* a,COLORREF,int da=0);
extern char** GetSpecial();
extern void Salvare();
char* SaveFile(HWND hWnd);
extern int marime;
extern char **a;
int comparare(char*,char*);
#endif
