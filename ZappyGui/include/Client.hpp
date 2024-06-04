/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Client
*/

#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define END_STR '\n'

namespace zappy {
class Client {
  public:
    Client();
    ~Client();

    void init(int argc, char **argv);
    void connectToServer();
    void sendToServer(std::string message);
    void receiveFromServer();

    void setHostname(std::string hostname);
    void setPort(int port);

    std::string getHostname();
    int getPort();

    int getSocketFd();
    void setSocketFd(int socket_fd);

    std::string getBuffer();
    void setBuffer(std::string buffer);

  protected:
    std::string _hostname; // IP address
    int _port; // Port number
    int _socketFd; // Socket file descriptor
    std::string _buffer; // Buffer to store data
};
} // namespace zappy
