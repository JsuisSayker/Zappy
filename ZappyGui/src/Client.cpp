/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Client
*/

#include "Client.hpp"

zappy::Client::Client() {}

/**
    * @brief Destructor for the Client class.
    * 
    * This destructor closes the socket file descriptor associated with the client.
    */
zappy::Client::~Client() { close(this->_socketFd); }

/**
 * @brief Connects to the server.
 * 
 * This function creates a socket and connects to the server using the specified hostname and port.
 * If the connection is successful, it prints a message indicating that it is connected to the server.
 * If the connection fails, it prints an error message and closes the socket.
 */
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

/**
 * Sends a message to the server.
 * 
 * @param message The message to send.
 */
void zappy::Client::sendToServer(std::string message)
{
    size_t totalBytesSent = 0;
    size_t messageLength = message.size();

    while (totalBytesSent < messageLength) {
        ssize_t bytesSent = send(this->_socketFd, message.c_str() + totalBytesSent, messageLength - totalBytesSent, 0);

        if (bytesSent == -1) {
            perror("send");
            return;
        }

        totalBytesSent += bytesSent;
    }
}

/**
 * @brief Receives data from the server.
 * 
 * This function reads data from the socket connected to the server and stores it in the internal buffer of the client.
 * The function reads data in chunks and dynamically resizes the buffer if necessary.
 * The received data is stored as a string in the buffer, and the function adjusts the size of the string to the actual data size.
 * If the received data ends with the END_STR character, it is removed from the buffer.
 * 
 * @note This function assumes that the socket is already connected to the server.
 * 
 * @note This function does not handle any errors that may occur during the read operation.
 * 
 * @note The buffer size is defined by the BUFSIZ constant.
 * 
 * @note The buffer is a member variable of the Client class.
 * 
 * @note This function does not return any value.
 */
void zappy::Client::receiveFromServer()
{
    int size = 0;
    int nBytesRead = 0;
    this->_buffer.resize(BUFSIZ);

    nBytesRead = read(this->_socketFd, &this->_buffer[size],
        this->_buffer.size() - size - 1);
    if (nBytesRead == -1) {
        return;
    }

    while (nBytesRead > 0) {
        size += nBytesRead;
        if (size > BUFSIZ - 1 || this->_buffer[size - 1] == END_STR) {
            break;
        }
        this->_buffer.resize(this->_buffer.size() +
            BUFSIZ); // Increase buffer size if necessary
        nBytesRead = read(this->_socketFd, &this->_buffer[size],
            this->_buffer.size() - size - 1);
    }

    this->_buffer.resize(
        size); // Adjust the size of the string to the actual data size
    if (!this->_buffer.empty() && this->_buffer.back() == END_STR) {
        this->_buffer.pop_back();
    }

    std::cout << "Buffer :" << this->_buffer << std::endl;
}

/**
 * @brief Initializes the zappy::Client object.
 *
 * This function parses the command line arguments and sets the port and hostname
 * based on the provided options.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of C-style strings containing the command line arguments.
 */
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

/**
 * @brief Sets the hostname for the client.
 * 
 * @param hostname The hostname to set.
 */
void zappy::Client::setHostname(std::string hostname)
{
    this->_hostname = hostname;
}

/**
 * @brief Sets the port number for the client.
 *
 * This function sets the port number that will be used by the client to connect to the server.
 *
 * @param port The port number to set.
 */
void zappy::Client::setPort(int port) { this->_port = port; }

/**
 * @brief Get the hostname of the client.
 * 
 * @return std::string The hostname of the client.
 */
std::string zappy::Client::getHostname() { return this->_hostname; }

/**
 * @brief Get the port number used by the client.
 * 
 * @return The port number.
 */
int zappy::Client::getPort() { return this->_port; }

/**
 * @brief Get the socket file descriptor.
 * 
 * @return int The socket file descriptor.
 */
int zappy::Client::getSocketFd() { return this->_socketFd; }

/**
 * @brief Sets the socket file descriptor for the client.
 * 
 * @param socket_fd The socket file descriptor to set.
 */
void zappy::Client::setSocketFd(int socket_fd) { this->_socketFd = socket_fd; }

/**
 * @brief Gets the buffer of the client.
 * 
 * @return The buffer of the client as a string.
 */
std::string zappy::Client::getBuffer() { return this->_buffer; }

/**
 * @brief Sets the buffer for the client.
 *
 * This function sets the buffer for the client to the specified value.
 *
 * @param buffer The buffer to set.
 */
void zappy::Client::setBuffer(std::string buffer) { this->_buffer = buffer; }
