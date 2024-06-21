/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_client
*/

#include <gtest/gtest.h>

#include "../include/Client.hpp"

TEST(Client, constructor_test)
{
    zappy::Client client;
    client.setPort(4242);
    client.setHostname("HOSTNAME");

    EXPECT_EQ(client.getPort(), 4242);
    EXPECT_EQ(client.getHostname(), "HOSTNAME");
}
