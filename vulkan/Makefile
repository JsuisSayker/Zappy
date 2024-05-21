##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Makefile
##

CFLAGS	=	-std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS	=	-L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

vulkan:	*.cpp *.hpp
	g++ $(CFLAGS) -o vulkan *.cpp $(LDFLAGS)

.PHONY:	test clean

test:	vulkan
	./vulkan

clean:
	rm -f vulkan
