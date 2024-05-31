/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** zappy_server
*/

#ifndef ZAPPY_SERVER_H_
    #define ZAPPY_SERVER_H_

    #include <zappy.h>
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
    #include <unistd.h>
    #include "macro_server.h"

typedef enum client_type_s {
    IA,
    GUI,
    UNKNOWN,
}client_type_t;

typedef enum ai_direction_s {
    NORTH,
    EAST,
    SOUTH,
    WEST,
} ai_direction_t;

char *direction_string(ai_direction_t orientation);

typedef struct char_tab_s {
    char *str;
    TAILQ_ENTRY(char_tab_s) next;
} char_tab_t;

struct char_tab_head {
    struct char_tab_s *tqh_first;
    struct char_tab_s **tqh_last;
};

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

// char_tab functions
void free_char_tab_list(struct char_tab_head *head);
void display_char_tab_list(struct char_tab_head *head);
int count_char_tab_list(struct char_tab_head *head);
void random_char_tab_list(struct char_tab_head *head);

typedef struct args_config_s {
    int port;
    int width;
    int height;
    int clientsNb;
    int freq;
    struct char_tab_head names;
} args_config_t;

typedef struct map_tile_s {
    int x;
    int y;
    inventory_t inventory;
} map_tile_t;

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
void display_gui_tile(map_tile_t tile, int socket);
void display_map_tile(map_tile_t **map_tile);
int get_len_map_tile(map_tile_t **map_tile);
int get_len_line_map_tile(map_tile_t **map_tile);

// int array functions
int **generate_int_array(int x, int y);
void free_int_array(int **possible_coordinate);
void shuffle_int_array(int **array, int n);


////////////////////////////////////////

typedef struct reply_s {
    char text[MAX_BODY_LENGTH];
    char reply_uuid[MAX_UUID_LENGTH];
    char sender_uuid[MAX_UUID_LENGTH];
    char thread_uuid[MAX_UUID_LENGTH];
    time_t timestamp;
    TAILQ_ENTRY(reply_s) next;
} reply_t;

struct replyhead {
    struct reply_s *tqh_first;
    struct reply_s **tqh_last;
};

typedef struct thread_s {
    char title[MAX_NAME_LENGTH];
    char body[MAX_DESCRIPTION_LENGTH];
    char thread_uuid[MAX_UUID_LENGTH];
    char channel_uuid[MAX_UUID_LENGTH];
    char sender_uuid[MAX_UUID_LENGTH];
    time_t timestamp;
    struct replyhead replys_head;
    TAILQ_ENTRY(thread_s) next;
} thread_t;

struct threadhead {
    struct thread_s *tqh_first;
    struct thread_s **tqh_last;
};

typedef struct buffer_s {
    char input_buffer[MAX_COMMAND_LENGTH];
    char output_buffer[MAX_COMMAND_LENGTH];
} buffer_t;

typedef struct fd_s {
    fd_set input;
    fd_set save_input;
    fd_set ouput;
} fd_t;

typedef struct ai_position_s {
    int x;
    int y;
    ai_direction_t direction;
} ai_position_t;

typedef struct ai_command_data_s {
    char *execusion;
    char **queue;
    float cast_time;
    bool is_contracted;
    double time;
} ai_command_data_t;

typedef struct client_s {
    buffer_t buffer;
    client_type_t type;
    int client_number;
    int level;
    char *team_name;
    struct sockaddr_in other_socket_addr;
    ai_command_data_t command;
    ai_position_t pos;
    inventory_t inventory;
} client_t;

typedef struct egg_s {
    int egg_number;
    int client_number;
    int x;
    int y;
    TAILQ_ENTRY(egg_s) next;
} egg_t;

struct egghead {
    struct egg_s *tqh_first;
    struct egg_s **tqh_last;
};

typedef struct team_s {
    char *name;
    char team_uuid[MAX_UUID_LENGTH];
    int nb_drones;
    int nb_matures_eggs;
    struct egghead eggs_head;
    client_t *client;
    TAILQ_ENTRY(team_s) next;
} team_t;

struct teamhead {
    struct team_s *tqh_first;
    struct team_s **tqh_last;
};


typedef struct zappy_server_s {
    fd_t fd;
    int my_socket;
    int actual_sockfd;
    int index_eggs;
    int index_clients;
    bool server_running;
    struct sockaddr_in server_addr;
    struct teamhead all_teams;
    struct threadhead all_threads;
    struct client_s clients[FD_SETSIZE];
    map_tile_t **map_tile;
    args_config_t *args;
} zappy_server_t;

// Linked list functions
void free_threads(struct threadhead *head);
void free_teams(struct teamhead *head);

// Tools functions
bool is_valid_resource(char *resource);
void free_array(char **array);
void set_inventory_resource_quantite(inventory_t *tile_inventory,
    char *resource, int resource_quantity);
// Server functions
int init_server(zappy_server_t *zappy_server, args_config_t *args);
int close_server(zappy_server_t *zappy_server);
int fd_is_set(zappy_server_t *zappy_server);
void init_buffer_struct(buffer_t *buffer, int *my_socket);
int scan_fd(zappy_server_t *zappy_server);
void handle_client(zappy_server_t *zappy_server);
int setup_server(int port, int max_clients);
void handle_client(zappy_server_t *zappy_server);
char **splitter(char const *const str, char *separator);
void generate_random_uuid(char *buffer);
int accept_new_connection(int my_socket,
    struct sockaddr_in *client_socket_addr);
int setup_server(int port, int max_clients);
int save_info_to_file(zappy_server_t *zappy_server);
int read_info_from_save_file(zappy_server_t *zappy_server);
thread_t *search_in_threads(struct threadhead *thread_head, char *uuid);
team_t *search_in_teams(struct teamhead *team_head, char *uuid);
int get_len_char_tab(char **command);
time_t get_actual_time(void);
int count_str_char(char *str, char c);

// COMMANDS
typedef struct command_s {
    char *command;
    void (*func)(zappy_server_t *zappy_server, char *command);
} command_t;

int handle_unknown_command(zappy_server_t *zappy_server, char *command);

// SERVER COMMANDS FUNCTIONS
int handle_server_command(zappy_server_t *zappy_server, char *command);
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

// AI FUNCTIONS
typedef struct command_ai_s {
    char *command;
    int (*func)(zappy_server_t *zappy_server, client_t *client, char *command);
} command_ai_t;

int queue_to_exec(client_t *client);
int add_in_queue(client_t *client, char *cmd);
int handle_ai_command(zappy_server_t *zappy, client_t *client, char *command);
int cast_action(zappy_server_t *zappy, client_t *client, int freq, char *cmd);
bool check_action(zappy_server_t *zappy, client_t *client);
int ai_initialisation(zappy_server_t *zappy_server, client_t *ia,
    team_t *tmp_team);

// AI COMMANDS FUNCTIONS
int ai_command_help(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_forward(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_right(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_left(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_take_object(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_fork(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_inventory(zappy_server_t *zappy, client_t *client, char *cmd);
int ai_command_look(zappy_server_t *zappy, client_t *client, char *cmd);

void send_ppo_command_to_all_gui(zappy_server_t *zappy, client_t *client);
void send_pin_command_to_all_gui(zappy_server_t *zappy, client_t *client);

// GUI COMMANDS FUNCTIONS
int handle_gui_command(zappy_server_t *zappy_server, char *command);
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

// ERROR FUNCTIONS
void error_command_argument(char *command, int nb_argument, int nb_expected);

#endif /* !ZAPPY_SERVER_H_ */
