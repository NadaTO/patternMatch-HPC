HEADER_DIR=include

CC=gcc
CFLAGS=-O3 -I$(HEADER_DIR) -pg
LDFLAGS= -fopenmp
SRC= omp_combine.c

OBJ= omp_combine.o

all: omp_combine



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

omp_combine:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f omp_combine $(OBJ)
