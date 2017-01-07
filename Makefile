OBJECTS = main.o brainfuck.o
CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Wextra

bfi: $(OBJECTS)
	gcc $(OBJECTS) -o bfi


run: bfi
	@./bfi

memcheck: bfi
	valgrind --leak-check=full bfi

clean:
	@rm -rf *.o bfi

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

