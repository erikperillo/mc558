NAME = lab_2
OBJ = $(NAME).o
CC = g++
DEPS = 
CFLAGS = -ansi -Wall -pedantic -lm

.PHONY: clean all

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(OBJ)
