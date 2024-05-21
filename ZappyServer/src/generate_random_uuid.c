/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** generate_random_uuid
*/

#include <string.h>
#include <uuid/uuid.h>

void generate_random_uuid(char *buffer)
{
    uuid_t uuid;
    char user_uuid[50];

    uuid_generate_random(uuid);
    uuid_unparse(uuid, user_uuid);
    strcpy(buffer, user_uuid);
}
