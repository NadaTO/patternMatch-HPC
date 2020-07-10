HEADER_DIR=include

CC=mpicc
CFLAGS=-O3 -I$(HEADER_DIR)
LDFLAGS= 
SRC= mpi.c

OBJ= mpi.o

all: mpi



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

mpi:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f mpi $(OBJ)
