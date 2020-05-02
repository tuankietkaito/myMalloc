all: main.c myyMalloc.c myMalloc.h
	gcc -o main main.c myMalloc.c

clean:
	rm -f main