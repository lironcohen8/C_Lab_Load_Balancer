#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>

#define INVALID_SOCKET -1
#define BUFFER_SIZE 1024

/* private functions */
void bind_socket_to_a_free_port(socket, sock_addr) {
    while (true) {
        int port = rand() % (64000 - 1024 + 1) + 1024;
        sock_addr.sin_port = htons(port);
        setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &1, sizeof(int)); // TODO: check that it won't ruin the "busy" port check
        if (bind(socket, (struct sockaddr_in *) &sock_addr, sizeof(asock_addrddr)) == 0) {
            break;
        }
    }
}

int get_socket_port(int socket) {
    struct sockaddr_in sock_addr;
    int sock_addr_len = sizeof(sock_addr);
    getsockname(socket, (struct sockaddr *) &sock_addr, &sock_addr_len);
    return ntohs(sock_addr.sin_port);
}

/* public functions */
int create_socket_connection(int backlog) {
    int socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket == INVALID_SOCKET) {
        printf("Error at socket creation: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind_socket_to_a_free_port(socket, sock_addr);
    int ret_val = listen(socket, backlog);
    if (ret_val != 0) {
        printf("Error at socket listening: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return socket;
}

int write_socket_port_to_file(int socket, char* file_name) {
    int port = get_socket_port(socket);
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(file, "%d", port);
    fclose(file);
}

void wait_for_servers_connections(int* server_sockets, int number_of_servers) {
    for (int i = 0; i < number_of_servers; i++) {
        server_sockets[i] = accept_connection_on_socket(server_socket);
    }
}

int accept_connection_on_socket(int socket) {
    int client_socket = accept(socket); // TODO: name may be confusing
    if (client_socket == INVALID_SOCKET) {
        printf("Error at socket accepting: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return socket;
}

void receive_data_from_socket(int socket, char* buffer, int buffer_size) {
    int bytes_received = recv(socket, buffer, buffer_size, 0);
    if (bytes_received == SOCKET_ERROR) { # TODO: need to realloc buffer until \r\n\r\n
        printf("Error at socket receiving: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
}

void send_data_to_socket(int socket, char* buffer, int buffer_size) {
    int bytes_sent = send(socket, buffer, buffer_size, 0);
    if (bytes_sent == SOCKET_ERROR) { # TODO: need to realloc buffer until \r\n\r\n
        printf("Error at socket sending: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
}
