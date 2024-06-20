/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_server_struct
*/

#ifndef AI_SERVER_STRUCT_H_
    #define AI_SERVER_STRUCT_H_
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

typedef enum client_type_s {
    IA,
    GUI,
    UNKNOWN,
}client_type_t;

typedef struct char_tab_s {
    char *str;
    TAILQ_ENTRY(char_tab_s) next;
} char_tab_t;

struct char_tab_head {
    struct char_tab_s *tqh_first;
    struct char_tab_s **tqh_last;
};

typedef struct args_config_s {
    int port;
    int width;
    int height;
    int clientsNb;
    int freq;
    struct char_tab_head names;
} args_config_t;

typedef struct buffer_s {
    char input_buffer[MAX_COMMAND_LENGTH];
    char output_buffer[MAX_COMMAND_LENGTH];
} buffer_t;

// AI PARAMETERS

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct map_tile_s {
    int x;
    int y;
    inventory_t inventory;
} map_tile_t;

typedef enum ai_direction_s {
    NORTH,
    EAST,
    SOUTH,
    WEST,
} ai_direction_t;

typedef struct ai_position_s {
    int x;
    int y;
    ai_direction_t direction;
} ai_position_t;

typedef struct ai_command_data_s {
    char *execution;
    char **queue;
    float cast_time;
    bool is_contracted;
    double time;
    bool incantation;
} ai_command_data_t;

typedef struct ai_health_s {
    double last_meal;
    double time_to_eat;
} ai_health_t;

typedef struct message_s {
    char *message;
    ai_position_t pos;
} message_t;

typedef struct client_s {
    buffer_t buffer;
    client_type_t type;
    int client_number;
    int level;
    bool incantation;
    char *team_name;
    struct sockaddr_in other_socket_addr;
    ai_health_t health;
    ai_command_data_t command;
    ai_position_t pos;
    inventory_t inventory;
    message_t *message_receive;
} client_t;

typedef struct fd_s {
    fd_set input;
    fd_set save_input;
    fd_set ouput;
} fd_t;


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

typedef struct look_struct_s {
    ai_position_t pos;
    char *message;
} look_struct_t;

#endif /* !AI_SERVER_STRUCT_H_ */
