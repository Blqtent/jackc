#ifndef JACK_X11_C_H
#define JACK_X11_C_H
#ifndef _WIN32
#ifndef __APPLE__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#ifdef JACK_IMPLEMENTATION

Display *display = NULL;
XImage *ximage;
int width = 512;
int height = 256;
Visual *visual;
Window window;
GC gc;
unsigned char *image32;
var consoleb[23 * 64] = {0};
var need_update = -1;

void init();

var Screen__clear()
{
	var i, j;
	unsigned char *p;
	unsigned char *q;
	init();
	for (i = 0; i < (23 * 64); i++) {
		consoleb[i] = ' ';
	}
	for (i = 0; i < height; i++) {
		p = image32 + (i * width * 4);	
		for (j = 0; j < width; j++) {
			q = p + (j << 2);
			q[0] = 0xFF; //blue
			q[1] = 0xFF; //green
			q[2] = 0xFF; //red
			q[3] = 0; //alpha
		}
	}
	return 0;
}

XImage *CreateImage()
{
	image32 = (unsigned char*)malloc(width * height * 4);
	return XCreateImage(display, visual, 
			DefaultDepth(display, DefaultScreen(display)),
			ZPixmap, 0, (char*)image32, width, height, 32, 0);
}


void init() 
{
	if (display != NULL) {
		return;
	}
	display = XOpenDisplay(NULL);
	visual = XDefaultVisual(display, 0);
	window = XCreateSimpleWindow(display, RootWindow(display, 0), 
			0, 0, width, height, 1, 0, 0);  
	if (visual->class != TrueColor) {
		fprintf(stderr, "Error: Visual dislay must be true color.\n");
		return;
	}
	ximage = CreateImage();
	XSelectInput(display, window, 
			ButtonPressMask|ExposureMask|KeyPressMask);
	XMapWindow(display, window);	
}

void deInit()
{
	if (display != NULL) {
		XDestroyWindow(display, window);
		XCloseDisplay(display);
		display = NULL;
		free(image32);
	}
}

var processEvent()
{
	XEvent ev;
	KeySym key;
	char text[255];
	int r;
	int x, y;
	var c;

	XNextEvent(display, &ev);
	gc = DefaultGC(display, 0);
	switch (ev.type) {
	case KeyPress:
		r = XLookupString(&ev.xkey, text, 255, &key, 0);
		if (r == 1 && text[0]) {
			return text[0];
		}
		switch (key) {
		case XK_Left:
			return Keyboard__LEFT_ARROW();
		case XK_Right:
			return Keyboard__RIGHT_ARROW();
		case XK_Up:
			return Keyboard__UP_ARROW();
		case XK_Down:
			return Keyboard__DOWN_ARROW();
		case XK_Return:
			return Keyboard__NEWLINE();
		case XK_BackSpace:
			return Keyboard__BACKSPACE();
		case XK_Home:
			return Keyboard__HOME();
		case XK_Page_Up:
			return Keyboard__PAGE_UP();
		case XK_Page_Down:
			return Keyboard__PAGE_DOWN();
		case XK_Insert:
			return Keyboard__INSERT();
		case XK_Delete:
			return Keyboard__DELETE();
		case XK_Escape:
			return Keyboard__ESC();
		case XK_F12:
			return Keyboard__F12();
		}
		if (key >= XK_F1 && key <= XK_F11) {
			return (key - XK_F1) + Keyboard__F1();
		}	
		break;
	case Expose:
		XPutImage(display, window, gc, ximage,
					0, 0, 0, 0, width, height);
		XSetForeground(display, gc, 0);
		for (y = 0; y < 23; y++) {
			for (x = 0; x < 64; x++) {
				c = consoleb[x + y * 64];
				if (c) {
					text[0] = (char)c;
					XDrawString(display, window, gc, 
						x * 8, 11 + y * 11,text, 1); 
				}
			}
		}
		break;
	case ButtonPress:
		//printf("Click\n");	
		break;
	}
	return 0;
}

var refresh() 
{
	XEvent ev;
	int x, y, xx;
	var p;
	var m;
	init();
	m = 16384;

	for (y = 0; y < height; y++) {
		for (x = 0; x < (width >> 4); x++) {
			p = Memory__peek(m + (y * (width>>4)) + x);
			for (xx = 0; xx < 16; xx++) {
				if ((p >> xx) & 0x01) {
					XPutPixel(ximage, xx + (x << 4), y, 
							0x00000000);		
				} else {
					XPutPixel(ximage, xx + (x << 4), y, 
							0x00FFFFFF); 
				}
			}		
		}
	}
	memset(&ev, 0, sizeof(ev));
	ev.type = Expose;
	ev.xexpose.window = window;
	XSendEvent(display, window, True,ExposureMask, &ev);
	return 0;
}

var Screen__drawChar(var c, var x, var y) 
{
	init();
	need_update = -1;
	consoleb[x + (64 * y)] = c;
	return 0;
}

var Keyboard__screenKeyPressed()
{
	var e;
	init();
	if (Memory__screenNeedRefresh() || need_update) {
		refresh();
		need_update = 0;
	}
	while (XPending(display)) {
		e = processEvent();
		if (e) {
			Screen__clear();
			return e;
		}
	}
	return 0;
}


#endif // JACK_IMPLEMENTATION

#endif // __APPLE
#endif // _WIN32
#endif // H

