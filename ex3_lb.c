#include "lb_transfer.h"
#include "socket_handler.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SERVER_PORT_FILE_NAME "server_port"
#define HTTP_PORT_FILE_NAME "http_port"
#define NUMBER_OF_CLIENTS 1
#define NUMBER_OF_SERVERS 3

int main()
{
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
    forward_socket_data(client_socket, current_server_socket, CLIENT_TO_SERVER);
    forward_socket_data(current_server_socket, client_socket, SERVER_TO_CLIENT);
    next_server_index = (next_server_index + 1) % NUMBER_OF_SERVERS;
  }

  return 0;
}