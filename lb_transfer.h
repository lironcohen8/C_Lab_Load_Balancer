#ifndef LB_TRANSFER_H
#define LB_TRANSFER_H

typedef enum { CLIENT_TO_SERVER, SERVER_TO_CLIENT } forward_socket_direction;

void forward_socket_data(int src_sock, int dest_sock, forward_socket_direction direction);

#endif