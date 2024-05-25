##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

all:
	make all -C ZappyServer
	make all -C ZappyAI
	make all -C vulkanTuto

debug:
	make debug -C ZappyServer
	make debug -C ZappyAI

clean:
	make clean -C ZappyServer
	make clean -C vulkanTuto

fclean:
	make fclean -C ZappyServer
	make fclean -C ZappyAI
	make fclean -C vulkanTuto

re:
	make re -C ZappyServer
	make re -C ZappyAI
	make re -C vulkanTuto

unit_tests:
	make unit_tests -C ZappyServer

tests_run:
	make tests_run -C ZappyServer

.PHONY: all clean fclean re tests_run
