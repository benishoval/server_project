//#ifndef CLIENT
//#define CLIENT
// provides required data types
#include<sys/types.h>
// has address families and socket functions
#include<sys/socket.h>
// has the sockaddr_in structure
#include<netinet/in.h>
// header for reading and write functions
#include<fcntl.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include<stdlib.h>
#include <stdio.h>
#include "menu.h"


// port through which connection is to be made
#define CONNECTION_PORT 3500
#define BUFFER_SIZE 1024
#define MESSAGE_SIZE 1024
//#endif 

char name[100];
char password[100];
char user_name_and_password[200];
char to_close_socket[] = "close socket";
char message[MESSAGE_SIZE];
int signal_from_server = -1;
int to_retry = 1;
int user_choice;


