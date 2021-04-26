#ifndef JACK_WIN32_H
#define JACK_WIN32_H
#ifdef _WIN32

#ifdef _WIN64
#else
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

#endif // JACK:IMPLEMENTATION

#endif // _WIN32
#endif // H

