all: main.c myMalloc.c myMalloc.h
	gcc -o main main.c myMalloc.c

clean:
	rm -f main