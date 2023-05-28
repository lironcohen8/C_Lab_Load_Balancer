#include "lb.h"
#include "lb_transfer.h"
#include "socket_handler.h"

#define SERVER_PORT_FILE_NAME "server_port"
#define HTTP_PORT_FILE_NAME "http_port"
#define NUMBER_OF_CLIENTS 1

void setup_load_balancer(load_balancer_t* lb) {
  lb->listening_server_socket = create_socket_connection(NUMBER_OF_SERVERS);
  write_socket_port_to_file(lb->listening_server_socket, SERVER_PORT_FILE_NAME);
  lb->http_socket = create_socket_connection(NUMBER_OF_CLIENTS);
  write_socket_port_to_file(lb->http_socket, HTTP_PORT_FILE_NAME);
  wait_for_servers_connections(lb->active_server_sockets, lb->listening_server_socket, NUMBER_OF_SERVERS);
  lb->current_active_socket = 0;
}

void load_balancer_handle_client(load_balancer_t* lb) {
  int current_server_socket = lb->active_server_sockets[lb->current_active_socket];
  int client_socket = accept_connection_on_socket(lb->http_socket);
  forward_socket_data(client_socket, current_server_socket, CLIENT_TO_SERVER);
  forward_socket_data(current_server_socket, client_socket, SERVER_TO_CLIENT);
  lb->current_active_socket = (lb->current_active_socket + 1) % NUMBER_OF_SERVERS;
}