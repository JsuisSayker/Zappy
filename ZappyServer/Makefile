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
				argument_function.c					\
				generate_ressourse_list.c			\
				map_tile_function.c					\
				map_tile_function_sub.c				\
				map_tile_function_ter.c				\
				int_array_function.c				\
				close_server.c						\
				fd_is_set.c							\
				handle_unknown_command.c			\
				ia_direction_function.c				\
				generate_egg_by_team.c				\
				send_mct_command_to_all_gui.c		\
				refill_map.c						\
				${DIR_TOOLS}						\
				${DIR_COMMAND_GUI}					\
				${DIR_COMMAND_SERVER}				\
				${DIR_AI}							\

TOOLS		=	splitter.c							\
				free_array.c						\
				get_len_char_tab.c					\
				char_tab_list_function.c			\
				is_valid_resource.c					\
				set_inventory_resource_quantite.c	\
				free_list.c							\
				normalize_coordonate.c				\
				free_string.c						\
				realloc_and_strcat.c				\
				nb_resource_tile.c					\

AI			=	ai_function.c						\
				ai_initialisation.c					\
				ai_queue.c							\
				ai_handle_command.c					\
				ai_execution_command.c				\
				${DIR_COMMAND_AI}


COMMAND_AI		=	ai_command_forward.c			\
					ai_command_left.c				\
					ai_command_right.c				\
					ai_command_take_object.c		\
					ai_command_fork.c				\
					ai_command_inventory.c			\
					ai_command_look.c				\
					ai_command_connect_nbr.c		\
					ai_command_set.c				\
					ai_command_incantation.c		\
					ai_command_breadcast.c			\
					ai_command_eject.c				\
					ai_receive_breadcast.c			\
					send_ppo_command_to_all_gui.c	\
					send_pin_command_to_all_gui.c	\
					send_pdi_command_to_all_gui.c	\
					send_pgt_command_to_all_gui.c	\
					send_pfk_command_to_all_gui.c	\
					send_pdr_command_to_all_gui.c	\
					send_pie_command_to_all_gui.c	\
					send_pic_command_to_all_gui.c	\
					send_plv_command_to_all_gui.c	\
					send_pbc_command_to_all_gui.c	\
					send_seg_command_to_all_gui.c	\
					send_die_command_to_all_gui.c	\
					send_pex_command_to_all_gui.c	\

COMMAND_GUI		=	handle_gui_command.c			\
					gui_command_msz.c				\
					gui_command_bct.c				\
					gui_command_mct.c				\
					gui_command_tna.c				\
					gui_command_ppo.c				\
					gui_command_plv.c				\
					gui_command_pin.c				\
					gui_command_suc.c				\
					gui_command_sgt.c				\
					gui_command_sst.c				\
					gui_command_gui_disconnected.c	\
					send_sgt_command_to_all_gui.c	\
					send_sbp_command_to_all_gui.c	\
					send_enw_command_to_all_gui.c	\

COMMAND_SERVER	=	handle_server_command.c			\
					server_command_help.c			\
					server_command_quit.c			\
					server_command_map.c			\
					server_command_clients.c		\
					server_command_clear.c			\
					server_command_tile.c			\
					server_command_tp.c				\
					server_command_set_tile.c		\
					server_command_set_inventory.c	\
					server_command_set_freq.c		\
					server_command_set_level.c		\
					server_command_send_guis.c		\
					server_command_kill.c			\
					server_command_fork.c			\
					server_command_start.c			\
					server_command_pause.c			\

DIR_SRC		=	$(addprefix src/, $(SRC))

DIR_COMMAND_AI	=	$(addprefix CommandsAi/, $(COMMAND_AI))

DIR_AI =	$(addprefix Ai/, $(AI))

DIR_COMMAND_GUI	=	$(addprefix CommandsGui/, $(COMMAND_GUI))

DIR_COMMAND_SERVER	=	$(addprefix CommandsServer/, $(COMMAND_SERVER))

DIR_TOOLS	=	$(addprefix tools/, $(TOOLS))

MAIN		=	main.c								\

DIR_MAIN		=	$(addprefix src/, $(MAIN))

TEST		=	test_splitter.c						\
				test_get_len_char_tab.c				\
				test_server_command_help.c			\
				test_server_command_map.c			\
				test_server_command_clear.c			\
				test_server_command_quit.c			\
				test_server_command_set_freq.c		\
				test_server_command_tile.c			\
				test_handle_server_command.c		\
				test_gui_command_mct.c				\
				test_gui_command_pin.c				\
				test_gui_command_ppo.c				\
				test_gui_command_suc.c				\
				test_zappy_server.c					\
				test_gui_command_bct.c				\
				test_gui_command_msz.c				\
				test_gui_command_plv.c				\
				test_gui_command_sgt.c				\
				test_gui_command_sst.c				\
				test_gui_command_tna.c				\
				test_fill_args_conf.c				\

DIR_TESTS	=	$(addprefix tests/, $(TEST))

OBJ_DIR_SRC	=	$(DIR_SRC:.c=.o)

OBJ_MAIN	=	$(DIR_MAIN:.c=.o)

OBJ			=	$(OBJ_DIR_SRC) $(OBJ_MAIN)

CPPFLAGS	=	-I include/			\
				-I ../include/		\

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
		@ rm -f $(OBJ)

fclean: clean
		@ rm -rf unit_tests
		@ rm -rf *.gcov
		@ rm -f $(NAME)

re:		fclean all

unit_tests:	re
		mkdir -p unit_tests
		gcc -o unit_tests/unit_tests $(DIR_SRC) $(DIR_TESTS)\
		$(CPPFLAGS) $(LDLIBS) --coverage -lcriterion

tests_run:	unit_tests
		./unit_tests/unit_tests

.PHONY: all clean fclean re tests_run
