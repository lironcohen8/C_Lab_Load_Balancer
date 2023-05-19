#include "socket_handler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SERVER_PORT_FILE_NAME "server_port"
#define HTTP_PORT_FILE_NAME "http_port"
#define NUMBER_OF_CLIENTS 1
#define NUMBER_OF_SERVERS 3
#define BASE_BUFFER_SIZE 1024
#define NUM_SEPARATORS_CLIENT_TO_SERVER 1
#define NUM_SEPARATORS_SERVER_TO_CLIENT 2

typedef enum { CLIENT_TO_SERVER, SERVER_TO_CLIENT } forward_socket_direction;

void forward_socket_data(int src_sock, int dest_sock, forward_socket_direction direction)
{
  char* buffer = (char*)calloc(1, BASE_BUFFER_SIZE);
  assert(buffer != NULL);
  unsigned int num_separators =
      (direction == CLIENT_TO_SERVER) ? NUM_SEPARATORS_CLIENT_TO_SERVER : NUM_SEPARATORS_SERVER_TO_CLIENT;

  int bytes_to_forward = receive_data_from_socket(src_sock, &buffer, BASE_BUFFER_SIZE, num_separators);
  send_data_to_socket(dest_sock, buffer, bytes_to_forward);

  free(buffer);
}

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