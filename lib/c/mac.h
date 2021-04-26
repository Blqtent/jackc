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

#ifdef JACK_IMPLEMENTATION


var Screen__refresh() 
{

		return 0;
}

var Keyboard__screenKeyPressed()
{
	return 0;
}


#endif // JACK_IMPLEMENTATION

#endif // __APPLE__
#endif // H

