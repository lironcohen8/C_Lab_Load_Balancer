#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "socket_handler.h"

#define SERVER_PORT_FILE_NAME "server_port"
#define HTTP_PORT_FILE_NAME "http_port"
#define NUMBER_OF_CLIENTS 1
#define NUMBER_OF_SERVERS 3
#define BUFFER_SIZE 1024


int main() {
    srand(time(NULL));
    int listening_server_socket = create_socket_connection(NUMBER_OF_SERVERS);
    write_socket_port_to_file(listening_server_socket, SERVER_PORT_FILE_NAME);
    int http_socket = create_socket_connection(NUMBER_OF_CLIENTS);
    write_socket_port_to_file(http_socket, HTTP_PORT_FILE_NAME);

    int active_server_sockets[NUMBER_OF_SERVERS];
    wait_for_servers_connections(active_server_sockets, listening_server_socket, NUMBER_OF_SERVERS);

    int next_server_index = 0;
    while (true) {
        int current_server_socket = active_server_sockets[next_server_index];
        int client_socket = accept_connection_on_socket(http_socket);
        char buffer[BUFFER_SIZE];
        receive_data_from_socket(client_socket, buffer, BUFFER_SIZE);
        send_data_to_socket(current_server_socket, buffer, BUFFER_SIZE);
        receive_data_from_socket(current_server_socket, buffer, BUFFER_SIZE);
        send_data_to_socket(client_socket, buffer, BUFFER_SIZE);
        next_server_index = (next_server_index + 1) % NUMBER_OF_SERVERS;
    }

    return 0;
}