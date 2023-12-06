#ifndef SERVER_H
#define SERVER_H

#include <string>

const int PORT = 8080;

int create_socket();
bool set_socket_options(int sockfd);
bool set_non_blocking(int sockfd);
bool bind_and_listen(int sockfd);

#endif // SERVER_H
