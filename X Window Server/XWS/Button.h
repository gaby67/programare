typedef void (*Callback)(void *cbdata);
typedef struct Button Button;
struct Button {
	XChar2b * text;
	int text_width;
	int font_ascent;
	int width, height;
	unsigned long border, background, foreground;
	void *cbdata;
	Callback buttonRelease;
};

/*void buttonExpose(Button *button, XEvent *ev) {
	int textx, texty, len;
	if (!button) return;
	if (button->text){
		len = XChar2bLen(button->text);
   		textx = (button->width - button->text_width)/2;
   		texty = (button->height + button->font_ascent)/2;
   		XDrawString16(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), textx, texty,button->text, len);
	} else {  /* if there's no text draw the big X
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), 0, 0, button->width, button->height);
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), button->width, 0, 0, button->height);
	}
}*/

