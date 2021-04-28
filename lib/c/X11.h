#ifndef JACK_X11_C_H
#define JACK_X11_C_H
#ifndef _WIN32
#ifndef __APPLE__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glu.h>

#ifdef JACK_IMPLEMENTATION

Display *display = NULL;
XImage *ximage;
int width = 512;
int height = 256;
Visual *visual;
Window window;
Window root;
GC gc;
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
XWindowAttributes gwa;
GLXFBConfig *fbc;
GLuint base;
Atom wm_del;

unsigned char *image32;
var consoleb[23 * 64] = {0};
var need_update = -1;
GLint att[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT, 
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	       	GLX_DOUBLEBUFFER, True,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		None};
GLXContext glc;
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
			q[3] = 0xFF; //alpha
		}
	}
	return 0;
}

/*XImage *CreateImage()
{
	image32 = (unsigned char*)malloc(width * height * sizeof(unsigned int));
	//return XCreateImage(display, visual, 
	//		DefaultDepth(display, DefaultScreen(display)),
	//
	return NULL;
}*/


void init() 
{
	XFontStruct *fontInfo;
	Font id;
	unsigned int first,last;
	int fbcount;
	if (display != NULL) {
		return;
	}
	display = XOpenDisplay(NULL);
	root = DefaultRootWindow(display);
	fbc = glXChooseFBConfig(display, DefaultScreen(display), att, &fbcount);
	if (fbc == NULL) {
		printf("\n Failed to get config.\n");
		return;
	}
	//vi = glXChooseVisual(display, 0, att);
	vi = glXGetVisualFromFBConfig(display, fbc[0]);
	if (vi == NULL) {
		printf("\n No GL visual found.\n");
		return;
	}
	//visual = XDefaultVisual(display, 0);
	cmap = XCreateColormap(display, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ButtonPressMask|ExposureMask|KeyPressMask;
	window = XCreateWindow(display, root, 
			0, 0, width, height, 0, vi->depth, InputOutput,
			vi->visual, CWColormap|CWEventMask, &swa);  

	XMapWindow(display, window);
	XStoreName(display, window, "JACK Application");

	glc = glXCreateContext(display, vi, NULL, GL_TRUE);
	glXMakeCurrent(display, window, glc);
	glEnable(GL_DEPTH_TEST);

	fontInfo = XLoadQueryFont(display, "fixed");
	id = fontInfo->fid;
	first = fontInfo->min_char_or_byte2;
	last = fontInfo->max_char_or_byte2;
	base = glGenLists(last + 1);
	glXUseXFont(id, first, last-first+1, base+first);
	image32 = (unsigned char*)malloc(width * height * 4);
	wm_del = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wm_del, 1);
	XMapWindow(display, window);	
}

void deInit()
{
	if (display != NULL) {
		glXMakeCurrent(display, None, NULL);
		glXDestroyContext(display, glc);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
		display = NULL;
		free(image32);
	}
}

void drawImage()
{
	XGetWindowAttributes(display, window, &gwa);
	glXMakeCurrent(display, window, glc);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2i(-1, -1);
	glViewport(0, 0, gwa.width, gwa.height);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image32);
}

var processEvent()
{
	XEvent ev;
	KeySym key;
	char text[255];
	int r;
	int x, y;
	var c;
	float fx, fy;

	fx = 8.0 * 2.0 / width;
	fy = 11.0 * 2.0 / height;

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
		drawImage();
		glColor4b(0x00, 0xFF, 0xFF, 0x00);
		for (y = 0; y < 23; y++) {
			for (x = 0; x < 64; x++) {
				c = consoleb[x + y * 64];
				if (c) {
					text[0] = (char)c;
					glRasterPos2f(x*fx-1.0, 1.0-(y+1)*fy);
					glListBase(base);
					glCallLists(1, GL_UNSIGNED_BYTE, text); 
				}
			}
		}
		glXSwapBuffers(display, window);		
		break;
	case ButtonPress:
		//printf("Click\n");	
		break;
	case ClientMessage:
		if (ev.xclient.data.l[0] == wm_del) {
			deInit();
			exit(0);
		}
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
	int l;	
	init();
	m = 16384;
	for (y = 0; y < height; y++) {
		for (x = 0; x < (width >> 4); x++) {
			p = Memory__peek(m + (y * (width>>4)) + x);
			l =  (height - y) * width + (x << 4);
			for (xx = 0; xx < 16; xx++) {
				if ((p >> xx) & 0x01) {
					 image32[(l+xx)*4] = 0x0;
					 image32[(l+xx)*4+1] = 0x0;
					 image32[(l+xx)*4+2] = 0x0;
					 image32[(l+xx)*4+3] = 0xFF;
				} else {
					 image32[(l+xx)*4] = 0xFF;
					 image32[(l+xx)*4+1] = 0xFF;
					 image32[(l+xx)*4+2] = 0xFF;
					 image32[(l+xx)*4+3] = 0xFF;
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

