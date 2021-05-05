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
var Screen__need_refresh = -1;
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
var isfirst = -1;
var dump = 0;

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
	Screen__need_refresh = -1;
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
	swa.event_mask = ButtonPressMask|ExposureMask|KeyPressMask
		|KeyReleaseMask;
	window = XCreateWindow(display, root, 
			0, 0, width, height, 0, vi->depth, InputOutput,
			vi->visual, CWColormap|CWEventMask, &swa);  

	XMapWindow(display, window);
	XStoreName(display, window, "JACK Application");

	glc = glXCreateContext(display, vi, NULL, GL_TRUE);
	glXMakeCurrent(display, window, glc);
	//glEnable(GL_DEPTH_TEST);

	fontInfo = XLoadQueryFont(display, "fixed");
	id = fontInfo->fid;
	first = fontInfo->min_char_or_byte2;
	last = fontInfo->max_char_or_byte2;
	base = glGenLists(last + 1);
	glXUseXFont(id, first, last-first+1, base+first);
	wm_del = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wm_del, 1);
	XMapWindow(display, window);	
	glWindowPos2i(0, 0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glPixelZoom(1, -1);
	Screen__clear();
}

void deInit()
{
	if (display != NULL) {
		glXMakeCurrent(display, None, NULL);
		glXDestroyContext(display, glc);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
		display = NULL;
	}
}

void drawImage()
{
	XGetWindowAttributes(display, window, &gwa);
	glXMakeCurrent(display, window, glc);
	glDisable(GL_TEXTURE_2D);
	//glRasterPos2i(-1, -1);
	//glViewport(0, 0, gwa.width, gwa.height);
	//glViewport(0, 0, gwa.width, gwa.height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, width, height, 0, -1, 1);
	glRasterPos2i(0, 0);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image32);
	//glRasterPos2i(0, 11);
}

var dump_font(var c);

var processEvent()
{
	XEvent ev;
	KeySym key;
	char text[255];
	int r;
	int x, y;
	var c;
	float fx, fy;
	static int done = 0;
	fx = 8.0 * 2.0 / width;
	fy = 11.0 * 2.0 / height;

	XNextEvent(display, &ev);
	gc = DefaultGC(display, 0);
	switch (ev.type) {
	case KeyRelease:
		Memory__poke(24576, 0);
		return -1;
	case KeyPress:
		c = 0;
		r = XLookupString(&ev.xkey, text, 255, &key, 0);
		if (r == 1 && text[0]) {
			c = text[0];
		}
		switch (key) {
		case XK_Left:
			c = Keyboard__LEFT_ARROW();
			break;
		case XK_Right:
			c = Keyboard__RIGHT_ARROW();
			break;
		case XK_Up:
			c = Keyboard__UP_ARROW();
			break;
		case XK_Down:
			c = Keyboard__DOWN_ARROW();
			break;
		case XK_Return:
			c = Keyboard__NEWLINE();
			break;
		case XK_BackSpace:
			c = Keyboard__BACKSPACE();
			break;
		case XK_Home:
			c = Keyboard__HOME();
			break;
		case XK_Page_Up:
			c = Keyboard__PAGE_UP();
			break;
		case XK_Page_Down:
			c = Keyboard__PAGE_DOWN();
			break;
		case XK_Insert:
			c = Keyboard__INSERT();
			break;
		case XK_Delete:
			c = Keyboard__DELETE();
			break;
		case XK_Escape:
			c = Keyboard__ESC();
			break;
		case XK_F12:
			c = Keyboard__F12();
			break;
		}
		if (key >= XK_F1 && key <= XK_F11) {
			c = (key - XK_F1) + Keyboard__F1();
		}
		if (c) {	
			Memory__poke(24576, c);
			return c;
		}
		break;
	case Expose:
		for (c = 32; !done && c < 127; c++) {
			Screen__clear();
			drawImage();
			text[0] = c;
		glColor4b(0x00, 0xFF, 0xFF, 0x00);
			glRasterPos2f(0, 11);
			glListBase(base);
			glCallLists(1, GL_UNSIGNED_BYTE, text);
			glReadPixels(0, 0, width, height, GL_RGBA, 
				GL_UNSIGNED_BYTE, image32);
			dump_font(c);
		}	
		done = 1;
		drawImage();
		/*glColor4b(0x00, 0xFF, 0xFF, 0x00);
		for (y = 0; y < 23; y++) {
			for (x = 0; x < 64; x++) {
				c = consoleb[x + y * 64];
				if (c) {
					text[0] = (char)c;
					//glRasterPos2f(x*fx-1.0, 1.0-(y+1)*fy);
					glRasterPos2f(x * 8, (y+1) *11);
					glListBase(base);
					glCallLists(1, GL_UNSIGNED_BYTE, text); 
				}
			}
		}*/
		//glRasterPos2i(0, 0);
		//glReadPixels(0, 0, width, height, GL_RGBA, 
		//		GL_UNSIGNED_BYTE, image32);
		//dump_font(dump);
		glXSwapBuffers(display, window);	
		//dump_font(dump);
		isfirst = 0;	
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

var Screen__dumpFont(var c)
{
	dump = c;
}

var dump_font(var c)
{
	FILE *f;
	int x, y, xx, i, l;
	var p;
	if (!c) {
		return 0;
	}
	if (dump == c) {
	       dump = 0;
	}
	i = (c - 32) * 6;
	f = fopen("Font.jack", "a+w");
	fprintf(f, "\t\t// %c (%d)\n", c, c);
	for (y = 0; y < 11; y++) {
		x = 0;
		l =  (height - y - 1) * width + (x << 4);
		c = 0;
		for (xx = 0; xx < 8; xx++) {
			if (!image32[(l+xx)*4+1]) {
				 c = c | (1 << xx);
			}
		}
		y++;
		l =  (height - y - 1) * width + (x << 4);
		if (y < 11) {
			for (xx = 0; xx < 8; xx++) {
				if (!image32[(l+xx)*4+1]) {
					 c = c | (256 << xx);
				}
			}
		}
		if (c > 32767) {
			if (c = 32768) {
				fprintf(f, "\t\tlet v[%d] = 32767 + 1;\n", i);
			} else {
				fprintf(f, "\t\tlet v[%d] = -%d;\n", i,
					((~c) + 1) & 0x7FFF);
			}
		} else {
			fprintf(f, "\t\tlet v[%d] = %d;\n", i, c);
		}
		i++;
	}
	fclose(f);
	return 0;
}

var refresh() 
{
	XEvent ev;
	init();
	screen2rgba(width, height);
	memset(&ev, 0, sizeof(ev));
	ev.type = Expose;
	ev.xexpose.window = window;
	XSendEvent(display, window, True,ExposureMask, &ev);
	return 0;
}

/*
var Screen__drawChar(var c, var x, var y) 
{
	int j, l;
	var d;
	if (y > 22 || x > 63 || x < 0 || y < 0) {
				printf("GOH\n");
		return 0;
	}
	init();
	need_update = -1;
	d = consoleb[x + (64 * y)];
	consoleb[x + (64 * y)] = c;
#ifdef JACK_HACK
	if (d != ' ' || c != ' ') {
		for (j = 0; j < 11; j++) {
			l =  (((int)y * 11) + j) * width / 16 + (int)x/2;
			if (x & 0x1) {
				 Memory__memory[l+16384] &= 0x00FF;
			} else {
				 Memory__memory[l+16384] &= 0xFF00;
			}
			if (l+16384 > 24576) {
				printf("HHHH\n");
			}
		}
	}
#endif 
	return 0;
}
*/
/*
var Screen__putPixel(var index, var newvalue)
{
	var old;
	var x, y;
	need_update = -1;
	old = Memory__peek(index + 16384);
	x = (index % 32) * 2;
	y = (index / 32);
	if (y > 22) {
		return 0;
	}
	//if ((old & 0x00FF) != (newvalue & 0x00FF)) {
		Screen__drawChar(' ', x, y) ;
	//}
	//if ((old & 0xFF00) != (newvalue & 0xFF00)) {
		Screen__drawChar(' ', x+1, y) ;
	//}
	return 0;
}
*/

var Screen__refresh()
{
	Screen__need_refresh = -1;
	return 0;
}

var Screen__processEvents()
{
	static var inproc = 0;
	static var was_key = 0;
	var e, r;

	if (inproc) {
		return 0;
	}
	inproc = -1;
	init();
	if (Screen__need_refresh) {
		refresh();
		Screen__need_refresh = 0;
	}
	while (XPending(display)) {
		e = processEvent();
		if (e) {
			r = e;
		}
	}
	if (r) {
		if (was_key) {
			Sys__wait(30);
		}
		was_key = -1;
		inproc = 0;
		return r;
	}
	was_key = 0;
	Sys__wait(10);
	inproc = 0;
	return 0;
}


#endif // JACK_IMPLEMENTATION

#endif // __APPLE
#endif // _WIN32
#endif // H

