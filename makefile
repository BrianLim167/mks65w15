all: controller.c
	gcc controller.c -o controller
run: all
	./controller $(flag) $(n)
clean:
	rm -f *~
	rm -f *.o
