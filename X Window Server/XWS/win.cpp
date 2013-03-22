
#include "Window.h"
GC pen;
int main(int argc, char* argv[]){
	XFontStruct *font;
	/*Button* b = NULL:

	font = XLoadQueryFont(display, "fixed");

	button->font_ascent = font->ascent;
	button->text = malloc(sizeof(*button->text)*(strlength+1));
	button->buttonRelease = callback;
	button->cbdata = cbdata;
	button->width = 100;
	button->height = 20;
	button->backgroud = backgroud
	button->foreground = foreground;
	button->border = border;*/

		

	Window win,child;
	win = Create_Window(10,10,300,300);
	Create_Pen(win,pen);	
	SetWindowTitle(win,"Fereastra");
	ShowWindow(win);
	SetWindowText(win,"mumata e pe val");
	DrawLine(win,10,60,180,20);
	DrawLine(win,10,10,100,100);
	
	sleep(10);
}

