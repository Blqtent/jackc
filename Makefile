
all: exe.py
	#python exe.py


exe.py: jacky.py tree2py.py  exe/main.jack
	python jacky.py exe
clean:
	rm -f exe.py
	rm -f *.pyc

