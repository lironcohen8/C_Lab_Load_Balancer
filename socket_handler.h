#ifndef SOCKET_HANLDER_H
#define SOCKET_HANLDER_H

int create_socket_connection(int backlog);
void write_socket_port_to_file(int socket, char* file_name);
void wait_for_servers_connections(int* server_sockets, int server_socket, int number_of_servers);
int accept_connection_on_socket(int socket);
int receive_data_from_socket(int socket, char** buffer, int buffer_size, unsigned int num_of_delim_until_stop);
void send_data_to_socket(int socket, char* buffer, int bytes_to_send);

#endif