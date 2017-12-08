all: semphone.c
	gcc semaphone.c -o semaphone
run: all
	./semaphone $(flag) $(n)
clean:
	rm -f *~
	rm -f *.o
