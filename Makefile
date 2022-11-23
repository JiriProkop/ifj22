CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g #-O2 na debug vypnu, also pribylo -g
LDLIBS = -lm

TESTDIR = tests/
CASES = tests_dynstr tests_stack
TESTS = $(addprefix $(TESTDIR), $(CASES))

PARTS = $(TESTS)


all: $(PARTS)

# A BLOCK FOR TESTS --------------------------------
test: $(TESTS)
# dynstr tests
	$(TESTDIR)tests_dynstr > $(TESTDIR)tests_dynstr.output
	diff -su $(TESTDIR)tests_dynstr.output $(TESTDIR)correct_out/tests_dynstr.output
# stack tests
	$(TESTDIR)tests_stack > $(TESTDIR)tests_stack.output
#	diff -su $(TESTDIR)tests_stack.output $(TESTDIR)correct_out/tests_stack.output

# check the tests with valgrind
valgrind: $(TESTS)
	valgrind $(TESTDIR)tests_dynstr
	valgrind $(TESTDIR)tests_scanner < $(TESTDIR)tests_scanner.input
	valgrind $(TESTDIR)tests_stack

# $(TESTDIR)name_of_test_file: list.o of.o dependencies.o
$(TESTDIR)tests_dynstr: dynstr.o error.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_scanner: scanner.o error.o dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
$(TESTDIR)tests_dynstr: stack.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_stack: stack.o error.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
# --------------------------------------------------


# compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(PARTS) $(TESTS) $(TESTDIR)*.output
