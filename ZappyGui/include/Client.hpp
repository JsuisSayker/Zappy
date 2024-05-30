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
    void connectToServer(); // connect
    void sendToServer();
    void receiveFromServer();

    void setHostname(std::string hostname) { this->_hostname = hostname; }
    void setPort(int port) { this->_port = port; }

    std::string getHostname() { return this->_hostname; }
    int getPort() { return this->_port; }

    int getSocketFd() { return this->_socketFd; }
    void setSocketFd(int socket_fd) { this->_socketFd = socket_fd; }

  protected:
    std::string _hostname = "default";
    int _port = 0;
    int _socketFd = 0;
};
} // namespace zappy
