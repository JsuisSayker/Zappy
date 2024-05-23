##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

all:
	make all -C ZappyServer
	make all -C ZappyAI

debug:
	make debug -C ZappyServer
	make debug -C ZappyAI

clean:
	make clean -C ZappyServer

fclean: clean
	make fclean -C ZappyServer
	make fclean -C ZappyAI

re:
	make re -C ZappyServer
	make re -C ZappyAI

unit_tests:
	make unit_tests -C ZappyServer

tests_run:
	make tests_run -C ZappyServer

.PHONY: all clean fclean re tests_run
