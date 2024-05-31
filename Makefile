##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

all:
	make all -C Server
	make all -C ZappyAI
	make all -C ZappyGui

debug:
	make debug -C Server
	# make debug -C ZappyAI

clean:
	make clean -C Server
	make clean -C ZappyGui

fclean:
	make fclean -C Server
	make fclean -C ZappyAI
	make fclean -C ZappyGui

re:
	make re -C Server
	make re -C ZappyAI
	make re -C ZappyGui

server:
	make -C ZappyServer

gui:
	make -C ZappyGui

ai:
	make -C ZappyAi

unit_tests:
	make unit_tests -C Server

tests_run:
	make tests_run -C Server

.PHONY: all clean fclean re tests_run
