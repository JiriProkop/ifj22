CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g #-O2 na debug vypnu, also pribylo -g
LDLIBS = -lm

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
# scanner tests
	$(TESTDIR)tests_scanner < $(TESTDIR)tests_scanner.input > $(TESTDIR)tests_scanner.output
#	diff -su $(TESTDIR)tests_scanner_1.output $(TESTDIR)correct_out/tests_scanner_1.output

# $(TESTDIR)name_of_test_file: list.o of.o dependencies.o
$(TESTDIR)tests_dynstr: dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_scanner: scanner.o error.o dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
# --------------------------------------------------

# compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(PARTS) $(TESTS) $(TESTDIR)*.output
