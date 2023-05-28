#ifndef LB_H
#define LB_H

#define NUMBER_OF_SERVERS 3

typedef struct {
  int http_socket;
  int listening_server_socket;
  int active_server_sockets[NUMBER_OF_SERVERS];
  int current_active_socket;
} load_balancer_t;

void setup_load_balancer(load_balancer_t* lb);
void load_balancer_handle_client(load_balancer_t* lb);

#endif