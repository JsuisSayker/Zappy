/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** zappy_server
*/

#ifndef ZAPPY_SERVER_H_
    #define ZAPPY_SERVER_H_

    #include <arpa/inet.h>
    #include <dirent.h>
    #include <limits.h>
    #include <netinet/in.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/queue.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <time.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include "macro_server.h"
    #include "server_struct.h"

char *direction_string(ai_direction_t orientation);

// char_tab functions
void free_char_tab_list(struct char_tab_head *head);
void display_char_tab_list(struct char_tab_head *head);
int count_char_tab_list(struct char_tab_head *head);
void random_char_tab_list(struct char_tab_head *head);

// args_config functions
args_config_t *init_args_config(void);
void display_args_config(args_config_t *args);
void free_args_config(args_config_t *args);
int fill_args_conf(args_config_t *args, int argc, char **argv);

// zappy_server functions
int zappy_server(args_config_t *args);
struct char_tab_head *generate_ressourse_list(int x, int y);

// map_tile functions
map_tile_t **generate_map_tile(int x, int y);
void free_map_tile(map_tile_t **map_tile);
void put_resource_on_map_tile(map_tile_t **map_tile,
    struct char_tab_head *head, int x, int y);
map_tile_t **setup_map_tile(int x, int y);
void display_tile(map_tile_t tile);
void display_gui_tile(map_tile_t *tile, int socket);
void display_map_tile(map_tile_t **map_tile);
int get_len_map_tile(map_tile_t **map_tile);
int get_len_line_map_tile(map_tile_t **map_tile);
map_tile_t **copy_map_tile(map_tile_t **source);


// int array functions
int **generate_int_array(int x, int y);
void free_int_array(int **possible_coordinate);
void shuffle_int_array(int **array, int n);

////////////////////////////////////////


void refill_map_tile(zappy_server_t *zappy, map_tile_t **destination,
    map_tile_t **source);

// Linked list functions
void free_teams(struct teamhead *head);
void free_node(message_t *node);
int remove_first_node(message_t *list);

//  TOOLS FUNCTION
bool is_valid_resource(char *resource);
void free_array(char **array);
void set_inventory_resource_quantite(inventory_t *tile_inventory,
    char *resource, int resource_quantity);
void normalize_coordinate(int *x, int *y, zappy_server_t *zappy);
void free_string(char **str);
void realloc_and_strcat(char **message, char *str);

// node
void free_node(message_t *node);
void free_list(message_t *list);
int remove_first_node(message_t *list);
message_t *create_node(char *cmd, ai_position_t pos);
message_t *add_node_in_list(message_t *list, char *cmd, ai_position_t pos);

// Server functions
int init_server(zappy_server_t *zappy, args_config_t *args);
int close_server(zappy_server_t *zappy);
int fd_is_set(zappy_server_t *zappy);
int scan_fd(zappy_server_t *zappy);
void handle_client(zappy_server_t *zappy);
int setup_server(int port, int max_clients);
char **splitter(char const *const str, char *separator);
void generate_random_uuid(char *buffer);
int accept_new_connection(int my_socket,
    struct sockaddr_in *client_socket_addr);
int setup_server(int port, int max_clients);
int save_info_to_file(zappy_server_t *zappy);
int read_info_from_save_file(zappy_server_t *zappy);
team_t *search_in_teams(struct teamhead *team_head, char *uuid);
int get_len_char_tab(char **command);
time_t get_actual_time(void);
int count_str_char(char *str, char c);
void generate_egg_by_team(zappy_server_t *zappy,
    team_t *new_team, int x, int y);
void refill_map(zappy_server_t *zappy);

// COMMANDS
typedef struct command_s {
    char *command;
    void (*func)(zappy_server_t *zappy, char *command);
} command_t;

int handle_unknown_command(zappy_server_t *zappy, char *command);
void send_mct_command_to_all_gui(zappy_server_t *zappy);

// SERVER COMMANDS FUNCTIONS
int handle_server_command(zappy_server_t *zappy, char *command);
void server_command_help(zappy_server_t *zappy, char *command);
void server_command_quit(zappy_server_t *zappy, char *command);
void server_command_map(zappy_server_t *zappy, char *command);
void server_command_clients(zappy_server_t *zappy, char *command);
void server_command_clear(zappy_server_t *zappy, char *command);
void server_command_tile(zappy_server_t *zappy, char *command);
void server_command_tp(zappy_server_t *zappy, char *command);
void server_command_set_tile(zappy_server_t *zappy, char *command);
void server_command_set_inventory(zappy_server_t *zappy, char *command);
void server_command_set_freq(zappy_server_t *zappy, char *command);
void server_command_set_level(zappy_server_t *zappy, char *command);
void server_command_send_guis(zappy_server_t *zappy, char *command);
void server_command_kill(zappy_server_t *zappy, char *command);
void server_command_fork(zappy_server_t *zappy, char *command);
void server_command_start(zappy_server_t *zappy, char *command);
void server_command_pause(zappy_server_t *zappy, char *command);

// AI FUNCTIONS

typedef struct list_command_ai_s {
    char *command;
    double cast_time;
    int (*func)(zappy_server_t *zappy, client_t *client, char *command);
    bool (*condition)(zappy_server_t *zappy, client_t *client);
} list_command_ai_t;

int ai_function(zappy_server_t *zappy, client_t *client, char *cmd);
int exec_command(zappy_server_t *zappy, client_t *client);
void add_in_queue(client_t *client, char *command, list_command_ai_t info_cmd,
    double freq);
int unqueue_command(client_t *client);
int handle_ai_command(zappy_server_t *zappy, client_t *client, char *command);
int ai_initialisation(zappy_server_t *zappy, client_t *ia,
    team_t *tmp_team);
void send_gui_map_content(map_tile_t **map, int x, int y, int socket);

//AI CONDITION COMMAND
bool incantation_condition(zappy_server_t *zappy, client_t *client);

// AI COMMANDS FUNCTIONS
int ai_command_breadcast(zappy_server_t *zappy, client_t *client,
    char *cmd);
int ai_command_set(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_forward(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_right(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_left(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_take_object(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_fork(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_inventory(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_look(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_connect_nbr(zappy_server_t *zappy, client_t *client,
    char *cmd);
int ai_command_incantation(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_eject(zappy_server_t *zappy, client_t *client, char *cmd);
bool is_alive(zappy_server_t *zappy, client_t *client);

int read_message_receive(zappy_server_t *zappy, client_t *client,
    message_t *message, int actual_sockfd);

void send_ppo_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_pin_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_pdi_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_pgt_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int nb_resource);
void send_pfk_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_pdr_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int nb_resource);
void send_pie_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int result);
void send_pic_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_plv_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_enw_command_to_all_gui(zappy_server_t *zappy, egg_t *egg);
void send_pbc_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    char *message);
void send_seg_command_to_all_gui(zappy_server_t *zappy, char *team_name);
void send_die_command_to_all_gui(zappy_server_t *zappy, int egg_number);
void send_pex_command_to_all_gui(zappy_server_t *zappy, int player_number);

// GUI COMMANDS FUNCTIONS
int handle_gui_command(zappy_server_t *zappy, char *command);
void gui_command_msz(zappy_server_t *zappy, UNUSED char *command);
void gui_command_bct(zappy_server_t *zappy, char *command);
void gui_command_mct(zappy_server_t *zappy, char *command);
void gui_command_tna(zappy_server_t *zappy, char *command);
void gui_command_ppo(zappy_server_t *zappy, char *command);
void gui_command_plv(zappy_server_t *zappy, char *command);
void gui_command_pin(zappy_server_t *zappy, char *command);
void gui_command_suc(zappy_server_t *zappy, char *command);
void gui_command_sgt(zappy_server_t *zappy, char *command);
void gui_command_sst(zappy_server_t *zappy, char *command);
void send_sgt_command_to_all_gui(zappy_server_t *zappy);
void send_sbp_command_to_all_gui(zappy_server_t *zappy);

#endif /* ZAPPY_SERVER_H_ */
