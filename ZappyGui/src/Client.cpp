/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Client
*/

#include "Client.hpp"

zappy::Client::Client() {}

zappy::Client::~Client() { close(this->_socketFd); }

void zappy::Client::connectToServer()
{
    std::cout << "Connected to the server" << std::endl;
    struct sockaddr_in server_addr;
    this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->_socketFd == -1) {
        fprintf(stderr, "Failed to connect to the server\n");
        close(this->_socketFd);
        return;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->_port);
    server_addr.sin_addr.s_addr = inet_addr(this->_hostname.c_str());
    if (connect(this->_socketFd,
            reinterpret_cast<struct sockaddr *>(&server_addr),
            sizeof(server_addr)) == -1) {
        perror("Error (connect)");
        close(this->_socketFd);
    }
}

void zappy::Client::sendToServer()
{
    
}

void zappy::Client::receiveFromServer()
{
    int size = 0;
    int nBytesRead = 0;
    this->_buffer.resize(BUFSIZ);

    nBytesRead = read(this->_socketFd, &this->_buffer[size], this->_buffer.size() - size - 1);
    if (nBytesRead == -1) {
        return;
    }

    while (nBytesRead > 0) {
        size += nBytesRead;
        if (size > BUFSIZ - 1 || this->_buffer[size - 1] == END_STR) {
            break;
        }
        this->_buffer.resize(this->_buffer.size() + BUFSIZ); // Increase buffer size if necessary
        nBytesRead = read(this->_socketFd, &this->_buffer[size], this->_buffer.size() - size - 1);
    }

    this->_buffer.resize(size); // Adjust the size of the string to the actual data size
    if (!this->_buffer.empty() && this->_buffer.back() == END_STR) {
        this->_buffer.pop_back();
    }


    std::cout << "Buffer :" << this->_buffer << std::endl;
}

void zappy::Client::init(int argc, char **argv)
{
    int opt = getopt(argc, argv, "p:h:");

    while (opt != -1) {
        switch (opt) {
        case 'p':
            this->_port = atoi(optarg);
            break;
        case 'h':
            this->_hostname = optarg;
            break;
        default:
            break;
        }
        opt = getopt(argc, argv, "p:h:");
    }
}
