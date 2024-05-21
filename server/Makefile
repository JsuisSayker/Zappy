##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC			=	display_help.c 		\
				client.c 			\
				server.c 			\
				free.c 				\
				free_sub.c			\
				socket.c			\
				accept.c			\
				command.c			\
				init.c				\
				$(DIR_PARSER)		\
				${DIR_TOOLBOX}		\
				${DIR_COMMAND}		\
				${DIR_DATABASE}		\

TOOLBOX		=	generate_uuid.c 			\
				structure_initialisation.c	\
				append_to_string.c			\
				get_function.c				\
				user_is_exist.c				\
				client_is_connected.c		\
				tab_append_str_at_end.c		\
				send_user_not_found.c		\
				send_team_not_found.c		\
				send_channel_not_found.c	\
				send_thread_not_found.c

SRC_PARSER	=	login.c				\
				logout.c			\
				users.c				\
				user.c				\
				send.c				\
				messages.c			\
				subscribe.c			\
				subscribed.c		\
				unsubscribe.c		\
				use.c				\
				create.c			\
				list.c				\
				info.c				\
				help.c				\

COMMAND		=	find_command.c		\
				login_logout.c		\
				subscribe.c			\
				users_function.c	\
				message.c			\
				send.c				\
				use.c				\
				create.c			\
				create_response.c	\
				unsubscribe.c		\
				subscribed.c		\
				list.c				\
				info.c				\
				help.c				\

DATABASE	=	load_data.c		\
				save_data.c		\

MAIN_SRC	=	main.c

TEST		=	test.c

DIR_SRC			=	$(addprefix src/, $(SRC))
DIR_TESTS		=	$(addprefix tests/, $(TEST))
DIR_MAIN		=	$(addprefix src/, $(MAIN_SRC))
DIR_TOOLBOX		=	$(addprefix toolbox/, $(TOOLBOX))
DIR_COMMAND		=	$(addprefix command/, $(COMMAND))
DIR_PARSER		=	$(addprefix parser/, $(SRC_PARSER))
DIR_DATABASE	=	$(addprefix database/, $(DATABASE))

OBJ_SRC		=	$(DIR_SRC:.c=.o)
OBJ_MAIN	=	$(DIR_MAIN:.c=.o)
OBJ			=	$(OBJ_SRC) \
				$(OBJ_MAIN)

NAME		=	myteams_server

LDLIB		=	-L../lib/ -L../libs/myteams/

LMY			=	-lmy -lmyteams

CPPFLAGS	=	-I../include/ -I../lib/my/include/ -I../libs/myteams/

WFLAGS		=	-W -Wall -Wextra -Werror

LCRITER		= --coverage -lcriterion

$(NAME): $(OBJ)
	gcc -o $@ $^ $(WFLAGS) $(LDLIB) $(LMY)
	mv $(NAME) ../

debug:
	gcc -o $(NAME) $(DIR_MAIN) $(DIR_SRC) $(CPPFLAGS) $(LDLIB) $(LMY) -ggdb3
	mv $(NAME) ../

all: $(NAME)

clean:
	@ rm -f $(OBJ)

fclean: clean
	@ rm -f ../$(NAME)
	@ rm -rf unit_tests/

re: fclean $(NAME)

unit_tests: fclean
	mkdir unit_tests
	gcc -o unit_tests/unit_tests $(DIR_SRC) $(DIR_TESTS) $(LDLIB) $(LMY) \
	$(CPPFLAGS) $(WFLAGS) $(LCRITER)

tests_run:	unit_tests
	./unit_tests

.PHONY: all clean fclean re unit_tests tests_run
