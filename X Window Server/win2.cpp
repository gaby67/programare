#include "Window.h"
int main(int argc, char* argv[]){
	Window win,child;
	win = Create_Window(10,10,300,300);
	SetWindowTitle(win,"Fereastra");
	WindowBorder(win,false);
	ShowWindow(win);
	CloseWindow();
	return 0;
}
