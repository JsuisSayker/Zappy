##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

all:
	make all -C ZappyServer

debug:
	make debug -C ZappyServer

clean:
	make clean -C ZappyServer

fclean: clean
	make fclean -C ZappyServer

re:
	make re -C ZappyServer

unit_tests:
	make unit_tests -C ZappyServer

tests_run:
	make tests_run -C ZappyServer

.PHONY: all clean fclean re tests_run
