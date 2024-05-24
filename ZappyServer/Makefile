##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC			=	zappy_server.c						\
				handle_client.c						\
				scan_fd.c							\
				init_server.c						\
				generate_random_uuid.c				\
				accept_new_connection.c				\
				setup_server.c						\
				search_in_object.c					\
				get_team_by_uuid.c					\
				get_thread_by_uuid.c				\
				linked_list/free_list.c				\
				argument_function.c					\
				generate_ressourse_list.c			\
				map_tile_function.c					\
				map_tile_function_sub.c				\
				int_array_function.c				\
				close_server.c						\
				fd_is_set.c							\
				handle_unknown_command.c			\
				error_display.c						\
				ia_direction_function.c				\
				${DIR_TOOLS}						\
				${DIR_COMMAND_IA}					\
				${DIR_COMMAND_GUI}					\
				${DIR_COMMAND_SERVER}

TOOLS		=	splitter.c						\
				free_array.c					\
				get_len_char_tab.c				\
				char_tab_list_function.c


COMMAND_IA		=	cast_action.c			\
					handle_ia_command.c		\
					ia_command_help.c		\

COMMAND_GUI		=	handle_gui_command.c		\
					gui_command_msz.c			\
					gui_command_bct.c			\
					gui_command_mct.c			\
					gui_command_tna.c			\
					gui_command_ppo.c			\
					gui_command_plv.c			\
					gui_command_pin.c			\

COMMAND_SERVER	=	handle_server_command.c		\
					server_command_help.c		\
					server_command_quit.c		\
					server_command_map.c		\
					server_command_clients.c	\
					server_command_clear.c		\
					server_command_tile.c		\

DIR_SRC		=	$(addprefix src/, $(SRC))

DIR_COMMAND_IA	=	$(addprefix CommandsIa/, $(COMMAND_IA))

DIR_COMMAND_GUI	=	$(addprefix CommandsGui/, $(COMMAND_GUI))

DIR_COMMAND_SERVER	=	$(addprefix CommandsServer/, $(COMMAND_SERVER))

DIR_TOOLS	=	$(addprefix tools/, $(TOOLS))

MAIN		=	main.c						\

DIR_MAIN		=	$(addprefix src/, $(MAIN))

TEST		=	\

DIR_TESTS	=	$(addprefix tests/, $(TEST))

OBJ_DIR_SRC	=	$(DIR_SRC:.c=.o)

OBJ_MAIN	=	$(DIR_MAIN:.c=.o)

OBJ			=	$(OBJ_DIR_SRC) $(OBJ_MAIN)

CPPFLAGS	=	-I include/ \
				-I ../include/

LDLIBS		=	-luuid

CFLAGS		=	-Wall -Wextra

LDFLAGS		=

NAME		=	../zappy_server

$(NAME):	$(LIBS) $(OBJ)
		gcc -o $(NAME) $(OBJ) $(LDLIBS)

all: $(NAME)

debug:
		gcc -o $(NAME) $(DIR_SRC) $(DIR_MAIN) $(CPPFLAGS) $(LDLIBS) $(CFLAGS)\
			-ggdb3

clean:
		rm -f $(OBJ)

fclean: clean
		rm -rf unit_tests
		rm -rf *.gcov
		rm -f $(NAME)

re:		fclean all

unit_tests:	re
		mkdir unit_tests
		gcc -o unit_tests/unit_tests $(DIR_SRC) $(DIR_TESTS)\
		$(CPPFLAGS) $(LDLIBS) --coverage -lcriterion

tests_run:	unit_tests
		./unit_tests/unit_tests

.PHONY: all clean fclean re tests_run
