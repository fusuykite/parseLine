CC = clang
CFLAGS = -Wall -pedantic
LDFLAGS = 
DEBUG = -g

all : parseline

parseline: parseline.o parseFuncs.o
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(DEBUG) $(LDFLAGS) -c -o $@ $^

clean : 
	rm -f *.o core
