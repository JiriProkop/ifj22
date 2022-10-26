CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
PARTS = tests_scanner tests_dynstr

all: $(PARTS)

test: tests_dynstr tests_scanner
	./tests_dynstr
	./tests_scanner

tests_dynstr: tests_dynstr.o dynstr.o
	$(CC) $(CFLAGS) tests_dynstr.o dynstr.o -o tests_dynstr

tests_scanner: tests_scanner.o scanner.o error.o
	$(CC) $(CFLAGS) tests_scanner.o scanner.o error.o -o tests_scanner

# compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(PARTS)
