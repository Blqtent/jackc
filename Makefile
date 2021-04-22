
all: exe.py
	rm -rf jackc/lib
	cp -r lib jackc/
	rm -rf ctests/lib
	cp -r lib/std ctests/lib
	cp -r lib/ext/Callback.jack ctests/lib
	cp -r lib/ext/CallbackStringDispose.jack ctests/lib
	cp -r lib/ext/Buffer.jack ctests/lib
#	cp -r lib ctests/
	python3 jacky.py jackc 
	#python3 jackc.py tests
	python3 jackc.py jackc
	gcc -m64 -ggdb -static -Wall main_jack.c -o jack64
	gcc -m32 -ggdb -static -Wall main_jack.c -o jack32
	i686-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid main_jack.c -o jack32.exe 
	x86_64-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid main_jack.c -o jack64.exe 

hello:
	i686-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid hello.c -o hello32.exe 
	wine hello32.exe

test:
	rm -rf tests/lib
	cp -r lib tests/
	python3 jacky.py tests
	python3 tests.py

exe.py: jacky.py tree2py.py  exe/Main.jack
	rm -rf exe/lib
	cp -r lib exe/
	python3 jacky.py exe
clean:
	rm -f exe.py jackc.py
	rm -f test.txt
	rm -f a.out a.exe hello32.exe
	rm -f ctests/Main.jack.h
	rm -f jackc/Main.jack.h
	rm -f exe/Main.jack.h
	rm -f tests/*.jack.h
	rm -f main_jack.c
	rm -f jack32 jack64 jack32.exe jack64.exe
	rm -f test.xml
	rm -f tests.py
	rm -rf exe/lib
	rm -rf ctests/lib
	rm -rf tests/lib
	rm -rf jackc/lib
	rm -f *.pyc
	rm -rf __pycache__

