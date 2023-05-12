#!/usr/bin/python2.7 -tt

import sys
import socket

maximum_number_of_pending_connections = 1
number_of_bytes_to_recv_at_a_time = 1024
address_index_in_first_line_of_http_request = 1
local_host_addr = '127.0.0.1'
counter_addr = '/counter'


def create_socket_connection(port_number):
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        server_socket.bind((local_host_addr, port_number))
        server_socket.listen(maximum_number_of_pending_connections)
        return server_socket
    except socket.error as err:
        print('Failed to create socket connection: ' + err)
        sys.exit(1)


def receive_http_request(client_socket):
    request_data = ""
    while not request_data.endswith('\r\n\r\n'):
        request_data += client_socket.recv(number_of_bytes_to_recv_at_a_time).decode()
    return request_data


def check_if_request_is_valid(request_data):
    request_first_line = request_data.split('\r\n')[0]
    address = request_first_line.split(' ')[address_index_in_first_line_of_http_request]
    return address == counter_addr


def create_http_response(is_valid_request, num_of_client_requests):
    response_template = ('HTTP/1.{http_version} {status_code} {status_message}\r\n'
                         'Content-type: text/html\r\n'
                         'Content-length: {content_length}\r\n\r\n'
                         '{response_body}\r\n\r\n''')

    if is_valid_request:
        body = str(num_of_client_requests)
        response = response_template.format(
            http_version=0,
            status_code=200,
            status_message='OK',
            content_length=len(body),
            response_body=body
        )
    else:
        body = '<html><head><title>Not Found</title></head><body>\r\n' \
               'Sorry, the object you requested was not found.\r\n</body></html>'
        response = response_template.format(
            http_version=1,
            status_code=404,
            status_message='Not Found',
            content_length=len(body),
            response_body=body
        )

    return response


def send_http_response(response, client_socket):
    response_bytes = response.encode()
    bytes_sent = 0
    while bytes_sent < len(response_bytes):  # TODO: Check if it's okay, I only saw a warning regarding C
        bytes_sent += client_socket.send(response_bytes[bytes_sent:])


def run_server(port_number):
    server_socket = create_socket_connection(port_number)

    num_of_client_requests = 0
    while True:
        client_socket, addr = server_socket.accept()
        request_data = receive_http_request(client_socket)
        is_request_valid = check_if_request_is_valid(request_data)
        if is_request_valid:
            num_of_client_requests += 1
        response = create_http_response(is_request_valid, num_of_client_requests)
        send_http_response(response, client_socket)

    # TODO: understand how to call this, if needed. server_socket.close()


if __name__ == "__main__":
    run_server(port_number=12345) #int(sys.argv[1]))
