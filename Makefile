CFLAGS = -g -Wall -Wextra
PROGRAM = ex3_lb
CC = gcc

$(PROGRAM): ex3_lb.o socket_handler.o string_utils.o memory_utils.o load_balancer.o
	$(CC) -o $@ $^ $(CFLAGS)

ex3_lb.o: ex3_lb.c load_balancer.h

socket_handler.o: socket_handler.c socket_handler.h string_utils.h memory_utils.h

string_utils.o: string_utils.c string_utils.h

memory_utils.o: memory_utils.c memory_utils.h

load_balancer.o: load_balancer.c load_balancer.h socket_handler.h

.PHONY: clean all test our_test

all: $(PROGRAM)
	chmod 755 *

clean:
	-rm *.o $(PROGRAM)