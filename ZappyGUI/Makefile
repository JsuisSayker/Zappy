include .env

CFLAGS = -std=c++17 -I./include -I$(VULKAN_SDK_PATH)/include -I$(TINYOBJ_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = ../zappy_gui

$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*.cpp
	g++ $(CFLAGS) -o $(TARGET) src/*.cpp $(LDFLAGS)

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

.PHONY: test clean fclean re