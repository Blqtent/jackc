
all: exe.py
	#python exe.py


exe.py: jacky.py tree2py.py  exe/Main.jack
	python3 jacky.py exe
clean:
	rm -f exe.py
	rm -f *.pyc
	rm -rf __pycache__

