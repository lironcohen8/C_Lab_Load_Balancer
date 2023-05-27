#include "lb_transfer.h"
#include "socket_handler.h"
#include <assert.h>
#include <stdlib.h>

#define BASE_BUFFER_SIZE 1024
#define NUM_SEPARATORS_CLIENT_TO_SERVER 1
#define NUM_SEPARATORS_SERVER_TO_CLIENT 2

void forward_socket_data(int src_sock, int dest_sock, forward_socket_direction direction)
{
  char* buffer = (char*)calloc(1, BASE_BUFFER_SIZE);
  assert(buffer != NULL);
  unsigned int num_separators_until_stop =
      (direction == CLIENT_TO_SERVER) ? NUM_SEPARATORS_CLIENT_TO_SERVER : NUM_SEPARATORS_SERVER_TO_CLIENT;

  int bytes_to_forward = receive_data_from_socket(src_sock, &buffer, BASE_BUFFER_SIZE, num_separators_until_stop);
  send_data_to_socket(dest_sock, buffer, bytes_to_forward);

  free(buffer);
}