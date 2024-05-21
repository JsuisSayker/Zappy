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
				find_all_context.c					\
				get_user_by_uuid.c					\
				get_team_by_uuid.c					\
				get_thread_by_uuid.c				\
				linked_list/free_list.c				\
				linked_list/free_users.c			\
				argument_function.c					\
				generate_ressourse_list.c			\
				map_tile_function.c					\
				int_array_function.c				\
				${DIR_TOOLS}						\
				${DIR_COMMAND_IA}					\
				${DIR_COMMAND_GUI}

TOOLS		=	splitter.c						\
				free_array.c					\
				get_len_char_tab.c				\
				char_tab_list_function.c


COMMAND_IA		=	\

COMMAND_GUI		=	\

DIR_SRC		=	$(addprefix src/, $(SRC))

DIR_COMMAND_IA	=	$(addprefix commands_IA/, $(COMMAND_IA))

DIR_COMMAND_GUI	=	$(addprefix commands_GUI/, $(COMMAND_GUI))

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
