#include "server.h"



int main()
{

    char folder[] = "server";
// server socket 
    int socket_descriptor;
// socket created by the bind function
    int client_socket;
// buffer to store message
    char storage_buffer[BUFFER_SIZE];
    int length_of_address;
// option value for respective option_name
    int option_value=1;
// server and client address structures
    struct sockaddr_in server_address;
    struct sockaddr_in connection_address;
    char message[100];
// creating the socket with IPv4 domain and TCP protocol
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   // printf("line 36: (socet func) socket_descriptor = %d\n",socket_descriptor);
// Check if the socket is created successfully

    if(socket_descriptor<0)
   {
        perror("Socket creation failed");
// EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program
        exit(EXIT_FAILURE);
   } 
// set options for the socket
    int status=setsockopt(socket_descriptor, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value));
// check if options are set successfully
    if(status<0){
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }
//  initializing structure elements for address
    server_address.sin_family = AF_INET;
// convert port to network byte order using htons
    server_address.sin_port = htons(CONNECTION_PORT);
// any address available
    server_address.sin_addr.s_addr = INADDR_ANY;
// Assigning null character to the last index of the character array
    server_address.sin_zero[8]='\0';
// bind the socket with the values address and port from the sockaddr_in structure
    status=bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
// check if the binding was successful
    if(status<0){
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }
// listen on specified port with a maximum of 4 requests
    status=listen(socket_descriptor,4);
// check if the socket is listening successfully
    if(status<0){
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }
     while (1)
{   
    length_of_address = sizeof(connection_address);
// accept connection signals from the client
    
    client_socket = accept(socket_descriptor, (struct sockaddr*)&connection_address, &length_of_address);

// check if the server is accepting the signals from the client
    if(client_socket<0){
        perror("Couldn't establish connection with client");
        exit(EXIT_FAILURE);
    }
// Receive data sent by the client

    int is_legal;
    do{
      //  printf("74: in do\n");
        read(client_socket, storage_buffer, BUFFER_SIZE);
    // set the last index of the character array as a null character

        storage_buffer[BUFFER_SIZE] = '\0';
       // printf("78: Message from client: %s \n",storage_buffer);
        if(strcmp(storage_buffer, "close socket") == 0){
            printf("80: in if %s is legal = %d\n", storage_buffer, is_legal);
            //close(client_socket);
            break;
        }
        //The function checks if the user is registered in the system by username and password
        //The function returns 1 if the user is valid (registered in the system) and zero if not
        is_legal = compare_file_with_buffer("cleint_list.txt", storage_buffer);
        memset(storage_buffer, 0, 100);
        send(client_socket, &is_legal, sizeof(int), 0);
    }
    while(is_legal == 0);


    if(is_legal == 1)
    {
     int request;
     int is_file_exists;
     
     
     read(client_socket, &request, sizeof(int));
     send(client_socket,&request,sizeof(int),0);
     switch (request)
     {
        //Receive file from the client
        case 1:
        read(client_socket, storage_buffer, sizeof(storage_buffer));
        printf("105: file %s\n", storage_buffer);

        FILE*  file = fopen(storage_buffer, "r");
        if(file != NULL)
        {
            //If file exists in the system the server send 0 to the client
            is_file_exists = 0;
            fclose(file);
            printf("100\n");
            send(client_socket, &is_file_exists, sizeof(int), 0);
            //Emptied the buffer
            memset(storage_buffer, 0, 100);

        }
        else
        {
            //If file exist in the system rhe server send 1 to the client
            is_file_exists = 1;
            send(client_socket, &is_file_exists, sizeof(int), 0);
            receive_file_from_client(storage_buffer, client_socket);
            //Emptied the buffer
            memset(storage_buffer, 0, 100);
        }
        break;
        //Send file to the client 
        case 2:
        read(client_socket, storage_buffer, sizeof(storage_buffer));
        printf("134: case 2 buffer = %s\n", storage_buffer);
        FILE* file_to_send = fopen(storage_buffer, "r");

        //If file does not exist in the system send 0 to the client
        if(file_to_send == NULL){
            is_file_exists = 0;
            send(client_socket, &is_file_exists, sizeof(int), 0);
            fclose(file_to_send);//close file
            //Emptied the buffer
            memset(storage_buffer, 0, strlen(storage_buffer));
            
        }
        //File exist in the system
        else{
            //Send file to client
            send_file_to_client(file_to_send, client_socket);
            memset(storage_buffer, 0, strlen(storage_buffer));

        }
        break;
     }

    }

    close(client_socket);
}
    return 0;
}

