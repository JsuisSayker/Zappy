##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

all:
	make all -C ZappyServer
	make all -C ZappyGUI

debug:
	make debug -C ZappyServer
	make debug -C ZappyGUI

clean:
	make clean -C ZappyServer
	make clean -C ZappyGUI

fclean: clean
	make fclean -C ZappyServer
	make fclean -C ZappyGUI

re:
	make re -C ZappyServer
	make re -C ZappyGUI

unit_tests:
	make unit_tests -C ZappyServer
	make unit_tests -C ZappyGUI

tests_run:
	make tests_run -C ZappyServer
	make tests_run -C ZappyGUI

.PHONY: all clean fclean re tests_run
