##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

SRC			=	main.cpp	\

DIR_SRC		=	$(addprefix src/, $(SRC))

CFLAGS	=	-std=c++17 -O2

LDFLAGS	=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

NAME		=	zappy_gui

$(NAME): main.cpp
		g++ $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS)

all: $(NAME)

test: $(NAME)	
	./$(NAME)

clean:
	rm -f $(NAME)

.PHONY: test clean