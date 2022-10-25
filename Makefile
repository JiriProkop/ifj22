CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
PARTS = tests_scanner

all: $(PARTS)

test: tests_scanner
	./tests_scanner

tests_scanner: tests_scanner.o scanner.o error.o
	$(CC) $(CFLAGS) tests_scanner.o scanner.o error.o -o tests_scanner

# compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(PARTS)
