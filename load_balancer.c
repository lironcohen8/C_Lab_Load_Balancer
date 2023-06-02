#include "load_balancer.h"
#include "socket_handler.h"
#include <assert.h>
#include <stdlib.h>

#define SERVER_PORT_FILE_NAME "server_port"
#define HTTP_PORT_FILE_NAME "http_port"
#define NUMBER_OF_CLIENTS 1
#define BASE_BUFFER_SIZE 1024
#define NUM_SEPARATORS_CLIENT_TO_SERVER 1
#define NUM_SEPARATORS_SERVER_TO_CLIENT 2

/* private functions */
void forward_data(int src_sock, int dest_sock, forward_socket_direction direction)
{
  char* buffer = (char*)calloc(1, BASE_BUFFER_SIZE);
  assert(buffer != NULL);
  unsigned int num_separators_until_stop =
      (direction == CLIENT_TO_SERVER) ? NUM_SEPARATORS_CLIENT_TO_SERVER : NUM_SEPARATORS_SERVER_TO_CLIENT;

  int bytes_to_forward = receive_data_from_socket(src_sock, &buffer, BASE_BUFFER_SIZE, num_separators_until_stop);
  send_data_to_socket(dest_sock, buffer, bytes_to_forward);

  free(buffer);
}

/* public functions */
void setup_load_balancer(load_balancer_t* lb)
{
  lb->listening_server_socket = create_socket_connection(NUMBER_OF_SERVERS);
  write_socket_port_to_file(lb->listening_server_socket, SERVER_PORT_FILE_NAME);
  lb->http_socket = create_socket_connection(NUMBER_OF_CLIENTS);
  write_socket_port_to_file(lb->http_socket, HTTP_PORT_FILE_NAME);
  wait_for_servers_connections(lb->active_server_sockets, lb->listening_server_socket, NUMBER_OF_SERVERS);
  lb->current_active_socket = 0;
}

void load_balancer_handle_client(load_balancer_t* lb)
{
  int current_server_socket = lb->active_server_sockets[lb->current_active_socket];
  int client_socket = accept_connection_on_socket(lb->http_socket);
  forward_data(client_socket, current_server_socket, CLIENT_TO_SERVER);
  forward_data(current_server_socket, client_socket, SERVER_TO_CLIENT);
  lb->current_active_socket = (lb->current_active_socket + 1) % NUMBER_OF_SERVERS;
}