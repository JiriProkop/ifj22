CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g #-O2 na debug vypnu, also pribylo -g
LDLIBS = -lm

TESTDIR = tests/
SOURCES = src/
CASES = tests_generator
TESTS = $(addprefix $(TESTDIR), $(CASES))

PARTS = $(TESTS)


all: $(PARTS)

# A BLOCK FOR TESTS --------------------------------
test: $(TESTS)
# dynstr tests
#	$(TESTDIR)tests_dynstr > $(TESTDIR)tests_dynstr.output
#	diff -su $(TESTDIR)tests_dynstr.output $(TESTDIR)correct_out/tests_dynstr.output
# scanner tests
#	$(TESTDIR)tests_scanner < $(TESTDIR)tests_scanner.input > $(TESTDIR)tests_scanner.output
#	diff -su $(TESTDIR)tests_scanner.output $(TESTDIR)correct_out/tests_scanner.output
# parser tests
#	$(TESTDIR)tests_parser < $(TESTDIR)tests_parser.input
#   diff -su $(TESTDIR)tests_parser.output $(TESTDIR)correct_out/tests_parser.output
# stack tests
	$(TESTDIR)tests_generator
#	diff -su $(TESTDIR)tests_stack.output $(TESTDIR)correct_out/tests_stack.output

# check the tests with valgrind
valgrind: $(TESTS)
	valgrind $(TESTDIR)tests_generator

# $(TESTDIR)name_of_test_file: list.o of.o dependencies.o
$(TESTDIR)tests_generator: $(SOURCES)generator.o $(SOURCES)ll.o $(SOURCES)dynstr.o $(SOURCES)error.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
# --------------------------------------------------

# compile object files
%.o: $(SOURCES)%.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(SOURCES)*.o $(PARTS) $(TESTS) $(TESTDIR)*.output
