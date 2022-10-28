CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g #-O2 na debug to vypnu, also pribylo -g

TESTDIR = tests/
CASES = tests_dynstr tests_scanner
TESTS = $(addprefix $(TESTDIR), $(CASES))

PARTS = $(TESTS)


all: $(PARTS)

# A BLOCK FOR TESTS --------------------------------
test: $(TESTS)
# dynstr tests
	$(TESTDIR)tests_dynstr > $(TESTDIR)tests_dynstr.output
	diff -su $(TESTDIR)tests_dynstr.output $(TESTDIR)correct_out/tests_dynstr.output

# $(TESTDIR)name_of_test_file: list.o of.o dependencies.o
$(TESTDIR)tests_dynstr: dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_scanner: scanner.o error.o dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
# --------------------------------------------------

# compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(PARTS) $(TESTS) $(TESTDIR)*.output
