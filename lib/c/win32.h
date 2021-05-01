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

var Screen__clear() 
{
		return 0;
}

var Screen__drawChar(var c, int x, int y) 
{
		return 0;
}

var Screen__putPixel(var offset, var value)
{
	return 0;
}

var Screen__processEvents()
{
	return 0;
}

#endif // JACK:IMPLEMENTATION

#endif // _WIN32
#endif // H

