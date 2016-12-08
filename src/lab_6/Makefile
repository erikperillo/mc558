NAME = lab_6
PREFIX=~/random/lemon

all: $(NAME)

$(NAME): $(NAME).cpp
	g++ -I $(PREFIX)/include/ -L $(PREFIX)/lib/ -std=c++11 -o $(NAME) $(NAME).cpp -lemon -lglpk
