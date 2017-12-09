all: control.c client.c
	gcc control.c -o control
	gcc client.c -o client
clean:
	rm -f *~ *.o control client *# *.#
wipe:
	rm story
