all: main4.o
	gcc -o program main4.o
main4.o: main4.c
	gcc -c main4.c
run:
	./program
clean:
	rm *o
	rm *~
