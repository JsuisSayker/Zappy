##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

include .env

CFLAGS = -std=c++17 -I./include -I./include/systems -I$(TINYOBJ_PATH) -I$(STBIMAGE_PATH) -I./imgui/
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = ../zappy_gui

$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*.cpp
	g++ $(CFLAGS) -o $(TARGET) src/*.cpp src/systems/*.cpp $(LDFLAGS) imgui/*.cpp imgui/backends/*.cpp

all: $(TARGET)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf shaders/*.spv

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

debug: $(vertObjFiles) $(fragObjFiles)
	g++ $(CFLAGS) -o $(TARGET) src/*.cpp src/systems/*.cpp $(LDFLAGS) imgui/*.cpp imgui/backends/*.cpp -ggdb3

.PHONY: test clean fclean re debug