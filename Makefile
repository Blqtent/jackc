
all: exe.py
	#python exe.py


exe.py: jacky.py tree2py.py  exe/Main.jack
	rm -rf exe/lib
	cp -r lib exe/
	python3 jacky.py exe
clean:
	rm -f exe.py
	rm -rf exe/lib
	rm -f *.pyc
	rm -rf __pycache__

