/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Client
*/

#pragma once

#include <arpa/inet.h>
#include <functional>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <sstream>
#include <sys/socket.h>
#include "Map.hpp"
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

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

    // Methods for queue
    void pushToQueue(std::vector<std::string> message);
    std::vector<std::string> popFromQueue();

    // Setters and Getters
    void setHostname(std::string hostname);
    void setPort(int port);
    void setBuffer(std::string buffer);
    void setSocketFd(int socket_fd);

    std::string getHostname();
    int getPort();
    int getSocketFd();
    std::string getBuffer();
    std::queue<std::vector<std::string>> getQueue();

    bool running = true; // Boolean to check if the client is running
    std::mutex _mutex;   // Mutex to lock the queue
    Map map;                                    // Map object

  protected:
    std::string _hostname;                       // IP address
    int _port;                                   // Port number
    int _socketFd;                               // Socket file descriptor
    std::string _buffer;                         // Buffer to store data
    std::queue<std::vector<std::string>> _queue; // Queue to store messages
};
} // namespace zappy
