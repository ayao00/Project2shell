all: main3.o
	gcc -o program main3.o
main3.o: main3.c
	gcc -c main3.c
run:
	./program
clean:
	rm *o
	rm *~
