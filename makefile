all: main.c ex1.c ex1.h
	gcc -o main main.c ex1.c

clean:
	rm -f main