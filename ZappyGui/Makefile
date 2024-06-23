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
				Map.cpp \
				Model.cpp \
				Pipeline.cpp \
				Renderer.cpp \
				SwapChain.cpp \
				Texture.cpp \
				Trantorian.cpp \
				ZappyGui.cpp \

TEST			=	test_client.cpp	\
					test_hud.cpp	\

MAIN		=	Main.cpp

DIR_MAIN		=	$(addprefix src/, $(MAIN))

DIR_TESTS	=	$(addprefix tests/, $(TEST))

DIR_SRC		=	$(addprefix src/, $(SRC))

OBJ_SRC		=	$(DIR_SRC:.cpp=.o)

OBJ_MAIN	=	$(DIR_MAIN:.cpp=.o)

OBJ			=	$(OBJ_SRC) $(OBJ_MAIN)

CPPFLAGS = -I./include -I./include/systems -I$(TINYOBJ_PATH) -I$(STBIMAGE_PATH) -I./imgui/ -I./imgui/backends -ggdb3
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

NAME = ../zappy_gui

$(NAME):	$(OBJ) $(vertObjFiles) $(fragObjFiles)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS) $(CPPFLAGS) imgui/*.cpp imgui/backends/*.cpp -ggdb3

all: $(NAME)

documentation:
	./rebuild.sh

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

test: $(NAME)
	./$(NAME)

clean:
	rm -f shaders/*.spv
	rm -f $(OBJ)

fclean: clean
	rm -rf unit_tests
	rm -rf *.gcov
	rm -f $(NAME)

re: fclean all

debug: $(OBJ) $(vertObjFiles) $(fragObjFiles)
	g++ -o $(NAME) $(DIR_SRC) $(DIR_MAIN) $(CPPFLAGS) $(LDFLAGS) imgui/*.cpp imgui/backends/*.cpp

unit_tests: re $(DIR_SRC) $(DIR_TESTS)
	mkdir -p unit_tests
	g++ -o unit_tests/unit_tests $(DIR_SRC) $(DIR_TESTS) $(LDFLAGS) $(CPPFLAGS) imgui/*.cpp imgui/backends/*.cpp -I/usr/include/gtest -lgtest -lgtest_main -pthread -ggdb3

tests_run: unit_tests
	./unit_tests/unit_tests

.PHONY: test clean fclean re debug unit_tests tests_run documentation
