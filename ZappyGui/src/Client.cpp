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
 * This destructor closes the socket file descriptor associated with the
 * client.
 */
zappy::Client::~Client() { close(this->_socketFd); }

/**
 * @brief Connects to the server.
 *
 * This function creates a socket and connects to the server using the
 * specified hostname and port. If the connection is successful, it prints a
 * message indicating that it is connected to the server. If the connection
 * fails, it prints an error message and closes the socket.
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
        ssize_t bytesSent =
            send(this->_socketFd, message.c_str() + totalBytesSent,
                messageLength - totalBytesSent, 0);

        if (bytesSent == -1) {
            perror("send");
            return;
        }
        totalBytesSent += bytesSent;
    }
}

/**
 * @brief Splits the messages in the buffer.
 *
 * This function splits the messages in the buffer based on the specified
 * split character.
 *
 * @param splitChar The character to split the messages on.
 * @return std::vector<std::string> A vector of strings containing the
 * messages.
 */
std::vector<std::string> splitMessages(std::string message, char splitChar)
{
    std::vector<std::string> messages;
    std::stringstream ss(message);
    std::string token;

    while (std::getline(ss, token, splitChar)) {
        messages.push_back(token);
    }
    return messages;
}

void zappy::Client::receiveFromServer()
{
    char buffer[BUFSIZ];
    ssize_t bytesRead;

    while (this->running) {
        bytesRead = read(this->_socketFd, buffer, BUFSIZ);
        if (bytesRead == -1) {
            perror("read");
            return;
        }
        this->_buffer.append(buffer, bytesRead);
        if (this->_buffer.find(END_STR) != std::string::npos) {
            std::vector<std::string> messages =
                splitMessages(this->_buffer, END_STR);
            for (std::string message : messages) {
                this->pushToQueue(splitMessages(message, ' '));
                this->_buffer.clear();
            }
        }
    }
}

/**
 * @brief Initializes the zappy::Client object.
 *
 * This function parses the command line arguments and sets the port and
 * hostname based on the provided options.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of C-style strings containing the command line
 * arguments.
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
 * This function sets the port number that will be used by the client to
 * connect to the server.
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

/**
 * @brief Gets the queue of the client.
 *
 * @return The queue of the client.
 */
std::queue<std::vector<std::string>> zappy::Client::getQueue()
{
    return this->_queue;
}

/**
 * @brief Pushes a message to the queue.
 *
 * This function pushes a message to the queue of the client.
 *
 * @param message The message to push.
 */
void zappy::Client::pushToQueue(std::vector<std::string> message)
{
    this->_queue.push(message);
}

/**
 * @brief Pops a message from the queue.
 *
 * This function pops a message from the queue of the client.
 *
 * @return The message that was popped.
 */
std::vector<std::string> zappy::Client::popFromQueue()
{
    std::vector<std::string> message = this->_queue.front();
    this->_queue.pop();
    return message;
}
