CFLAGS = -g -Wall -Wextra
PROGRAM = ex3_lb
CC = gcc
SOCKET_TESTS_FOLDER = /a/home/cc/students/csguests/nimrodav/socket_ex

$(PROGRAM): ex3_lb.o socket_handler.o lb_transfer.o string_utils.o memory_utils.o
	$(CC) -o $@ $^ $(CFLAGS)

ex3_lb.o: socket_handler.h lb_transfer.h

socket_handler.o: socket_handler.h string_utils.h memory_utils.h

lb_transfer.o: lb_transfer.h socket_handler.h

string_utils.o: string_utils.h

memory_utils.o: memory_utils.h

.PHONY: clean all test our_test

all: $(PROGRAM)
	chmod 755 *

clean:
	-rm *.o $(PROGRAM)

test:
	@$(SOCKET_TESTS_FOLDER)/test.sh