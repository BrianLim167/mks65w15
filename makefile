all: semaphone.c
	gcc semaphone.c -o semaphone
run: all
	./semaphone $(flag) $(n)
clean:
	rm -f *~ *.o control client
wipe:
	rm story
