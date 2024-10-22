CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

.PHONY: all run clean gdb

all: dine

# making the dine rule
dine: dine.o
	$(CC) $(CFLAGS) -o $@ $^

# making the object files
dine.o: dine.c
	$(CC) $(CFLAGS) -c $< -o $@

# run domma
run: dine
	./dine

# Clean up generated files
clean:
	rm -rf *.o dine

# Debug with gdb
gdb: dine
	gdb ./dine
