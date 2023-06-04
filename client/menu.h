#ifndef MENU
#define MENU
#include<stdlib.h>
#include <string.h>
#include <stdio.h>
// provides required data types
#include <sys/types.h>
// holds address family and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// has functions for read and write operations
#include <fcntl.h>
#define BUFFER_SIZE 1024
#define CONNECTION_PORT 3500
#define BUFFER_SIZE 1024
#define MESSAGE_SIZE 1024
int cleint_menu();

void send_file_to_server(const char* filename, int client_socket);

void open_new_socket(char* user_name_and_password);

void receive_file_from_server(const char* filename, int client_socket);

#endif 