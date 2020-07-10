HEADER_DIR=include

CC=mpicc
CFLAGS=-O3 -I$(HEADER_DIR)
LDFLAGS= -fopenmp
SRC= hybrid_mpi.c

OBJ= hybrid_mpi.o

all: hybrid_mpi



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

hybrid_mpi:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f hybrid_mpi $(OBJ)
