CFLAGS = -g -Wall -Wextra
PROGRAM = ex3_lb
CC = gcc
GREP_TESTS_FOLDER = /a/home/cc/students/csguests/nimrodav/socket_ex

$(PROGRAM): ex3_lb.o socket_handler.o
	$(CC) -o $@ $^ $(CFLAGS)

ex3_lb.o: socket_handler.h

socket_handler.o: socket_handler.h

.PHONY: clean all test our_test

all: $(PROGRAM)
	chmod 755 *

clean:
	-rm *.o $(PROGRAM)

test:
	@$(GREP_TESTS_FOLDER)/test.sh