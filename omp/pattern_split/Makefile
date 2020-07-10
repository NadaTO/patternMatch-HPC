HEADER_DIR=include

CC=gcc
CFLAGS=-O3 -I$(HEADER_DIR)
LDFLAGS= -fopenmp
SRC= pattern_split.c

OBJ= pattern_split.o

all: pattern_split



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

apm:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f apm $(OBJ)
