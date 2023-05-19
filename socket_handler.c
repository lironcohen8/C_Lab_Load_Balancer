#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SOCKET_ERROR -1
#define LOCAL_HOST "127.0.0.1"
#define MIN_PORT_NUMBER 1024
#define MAX_PORT_NUMBER 64000

/* private functions */
void bind_socket_to_a_free_port(int socket, struct sockaddr_in sock_addr) {
    while (true) {
        int port = rand() % (MAX_PORT_NUMBER - MIN_PORT_NUMBER + 1) + MIN_PORT_NUMBER;
        sock_addr.sin_port = htons(port);
        int enable = 1;
        socklen_t addr_size = sizeof(sock_addr);
        setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)); // TODO: check that it won't ruin the "busy" port check
        if (bind(socket, (const struct sockaddr *) &sock_addr, addr_size) == 0) {
            break;
        }
    }
}

int get_socket_port(int socket) {
    struct sockaddr_in sock_addr;
    socklen_t addr_size = sizeof(sock_addr);
    getsockname(socket, (struct sockaddr *) &sock_addr, &addr_size);
    return ntohs(sock_addr.sin_port);
}

/* public functions */
int create_socket_connection(int backlog) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == SOCKET_ERROR) {
        printf("Error at socket creation\n");
        exit(1);
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
    bind_socket_to_a_free_port(sock, sock_addr);
    int ret_val = listen(sock, backlog);
    if (ret_val != 0) {
        printf("Error at socket listening\n");
        exit(1);
    }
    return sock;
}

void write_socket_port_to_file(int socket, char* file_name) {
    int port = get_socket_port(socket);
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(1);
    }
    fprintf(file, "%d", port);
    fclose(file);
}

int accept_connection_on_socket(int socket) { 
    struct sockaddr client_sock_addr;
    socklen_t addr_size = sizeof(client_sock_addr);
    int client_socket = accept(socket, &client_sock_addr , &addr_size); // TODO: name may be confusing
    if (client_socket == SOCKET_ERROR) {
        printf("Error at socket accepting\n");
        exit(1);
    }
    return client_socket;
}


void wait_for_servers_connections(int* server_sockets, int server_socket, int number_of_servers) {
    for (int i = 0; i < number_of_servers; i++) {
        server_sockets[i] = accept_connection_on_socket(server_socket);
    }
}

void receive_data_from_socket(int socket, char* buffer, int buffer_size) {
    int bytes_received = recv(socket, buffer, buffer_size, 0);
    if (bytes_received == SOCKET_ERROR) { // TODO: need to realloc buffer until \r\n\r\n
        printf("Error at socket receiving\n");
        exit(1);
    }
}

void send_data_to_socket(int socket, char* buffer, int buffer_size) {
    int bytes_sent = send(socket, buffer, buffer_size, 0);
    if (bytes_sent == SOCKET_ERROR) { // TODO: need to realloc buffer until \r\n\r\n
        printf("Error at socket sending\n");
        exit(1);
    }
}
