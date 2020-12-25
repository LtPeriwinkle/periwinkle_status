OUTPUT = peri_status
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -std=c99 -I.
INCLUDES = $(wildcard *.h)

%.o: %.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^

PHONY: clean
clean:
	rm *.o $(OUTPUT)
