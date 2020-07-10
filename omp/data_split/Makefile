HEADER_DIR=include

CC=gcc
CFLAGS=-O3 -I$(HEADER_DIR) 
LDFLAGS= -fopenmp
SRC= data_split.c

OBJ= data_split.o

all: data_split



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

apm:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f apm $(OBJ)
