/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_team_by_uuid
*/

#include "zappy_server.h"

channel_t *get_channel_by_uuid(struct channelhead *channel_head, char *uuid)
{
    channel_t *channel = NULL;

    TAILQ_FOREACH(channel, channel_head, next)
    {
        if (strcmp(channel->channel_uuid, uuid) == 0) {
            return channel;
        }
    }
    return NULL;
}

channel_t *get_all_channel_by_uuid(struct teamhead *team_head, char *uuid)
{
    team_t *team = NULL;
    channel_t *channel = NULL;

    TAILQ_FOREACH(team, team_head, next)
    {
        channel = get_channel_by_uuid(&team->channels_head, uuid);
        if (strcmp(channel->channel_uuid, uuid) == 0) {
            return channel;
        }
    }
    return NULL;
}
