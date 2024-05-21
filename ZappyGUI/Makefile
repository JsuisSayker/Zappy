##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

SRC			=	Zappy.cpp	\
				Lve_window.cpp	\

MAIN 		=	main.cpp

DIR_SRC		=	$(addprefix src/, $(SRC))

CFLAGS		=	-std=c++17 -O2

LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

NAME		=	zappy_gui

OBJ_DIR_SRC	=	$(DIR_SRC:.cpp=.o)

OBJ_MAIN	=	$(MAIN:.cpp=.o)

OBJ			=	$(OBJ_DIR_SRC) $(OBJ_MAIN)

CPPFLAGS	=	-I./include

$(NAME): $(OBJ)
		g++ $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

all: $(NAME)

test: $(NAME)	
	./$(NAME)

clean:
	rm -f $(NAME)
	rm -f $(OBJ)

re: clean all

.PHONY: test clean re all