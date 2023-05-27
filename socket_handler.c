#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "memory_utils.h"
#include "socket_handler.h"
#include "string_utils.h"

#define SOCKET_ERROR -1
// todo : verify this
#define LOCAL_HOST "127.0.0.1"
#define MIN_PORT_NUMBER 1024
#define MAX_PORT_NUMBER 64000
#define HTTP_SEPARATOR "\r\n\r\n"
#define BUFF_SIZE_MULTIPLY_FACTOR 2

/* private functions */
void bind_socket_to_a_free_port(int socket, struct sockaddr_in sock_addr)
{
  while (true) {
    int port = rand() % (MAX_PORT_NUMBER - MIN_PORT_NUMBER + 1) + MIN_PORT_NUMBER;
    sock_addr.sin_port = htons(port);
    int enable = 1;
    socklen_t addr_size = sizeof(sock_addr);
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable,
               sizeof(enable));  // TODO: check that it won't ruin the "busy" port check
    if (bind(socket, (const struct sockaddr*)&sock_addr, addr_size) == 0) {
      break;
    }
  }
}

int get_socket_port(int socket)
{
  struct sockaddr_in sock_addr;
  socklen_t addr_size = sizeof(sock_addr);
  getsockname(socket, (struct sockaddr*)&sock_addr, &addr_size);
  return ntohs(sock_addr.sin_port);
}

/* public functions */
int create_socket_connection(int backlog)
{
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  assert(sock != SOCKET_ERROR);
  struct sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
  bind_socket_to_a_free_port(sock, sock_addr);
  int ret_val = listen(sock, backlog);
  assert(ret_val != SOCKET_ERROR);
  return sock;
}

void write_socket_port_to_file(int socket, char* file_name)
{
  int port = get_socket_port(socket);
  FILE* file = fopen(file_name, "w");
  assert(file != NULL);
  fprintf(file, "%d", port);
  fclose(file);
}

int accept_connection_on_socket(int socket)
{
  struct sockaddr client_sock_addr;
  socklen_t addr_size = sizeof(client_sock_addr);
  int client_socket = accept(socket, &client_sock_addr, &addr_size);  // TODO: name may be confusing
  assert(client_socket != SOCKET_ERROR);
  return client_socket;
}

void wait_for_servers_connections(int* server_sockets, int server_socket, int number_of_servers)
{
  for (int i = 0; i < number_of_servers; i++) {
    server_sockets[i] = accept_connection_on_socket(server_socket);
  }
}

int receive_data_from_socket(int socket, char** buffer, int buffer_size, unsigned int num_of_delim_until_stop)
{
  int bytes_read_by_recv = 0, total_bytes_read = 0;
  int current_buff_len = buffer_size;
  int new_buffer_len = 0;
  do {
    if (current_buff_len == total_bytes_read) {
      new_buffer_len = current_buff_len * BUFF_SIZE_MULTIPLY_FACTOR;
      realloc_larger_buffer(buffer, current_buff_len, new_buffer_len);
      current_buff_len = new_buffer_len;
    }
    bytes_read_by_recv = recv(socket, *buffer + total_bytes_read, current_buff_len - total_bytes_read, 0);
    assert(bytes_read_by_recv != SOCKET_ERROR);
    total_bytes_read += bytes_read_by_recv;
  } while (count_sub_string_appearances(*buffer, HTTP_SEPARATOR) < num_of_delim_until_stop);

  return total_bytes_read;
}

void send_data_to_socket(int socket, char* buffer, int num_of_bytes_to_send)
{
  int total_bytes_sent = 0, bytes_sent_in_iteration = 0;
  do {
    bytes_sent_in_iteration = send(socket, buffer + total_bytes_sent, num_of_bytes_to_send - total_bytes_sent, 0);
    assert(bytes_sent_in_iteration != SOCKET_ERROR);
    total_bytes_sent += bytes_sent_in_iteration;
  } while (total_bytes_sent < num_of_bytes_to_send);
}
