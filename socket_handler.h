#ifndef SOCKET_HANLDER_H
#define SOCKET_HANLDER_H

#include <stdio.h>

int create_socket_connection(int backlog);
int write_socket_port_to_file(int socket, char* file_name);
void wait_for_servers_connections(int* server_sockets, int number_of_servers);
int accept_connection_on_socket(int socket);
void receive_data_from_socket(int socket, char* buffer, int buffer_size);
void send_data_to_socket(int socket, char* buffer, int buffer_size);

#endif