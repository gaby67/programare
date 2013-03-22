#include<X11/Xlib.h>
#include <X11/Xutil.h>
#include<string.h>

struct MwmHints {
    	unsigned long flags;
    	unsigned long functions;
    	unsigned long decorations;
    	long input_mode;
    	unsigned long status;
	};
	enum {
    	MWM_HINTS_FUNCTIONS = (1L << 0),
    	MWM_HINTS_DECORATIONS =  (1L << 1),
    	MWM_FUNC_ALL = (1L << 0),
    	MWM_FUNC_RESIZE = (1L << 1),
    	MWM_FUNC_MOVE = (1L << 2),
   		MWM_FUNC_MINIMIZE = (1L << 3),
    	MWM_FUNC_MAXIMIZE = (1L << 4),
    	MWM_FUNC_CLOSE = (1L << 5)
	};

Display* display;
Window Create_Window(int x, int y, int width, int height){
int screen_num;
	Window win;
	XEvent e;
	display = XOpenDisplay(NULL);
	screen_num = DefaultScreen(display);
	win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
							  x, y, width, height, 2,
							  BlackPixel(display,screen_num),
							  WhitePixel(display,screen_num));
	return win;
}

Window Create_Child(Window win, int x, int y, int width, int height){
int screen_num;
	Window child;
	XEvent e;
	display = XOpenDisplay(NULL);
	screen_num = DefaultScreen(display);
	child = XCreateSimpleWindow(display, win,
							  x, y, width, height, 2,
							  BlackPixel(display,screen_num),
							  WhitePixel(display,screen_num));
	return child;
}
void SetWindowTitle(Window win, const char* text){
    XTextProperty window_title_property;
	int rc = XStringListToTextProperty((char**)&text,
                                   1,
                                   &window_title_property);
	XSetWMName(display, win, &window_title_property);

}

void ShowWindow(Window win){
	XMapWindow(display, win);
	XSync(display, False);
	XEvent e;	
	while(1){
		if(e.type==ButtonPress) break;
	}	
}

void CloseWindow(){
	XCloseDisplay(display);
}

void SetWindowText(Window win,const char* msg){
	int screen_num;
	screen_num = DefaultScreen(display);
	XDrawString(display, win, DefaultGC(display, screen_num), 5, 15, msg, strlen(msg));
}

void Repaint_Window(Window win){
	XClearWindow(display,win);
}

void WindowEvents(Window win,long event_mask){
	XSelectInput(display, win, event_mask);	
}

void NextEvent(XEvent& e){
	XNextEvent(display, &e);		
}

void WindowBorder(Window win,bool k){
	if (k == false){
		Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", 0);
		struct MwmHints hints;
		hints.flags = MWM_HINTS_DECORATIONS;
		hints.decorations = 0;
		XChangeProperty(display, win, mwmHintsProperty, mwmHintsProperty, 32,	
        PropModeReplace, (unsigned char *)&hints, 5);
	}
}
