all: control.c
	gcc control.c -o control
clean:
	rm -f *~ *.o control client
wipe:
	rm story
