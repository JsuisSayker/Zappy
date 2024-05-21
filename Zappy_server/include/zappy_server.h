/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** zappy_server
*/

#ifndef ZAPPY_SERVER_H_
    #define ZAPPY_SERVER_H_

    #include "../../include/myteams.h"
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
    #define SAVE_FILE "myteams_save.txt"
    #define USERS_CHAR 'u'
    #define PRIVATE_MESSAGE_CHAR 'm'
    #define TEAMS_CHAR 't'
    #define CHANNELS_CHAR 'c'
    #define THREADS_CHAR 'h'
    #define REPLY_CHAR 'r'
    #define SUBSCRIBE_CHAR 's'

    #define FOOD_DENSITY 0.5
    #define LINEMATE_DENSITY 0.3
    #define DERAUMERE_DENSITY 0.15
    #define SIBUR_DENSITY 0.1
    #define MENDIANE_DENSITY 0.1
    #define PHIRAS_DENSITY 0.08
    #define THYSTAME_DENSITY 0.05
    #define MAX_CLIENTS 10

typedef struct subscribed_s {
    char team_uuid[MAX_UUID_LENGTH];
    char user_uuid[MAX_UUID_LENGTH];
    TAILQ_ENTRY(subscribed_s) next;
} subscribed_t;

struct subscribedhead {
    struct subscribed_s *tqh_first;
    struct subscribed_s **tqh_last;
};

typedef struct user_s {
    char username[MAX_NAME_LENGTH];
    char uuid[MAX_UUID_LENGTH];
    char team_context[MAX_UUID_LENGTH];
    char channel_context[MAX_UUID_LENGTH];
    char thread_context[MAX_UUID_LENGTH];
    int nb_clients;
    TAILQ_ENTRY(user_s) next;
} user_t;

struct userhead {
    struct user_s *tqh_first;
    struct user_s **tqh_last;
};

typedef struct message_s {
    char text[MAX_BODY_LENGTH];
    char sender_uuid[MAX_UUID_LENGTH];
    char receiver_uuid[MAX_UUID_LENGTH];
    char message_uuid[MAX_UUID_LENGTH];
    time_t timestamp;
    TAILQ_ENTRY(message_s) next;
} message_t;

struct messagehead {
    struct message_s *tqh_first;
    struct message_s **tqh_last;
};

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

typedef struct channel_s {
    char name[MAX_NAME_LENGTH];
    char desc[MAX_DESCRIPTION_LENGTH];
    char channel_uuid[MAX_UUID_LENGTH];
    char team_uuid[MAX_UUID_LENGTH];
    struct threadhead threads_head;
    TAILQ_ENTRY(channel_s) next;
} channel_t;

struct channelhead {
    struct channel_s *tqh_first;
    struct channel_s **tqh_last;
};

typedef struct team_s {
    char name[MAX_NAME_LENGTH];
    char desc[MAX_DESCRIPTION_LENGTH];
    char team_uuid[MAX_UUID_LENGTH];
    struct channelhead channels_head;
    TAILQ_ENTRY(team_s) next;
} team_t;

struct teamhead {
    struct team_s *tqh_first;
    struct team_s **tqh_last;
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

typedef struct client_s {
    buffer_t buffer;
    user_t *user;
    struct sockaddr_in other_socket_addr;
} client_t;

typedef struct teams_server_s {
    int my_socket;
    int actual_sockfd;
    fd_t fd;
    struct sockaddr_in server_addr;
    struct userhead all_user;
    struct messagehead private_messages;
    struct subscribedhead subscribed_teams_users;
    struct teamhead all_teams;
    struct client_s clients[FD_SETSIZE];
} teams_server_t;

// Linked list functions
void free_subscribed(struct subscribedhead *head);
void free_messages(struct messagehead *head);
void free_users(struct userhead *head);
void free_threads(struct threadhead *head);
void free_channels(struct channelhead *head);
void free_teams(struct teamhead *head);

// Tools functions
void free_array(char **array);

// Server functions
int init_server(teams_server_t *teams_server, int port);
void init_buffer_struct(buffer_t *buffer, int *my_socket);
int scan_fd(teams_server_t *teams_server);
void handle_client(teams_server_t *teams_server);
int setup_server(int port, int max_clients);
void handle_client(teams_server_t *teams_server);
char **splitter(char const *const str, char *separator);
void generate_random_uuid(char *buffer);
int accept_new_connection(int my_socket);
int setup_server(int port, int max_clients);
int save_info_to_file(teams_server_t *teams_server);
int read_info_from_save_file(teams_server_t *teams_server);
thread_t *search_in_threads(struct threadhead *thread_head, char *uuid);
channel_t *search_in_channels(struct channelhead *channel_head, char *uuid);
team_t *search_in_teams(struct teamhead *team_head, char *uuid);
int get_len_char_tab(char **command);
int find_all_context(teams_server_t *teams_server, team_t **team,
    channel_t **channel, thread_t **thread);
time_t get_actual_time(void);
int count_str_char(char *str, char c);
// get UUID
user_t *get_user_by_uuid(struct userhead *user_head, char *uuid);
team_t *get_team_by_uuid(struct teamhead *teams_head, char *uuid);
channel_t *get_channel_by_uuid(struct channelhead *channel_head, char *uuid);
channel_t *get_all_channel_by_uuid(struct teamhead *team_head, char *uuid);
thread_t *get_thread_by_uuid(struct threadhead *thread_head, char *uuid);
thread_t *get_all_thread_by_uuid(struct teamhead *team_head, char *uuid);

typedef struct all_context_s {
    team_t *team;
    channel_t *channel;
    thread_t *thread;
} all_context_t;

// COMMANDS
typedef struct command_s {
    char *command;
    void (*func)(teams_server_t *teams_server, char *command);
} command_t;

void help_command(teams_server_t *teams_server, char *command);
void login_command(teams_server_t *teams_server, char *command);
void logout_command(teams_server_t *teams_server, char *command);
void users_command(teams_server_t *teams_server, char *command);
void user_command(teams_server_t *teams_server, char *command);
void send_command(teams_server_t *teams_server, char *command);
void messages_command(teams_server_t *teams_server, char *command);
void subscribe_command(teams_server_t *teams_server, char *command);
void subscribed_command(teams_server_t *teams_server, char *command);
void unsubscribe_command(teams_server_t *teams_server, char *command);
void use_command(teams_server_t *teams_server, char *command);
void create_command(teams_server_t *teams_server, char *command);
void list_command(teams_server_t *teams_server, char *command);
void info_command(teams_server_t *teams_server, char *command);

int add_team(teams_server_t *teams_server, char **command_line, int nb_args,
    all_context_t *all_context);
int add_channel(teams_server_t *teams_server, char **command_line, int nb_args,
    all_context_t *all_context);
int add_thread(teams_server_t *teams_server, char **command_line, int nb_args,
    all_context_t *all_context);









typedef struct char_tab_s {
    char *str;
    TAILQ_ENTRY(char_tab_s) next;
} char_tab_t;

struct char_tab_head {
    struct char_tab_s *tqh_first;
    struct char_tab_s **tqh_last;
};

// char_tab functions
void free_char_tab_list(struct char_tab_head *head);
void display_char_tab_list(struct char_tab_head *head);
int count_char_tab_list(struct char_tab_head *head);
void random_char_tab_list(struct char_tab_head *head);

////////////////////////////////////////

typedef struct args_config_s {
    int port;
    int width;
    int height;
    int clientsNb;
    float freq;
    struct char_tab_head names;
} args_config_t;

typedef struct map_tile_s {
    int x;
    int y;
    struct char_tab_head resources;
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
void display_map_tile(map_tile_t **map_tile);

// int array functions
int **generate_int_array(int x, int y);
void free_int_array(int **possible_coordinate);
void shuffle_int_array(int **array, int n);

#endif /* !ZAPPY_SERVER_H_ */
