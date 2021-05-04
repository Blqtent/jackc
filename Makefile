
all:
	rm -rf jackc/lib
	#cp -r old jackc/lib
	cp -r lib jackc/
	gcc -o jack64.run jackc.c -lX11 -lGL -lGLU 
	./jack64.run jackc
	#gcc -m32 -ggdb -Wall jackc.c -lX11 -lGL -lGLU -o jack32
	gcc -m64 -ggdb -Wall -o jack64 jackc.c -lX11 -lGL -lGLU -lpthread
	#i686-w64-mingw32-gcc -g -municode  -static -lwsock32 -lopengl32 -lwinspool -lshell32 -luuid jackc.c -o jack32.exe 
	#x86_64-w64-mingw32-gcc -g -municode  -static -lwsock32 -lopengl32 -lwinspool -lshell32 -luuid jackc.c -o jack64.exe 

win:
	rm -rf jackc/lib
	cp -r lib jackc/
	./jack64.run jackc
	i686-w64-mingw32-gcc -g -municode  jackc.c -lgdi32 -lwsock32 -lopengl32 -lwinspool -lshell32 -luuid -o jack32.exe 
	x86_64-w64-mingw32-gcc -g -municode  jackc.c -lgdi32 -lwsock32 -lopengl32 -lwinspool -lshell32 -luuid -o jack64.exe 


release:
	mkdir -p jack-linux/
	rm -f jack-linux/*
	cp README.md jack-linux/
	cp LICENSE.txt jack-linux/
	cp jack64 jack-linux/
	cp jackc.c jack-linux/
	#cp jack32 jack-linux/
	tar -cvzf jack-linux.tar.gz jack-linux/

ckeck:
	echo ok.

py: exe.py
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
	gcc -m64 -ggdb -static -Wall -lX11 main_jack.c -o jack64
	gcc -m32 -ggdb -static -Wall -lX11 main_jack.c -o jack32
	i686-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid main_jack.c -o jack32.exe 
	x86_64-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid main_jack.c -o jack64.exe 
	cp jack64 jack64.run
hello:
	i686-w64-mingw32-gcc -static -lwsock32 -lwinspool -lshell32 -luuid hello.c -o hello32.exe 
	wine hello32.exe

test:
	rm -rf tests/lib
	cp -r lib tests/
	#./jack64.run tests
	#gcc -g tests.c -o tests.run -lX11 -lGL -lGLU
	#./tests.run
	./jack64 tests
	gcc -g tests.c -o tests.run -lX11 -lGL -lGLU
	./tests.run

exe.py: jacky.py tree2py.py  exe/Main.jack
	rm -rf exe/lib
	cp -r lib exe/
	python3 jacky.py exe
clean:
	rm -f exe.py jackc.py
	rm -f test.txt
	rm -f a.out a.exe hello32.exe
	rm -f ctests/Main.jack.h
	rm -f jackc.c
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

