<<<<<<< HEAD
all: semaphone.c
=======
all: semphone.c
>>>>>>> 848f693944dc8bf7e72858ba65f4885baa3e87dc
	gcc semaphone.c -o semaphone
run: all
	./semaphone $(flag) $(n)
clean:
	rm -f *~
	rm -f *.o
