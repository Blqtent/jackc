// git clone git@github.com:jimon/osx_app_in_plain_c.git


#ifndef JACK_MAC_H
#define JACK_MAC_H
#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
#elif TARGET_OS_IPHONE
        // iOS device
#elif TARGET_OS_MAC
        // Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif

#ifdef __LP64__
#define EncodeNSInteger "q"
#define EncodeNSUInteger "L"
#else
#define EncodeNSInteger "i"
#define EncodeNSUInteger "I"
#endif
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGGeometry.h>
typedef CGPoint NSPoint;
typedef CGRect NSRect;
extern id NSApp;
extern id const NSDefaultRunLoopMode;

#ifdef __arm64__
#define obj_msgSend_stret obj_msgSend
#define obj_msgSend_fpret obj_msgSend
#endif

#ifdef JACK_IMPLEMENTATION

int width = 512;
int height = 256;
var refresh_ = 0;
var key = 0;
SEL allocSel;
SEL initSel;
GLuint tex = 1;
id pool;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		3,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image32);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-1,1, -1);
	glTexCoord2f(1,0); glVertex3f( 1,1, -1);
	glTexCoord2f(1,1); glVertex3f( 1, -1, -1);
	glTexCoord2f(0,1); glVertex3f(-1, -1, -1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
//	SwapBuffers(hDC);
}

void deInit()
{
	objc_msgSend(pool, s		id distantPast = objc_msgSend((id)NSDateClass, distantPastSel);
		id event = objc_msgSend(NSApp, nextEventMatchingMaskSel, NSUIntegerMax, distantPast, NSDefaultRunLoopMode, YES);

		if(event)
		{
			NSUInteger eventType = (NSUInteger)objc_msgSend(event, typeSel);

			switch(eventType)
			{
				//case NSMouseMoved:
				//case NSLeftMouseDragged:
				//case NSRightMouseDragged:
				//case NSOtherMouseDragged:
				case 5:
				case 6:
				case 7:
				case 27:
				{
					id currentWindow = objc_msgSend(NSApp, keyWindowSel);

					id currentWindowContentView = objc_msgSend(currentWindow, contentViewSel);
					NSRect adjustFrame = (NSRect)objc_msgSend_stret(currentWindowContentView, frameSel);

					NSPoint p = (NSPoint)objc_msgSend(currentWindow, mouseLocationOutsideOfEventStreamSel);

					if(p.x < 0) p.x = 0;
					else if(p.x > adjustFrame.size.width) p.x = adjustFrame.size.width;
					if(p.y < 0) p.y = 0;
					else if(p.y > adjustFrame.size.height) p.y = adjustFrame.size.height;

					NSRect r = {p.x, p.y, 0, 0};
					r = (NSRect)objc_msgSend_stret(currentWindowContentView, convertRectToBackingSel, r);
					p = r.origin;

					break;
				}
				//case NSLeftMouseDown:
				case 1:
	el_registerName("drain"));
}

NSUInteger applicationShouldTerminate(id self, SEL _sel, id sender)
{
	return 0;
}

void windowWillClose(id self, SEL _sel, id notification)
{
}

void init()
{
	allocSel = sel_registerName("alloc");
	initSel = sel_registerName("init");

	Class NSAutoreleasePoolClass = objc_getClass("NSAutoreleasePool");
	id poolAlloc = objc_msgSend((id)NSAutoreleasePoolClass, allocSel);
	pool = objc_msgSend(poolAlloc, initSel);

	Class NSApplicationClass = objc_getClass("NSApplication");
	SEL sharedApplicationSel = sel_registerName("sharedApplication");
	objc_msgSend((id)NSApplicationClass, sharedApplicationSel);

	SEL setActivationPolicySel = sel_registerName("setActivationPolicy:");
	objc_msgSend(NSApp, setActivationPolicySel, 0);

	Class NSObjectClass = objc_getClass("NSObject");
	Class AppDelegateClass = objc_allocateClassPair(NSObjectClass, "AppDelegate", 0);
	Protocol* NSApplicationDelegateProtocol = objc_getProtocol("NSApplicationDelegate");
	bool resultAddProtoc = class_addProtocol(AppDelegateClass, NSApplicationDelegateProtocol);
	SEL applicationShouldTerminateSel = sel_registerName("applicationShouldTerminate:");
	bool resultAddMethod = class_addMethod(AppDelegateClass, applicationShouldTerminateSel, (IMP)applicationShouldTerminate, EncodeNSUInteger "@:@");
	id dgAlloc = objc_msgSend((id)AppDelegateClass, allocSel);
	id dg = objc_msgSend(dgAlloc, initSel);

	SEL autoreleaseSel = sel_registerName("autorelease");
	objc_msgSend(dg, autoreleaseSel);

	SEL setDelegateSel = sel_registerName("setDelegate:");
	objc_msgSend(NSApp, setDelegateSel, dg);

	SEL finishLaunchingSel = sel_registerName("finishLaunching");
	objc_msgSend(NSApp, finishLaunchingSel);

	Class NSMenuClass = objc_getClass("NSMenu");
	id menubarAlloc = objc_msgSend((id)NSMenuClass, allocSel);
	id menubar = objc_msgSend(menubarAlloc, initSel);
	objc_msgSend(menubar, autoreleaseSel);

	Class NSMenuItemClass = objc_getClass("NSMenuItem");
	id appMenuItemAlloc = objc_msgSend((id)NSMenuItemClass, allocSel);
	id appMenuItem = objc_msgSend(appMenuItemAlloc, initSel);
	objc_msgSend(appMenuItem, autoreleaseSel);

	SEL addItemSel = sel_registerName("addItem:");
	objc_msgSend(menubar, addItemSel, appMenuItem);

	SEL setMainMenuSel = sel_registerName("setMainMenu:");
	objc_msgSend(NSApp, setMainMenuSel, menubar);

	id appMenuAlloc = objc_msgSend((id)NSMenuClass, allocSel);
	id appMenu = objc_msgSend(appMenuAlloc, initSel);
	objc_msgSend(appMenu, autoreleaseSel);

	Class NSProcessInfoClass = objc_getClass("NSProcessInfo");
	SEL processInfoSel = sel_registerName("processInfo");
	id processInfo = objc_msgSend((id)NSProcessInfoClass, processInfoSel);
	SEL processNameSel = sel_registerName("processName");
	id appName = objc_msgSend(processInfo, processNameSel);

	Class NSStringClass = objc_getClass("NSString");
	SEL stringWithUTF8StringSel = sel_registerName("stringWithUTF8String:");
	id quitTitlePrefixString = objc_msgSend((id)NSStringClass, stringWithUTF8StringSel, "Quit ");
	SEL stringByAppendingStringSel = sel_registerName("stringByAppendingString:");
	id quitTitle = objc_msgSend(quitTitlePrefixString, stringByAppendingStringSel, appName);

	id quitMenuItemKey = objc_msgSend((id)NSStringClass, stringWithUTF8StringSel, "q");
	id quitMenuItemAlloc = objc_msgSend((id)NSMenuItemClass, allocSel);
	SEL initWithTitleSel = sel_registerName("initWithTitle:action:keyEquivalent:");
	SEL terminateSel = sel_registerName("terminate:");
	id quitMenuItem = objc_msgSend(quitMenuItemAlloc, initWithTitleSel, quitTitle, terminateSel, quitMenuItemKey);
	objc_msgSend(quitMenuItem, autoreleaseSel);

	objc_msgSend(appMenu, addItemSel, quitMenuItem);

	SEL setSubmenuSel = sel_registerName("setSubmenu:");
	objc_msgSend(appMenuItem, setSubmenuSel, appMenu);

	NSRect rect = {{0, 0}, {512, 256}};
	Class NSWindowClass = objc_getClass("NSWindow");
	id windowAlloc = objc_msgSend((id)NSWindowClass, allocSel);
	SEL initWithContentRectSel = sel_registerName("initWithContentRect:styleMask:backing:defer:");
	id window = objc_msgSend(windowAlloc, initWithContentRectSel, rect, 15, 2, NO);
	objc_msgSend(window, autoreleaseSel);

	SEL setReleasedWhenClosedSel = sel_registerName("setReleasedWhenClosed:");
	objc_msgSend(window, setReleasedWhenClosedSel, NO);

	windowCount = 1;

	Class WindowDelegateClass = objc_allocateClassPair(NSObjectClass, "WindowDelegate", 0);
	Protocol* NSWindowDelegateProtocol = objc_getProtocol("NSWindowDelegate");
	resultAddProtoc = class_addProtocol(WindowDelegateClass, NSWindowDelegateProtocol);
	SEL windowWillCloseSel = sel_registerName("windowWillClose:");
	resultAddMethod = class_addMethod(WindowDelegateClass, windowWillCloseSel, (IMP)windowWillClose,  "v@:@");
	id wdgAlloc = objc_msgSend((id)WindowDelegateClass, allocSel);
	id wdg = objc_msgSend(wdgAlloc, initSel);
	objc_msgSend(wdg, autoreleaseSel);

	objc_msgSend(window, setDelegateSel, wdg);

	SEL contentViewSel = sel_registerName("contentView");
	id contentView = objc_msgSend(window, contentViewSel);

	SEL setWantsBestResolutionOpenGLSurfaceSel = sel_registerName("setWantsBestResolutionOpenGLSurface:");
	objc_msgSend(contentView, setWantsBestResolutionOpenGLSurfaceSel, YES);

	NSPoint point = {20, 20};
	SEL cascadeTopLeftFromPointSel = sel_registerName("cascadeTopLeftFromPoint:");
	objc_msgSend(window, cascadeTopLeftFromPointSel, point);

	id titleString = ((id (*)(Class, SEL, const char*))objc_msgSend)(NSStringClass, stringWithUTF8StringSel, "sup from C");
	SEL setTitleSel = sel_registerName("setTitle:");
	objc_msgSend(window, setTitleSel, titleString);

	uint32_t glAttributes[] =
	{
		8, 24,
		11, 8,
		5,
		73,
		72,
		55, 1,
		56, 4,
		99, 0x1000, // or 0x3200
		0
	};

	Class NSOpenGLPixelFormatClass = objc_getClass("NSOpenGLPixelFormat");
	id pixelFormatAlloc = objc_msgSend((id)NSOpenGLPixelFormatClass, allocSel);
	SEL initWithAttributesSel = sel_registerName("initWithAttributes:");
	id pixelFormat = objc_msgSend(pixelFormatAlloc, initWithAttributesSel, glAttributes);
	objc_msgSend(pixelFormat, autoreleaseSel);

	Class NSOpenGLContextClass = objc_getClass("NSOpenGLContext");
	id openGLContextAlloc = objc_msgSend((id)NSOpenGLContextClass, allocSel);
	SEL initWithFormatSel = sel_registerName("initWithFormat:shareContext:");
	id openGLContext = objc_msgSend(openGLContextAlloc, initWithFormatSel, pixelFormat, nil);
	objc_msgSend(openGLContext, autoreleaseSel);

	SEL setViewSel = sel_registerName("setView:");
	objc_msgSend(openGLContext, setViewSel, contentView);

	SEL makeKeyAndOrderFrontSel = sel_registerName("makeKeyAndOrderFront:");
	objc_msgSend(window, makeKeyAndOrderFrontSel, window);

	SEL setAcceptsMouseMovedEventsSel = sel_registerName("setAcceptsMouseMovedEvents:");
	objc_msgSend(window, setAcceptsMouseMovedEventsSel, YES);

	Class NSColorClass = objc_getClass("NSColor");
	id blackColor = ((id (*)(Class, SEL))objc_msgSend)(NSColorClass, sel_registerName("blackColor"));
	SEL setBackgroundColorSel = sel_registerName("setBackgroundColor:");
	objc_msgSend(window, setBackgroundColorSel, blackColor);

	SEL activateIgnoringOtherAppsSel = sel_registerName("activateIgnoringOtherApps:");
	objc_msgSend(NSApp, activateIgnoringOtherAppsSel, YES);


	Class NSDateClass = objc_getClass("NSDate");
	SEL distantPastSel = sel_registerName("distantPast");
	SEL nextEventMatchingMaskSel = sel_registerName("nextEventMatchingMask:untilDate:inMode:dequeue:");
	SEL frameSel = sel_registerName("frame");
	SEL typeSel = sel_registerName("type");
	SEL buttonNumberSel = sel_registerName("buttonNumber");
	SEL keyCodeSel = sel_registerName("keyCode");
	SEL keyWindowSel = sel_registerName("keyWindow");
	SEL mouseLocationOutsideOfEventStreamSel = sel_registerName("mouseLocationOutsideOfEventStream");
	SEL convertRectToBackingSel = sel_registerName("convertRectToBacking:");
	SEL scrollingDeltaXSel = sel_registerName("scrollingDeltaX");
	SEL scrollingDeltaYSel = sel_registerName("scrollingDeltaY");
	SEL hasPreciseScrollingDeltasSel = sel_registerName("hasPreciseScrollingDeltas");
	SEL modifierFlagsSel = sel_registerName("modifierFlags");
	SEL charactersSel = sel_registerName("characters");
	SEL UTF8StringSel = sel_registerName("UTF8String");
	SEL sendEventSel = sel_registerName("sendEvent:");
	SEL updateWindowsSel = sel_registerName("updateWindows");
	SEL updateSel = sel_registerName("update");
	SEL makeCurrentContextSel = sel_registerName("makeCurrentContext");
	SEL flushBufferSel = sel_registerName("flushBuffer");

void check_events()
{

	while(!terminated)
	{
		id distantPast = objc_msgSend((id)NSDateClass, distantPastSel);
		id event = objc_msgSend(NSApp, nextEventMatchingMaskSel, NSUIntegerMax, distantPast, NSDefaultRunLoopMode, YES);

		if(event)
		{
			NSUInteger eventType = (NSUInteger)objc_msgSend(event, typeSel);

			switch(eventType)
			{
				//case NSMouseMoved:
				//case NSLeftMouseDragged:
				//case NSRightMouseDragged:
				//case NSOtherMouseDragged:
				case 5:
				case 6:
				case 7:
				case 27:
				{
					id currentWindow = objc_msgSend(NSApp, keyWindowSel);

					id currentWindowContentView = objc_msgSend(currentWindow, contentViewSel);
					NSRect adjustFrame = (NSRect)objc_msgSend_stret(currentWindowContentView, frameSel);

					NSPoint p = (NSPoint)objc_msgSend(currentWindow, mouseLocationOutsideOfEventStreamSel);

					if(p.x < 0) p.x = 0;
					else if(p.x > adjustFrame.size.width) p.x = adjustFrame.size.width;
					if(p.y < 0) p.y = 0;
					else if(p.y > adjustFrame.size.height) p.y = adjustFrame.size.height;

					NSRect r = {p.x, p.y, 0, 0};
					r = (NSRect)objc_msgSend_stret(currentWindowContentView, convertRectToBackingSel, r);
					p = r.origin;

					break;
				}
				//case NSLeftMouseDown:
				case 1:
					break;
				//case NSLeftMouseUp:
				case 2:
					break;
				//case NSRightMouseDown:
				case 3:
					break;
				//case NSRightMouseUp:
				case 4:
					break;
				//case NSOtherMouseDown:
				case 25:
				{
					NSInteger number = (NSInteger)objc_msgSend(event, buttonNumberSel);
					break;
				}
				//case NSOtherMouseUp:
				case 26:
				{
					NSInteger number = (NSInteger)objc_msgSend(event, buttonNumberSel);
					break;
				}
				//case NSScrollWheel:
				case 22:
				{
					CGFloat deltaX = (CGFloat)objc_msgSend_fpret(event, scrollingDeltaXSel);

					CGFloat deltaY = (CGFloat)objc_msgSend_fpret(event, scrollingDeltaYSel);

					BOOL precisionScrolling = (BOOL)objc_msgSend(event, hasPreciseScrollingDeltasSel);

					if(precisionScrolling)
					{
						deltaX *= 0.1f; // similar to glfw
						deltaY *= 0.1f;
					}

					if(fabs(deltaX) > 0.0f || fabs(deltaY) > 0.0f)
					{
					}
					break;
				}
				//case NSFlagsChanged:
				case 12:
				{
					NSUInteger modifiers = (NSUInteger)objc_msgSend(event, modifierFlagsSel);

					struct
					{
						union
						{
							struct
							{
								uint8_t alpha_shift:1;
								uint8_t shift:1;
								uint8_t control:1;
								uint8_t alternate:1;
								uint8_t command:1;
								uint8_t numeric_pad:1;
								uint8_t help:1;
								uint8_t function:1;
							};
							uint8_t mask;
						};
					} keys;

					keys.mask = (modifiers & 0xffff0000UL) >> 16;

					break;
				}
				//case NSKeyDown:
				case 10:
				{
					id inputText = objc_msgSend(event, charactersSel);

					const char * inputTextUTF8 = (const char*)objc_msgSend(inputText, UTF8StringSel);

					uint16_t keyCode = (unsigned short)objc_msgSend(event, keyCodeSel);

					break;
				}
				//case NSKeyUp:
				case 11:
				{
					uint16_t keyCode = (unsigned short)objc_msgSend(event, keyCodeSel);
					break;
				}
				default:
					break;
			}

			objc_msgSend(NSApp, sendEventSel, event);

			if(terminated)
				break;

			objc_msgSend(NSApp, updateWindowsSel);
		}

		objc_msgSend(openGLContext, updateSel);

		objc_msgSend(openGLContext, makeCurrentContextSel);

		NSRect rect = (NSRect)objc_msgSend_stret(contentView, frameSel);

		rect = (NSRect)objc_msgSend_stret(contentView, convertRectToBackingSel, rect);

		glViewport(0, 0, rect.size.width, rect.size.height);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 0.85f, 0.35f);
		glBegin(GL_TRIANGLES);
		{
			glVertex3f(  0.0f,  0.6f, 0.0f);
			glVertex3f( -0.2f, -0.3f, 0.0f);
			glVertex3f(  0.2f, -0.3f ,0.0f);
		}
		glEnd();

		objc_msgSend(openGLContext, flushBufferSel);
	}
}

	return 0;

var Screen__refresh() 
{
	if (refresh_) return 0;
	refresh_ = -1;
	init();
	return 0;
}

var Screen__processEvents()
{
	var k = 0;
	static var in_proc = 0;
	if (in_proc) {
		return 0;
	}
	in_proc = -1;
	init();
	key = 0;
	check_events();
		if (key) {
			k = key;
		}
	Sys__wait(20);
	in_proc = 0;
	return k;
}


#endif // JACK_IMPLEMENTATION

#endif // __APPLE__
#endif // H

