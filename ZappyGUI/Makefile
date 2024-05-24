##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

SRC			=	Zappy.cpp			\
				Lve_window.cpp		\
				Lve_pipeline.cpp	\
				Lve_device.cpp		\
				Lve_swap_chain.cpp	\

MAIN			=	main.cpp

DIR_SRC		=	$(addprefix src/, $(SRC))

CFLAGS		=	-std=c++17 -O2

LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

NAME		=	../zappy_gui

OBJ_DIR_SRC	=	$(DIR_SRC:.cpp=.o)

OBJ_MAIN	=	$(MAIN:.cpp=.o)

OBJ			=	$(OBJ_DIR_SRC) $(OBJ_MAIN)

CPPFLAGS	=	-I./include

# shaders compilation
SHADER		=	simple_shader.vert	\
				simple_shader.frag	\

SHADER_DIR	=	$(addprefix shaders/, $(SHADER))

SHADER_OUT	=	$(SHADER_DIR:.vert=.vert.spv) $(SHADER_DIR:.frag=.frag.spv)

%.vert.spv: %.vert
	glslc $< -o $@

%.frag.spv: %.frag
	glslc $< -o $@

# Règle pour compiler les fichiers objets
%.o: %.cpp
	g++ $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Règle principale
$(NAME): $(OBJ) $(SHADER_OUT)
	g++ $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

all: $(NAME)

test: $(NAME)
	./$(NAME)

clean:
	rm -f $(OBJ)
	rm -f shaders/*.spv

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: test clean re all fclean
