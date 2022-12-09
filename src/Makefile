#
# Project: compiler of IFJ22 programming language
#
# File: Makefile
# Brief: The implementation of Makefile.
#
# Author: Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
#

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
LDLIBS = -lm

# the main program
compiler: scanner.o parser.o generator.o error.o expr.o symtable.o dynstr.o stack.o ll.o expr_codegen.o
	$(CC) $(CFLAGS) $^ main.c -o $@ $(LDLIBS)

# compile object files
%.o: $(SOURCES)%.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(SOURCES)*.o compiler
