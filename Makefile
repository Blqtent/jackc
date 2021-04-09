
all: exe.py
	rm -rf jackc/lib
	cp -r lib jackc/
	rm -rf ctests/lib
	cp -r lib ctests/
	python3 jacky.py jackc 
	python3 jackc.py ctests


exe.py: jacky.py tree2py.py  exe/Main.jack
	rm -rf exe/lib
	cp -r lib exe/
	python3 jacky.py exe
clean:
	rm -f exe.py jackc.py
	rm -f test.txt
	rm -rf exe/lib
	rm -rf c/lib
	rm -f *.pyc
	rm -rf __pycache__

