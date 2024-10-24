CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread -lrt

.PHONY: all run clean gdb

all: dine

# making the dine rule
dine: dine.o print.o
	$(CC) $(CFLAGS) -o $@ $^

# making the object files
dine.o: dine.c dine.h print.h
	$(CC) $(CFLAGS) -c $< -o $@

print.o: print.c print.h
	$(CC) $(CFLAGS) -c $< -o $@

# run dine
run: dine
	./dine

# clean it all up
clean:
	rm -rf *.o dine

# for if i need gdb
gdb: dine
	gdb ./dine
