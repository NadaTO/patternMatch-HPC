HEADER_DIR=include

CC=gcc
CFLAGS=-O3 -I$(HEADER_DIR) -pg
LDFLAGS=
SRC= apm.c

OBJ= apm.o

all: apm



%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

apm:$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f apm $(OBJ)
