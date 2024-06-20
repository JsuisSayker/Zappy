##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

include .env

SRC			=	Window.cpp \
				systems/PointLightSystem.cpp \
				systems/SimpleRenderSystem.cpp \
				Buffer.cpp \
				Camera.cpp \
				Client.cpp \
				Descriptors.cpp \
				Device.cpp \
				Egg.cpp \
				GameObject.cpp \
				KeyboardMovementController.cpp \
				Main.cpp \
				Map.cpp \
				Model.cpp \
				Pipeline.cpp \
				Renderer.cpp \
				SwapChain.cpp \
				Texture.cpp \
				Trantorian.cpp \
				ZappyGui.cpp \

DIR_SRC		=	$(addprefix src/, $(SRC))

OBJ_SRC		=	$(DIR_SRC:.cpp=.o)

OBJ			=	$(OBJ_SRC)

CPPFLAGS = -I./include -I./include/systems -I$(TINYOBJ_PATH) -I$(STBIMAGE_PATH) -I./imgui/
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

NAME = ../zappy_gui

$(NAME):	$(OBJ) $(vertObjFiles) $(fragObjFiles)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS) $(CPPFLAGS) imgui/*.cpp imgui/backends/*.cpp
	./rebuild.sh

all: $(NAME)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

test: $(NAME)
	./$(NAME)

clean:
	rm -rf shaders/*.spv $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: $(OBJ) $(vertObjFiles) $(fragObjFiles)
	g++ -o $(NAME) $(OBJ) $(CPPFLAGS) $(LDFLAGS) -ggdb3

.PHONY: test clean fclean re debug
