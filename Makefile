CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g #-O2 na debug vypnu, also pribylo -g
LDLIBS = -lm

TESTDIR = tests/
SOURCES = src/
CASES = tests_dynstr tests_scanner tests_parser tests_stack tests_generator
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
	diff -su $(TESTDIR)tests_scanner.output $(TESTDIR)correct_out/tests_scanner.output
# parser tests
	$(TESTDIR)tests_parser < $(TESTDIR)tests_parser.input
#	diff -su $(TESTDIR)tests_parser.output $(TESTDIR)correct_out/tests_parser.output
# stack tests
#	$(TESTDIR)tests_stack > $(TESTDIR)tests_stack.output
#	diff -su $(TESTDIR)tests_stack.output $(TESTDIR)correct_out/tests_stack.output
# expr tests
#	$(TESTDIR)tests_expr > $(TESTDIR)tests_expr.output < $(TESTDIR)tests_expr.input
#	diff -su $(TESTDIR)tests_expr.output $(TESTDIR)correct_out/tests_expr.output
# generator tests
	$(TESTDIR)tests_generator > $(TESTDIR)tests_generator.output
	diff -su $(TESTDIR)tests_generator.output $(TESTDIR)correct_out/tests_generator.output

# check the tests with valgrind
valgrind: $(TESTS)
	valgrind $(TESTDIR)tests_parser
	valgrind $(TESTDIR)tests_generator

# $(TESTDIR)name_of_test_file: list.o of.o dependencies.o
$(TESTDIR)tests_dynstr: $(SOURCES)dynstr.o $(SOURCES)error.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_scanner: $(SOURCES)scanner.o $(SOURCES)error.o $(SOURCES)dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
$(TESTDIR)tests_parser: $(SOURCES)parser.o $(SOURCES)scanner.o $(SOURCES)error.o $(SOURCES)dynstr.o $(SOURCES)expr.o $(SOURCES)stack.o $(SOURCES)symtable.o $(SOURCES)ll.o $(SOURCES)generator.o
	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
$(TESTDIR)tests_stack: $(SOURCES)stack.o $(SOURCES)error.o $(SOURCES)dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
#$(TESTDIR)tests_expr: $(SOURCES)expr.o $(SOURCES)parser.o $(SOURCES)scanner.o $(SOURCES)error.o $(SOURCES)dynstr.o $(SOURCES)stack.o $(SOURCES)symtable.o $(SOURCES)ll.o $(SOURCES)generator.o
#	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
$(TESTDIR)tests_ll: $(SOURCES)ll.o $(SOURCES)error.o $(SOURCES)dynstr.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_symtable: $(SOURCES)ll.o $(SOURCES)error.o $(SOURCES)dynstr.o $(SOURCES)symtable.o
	$(CC) $(CFLAGS) $^ $@.c -o $@
$(TESTDIR)tests_generator: $(SOURCES)dynstr.o $(SOURCES)ll.o $(SOURCES)error.o $(SOURCES)generator.o $(SOURCES)symtable.o $(SOURCES)parser.o $(SOURCES)scanner.o $(SOURCES)expr.o $(SOURCES)stack.o
	$(CC) $(CFLAGS) $^ $@.c -o $@ $(LDLIBS)
# --------------------------------------------------

# compile object files
%.o: $(SOURCES)%.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(SOURCES)*.o $(PARTS) $(TESTS) $(TESTDIR)*.output
