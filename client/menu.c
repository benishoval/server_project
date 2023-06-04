
#include "menu.h"

//The function prints the menu in the terminal
int cleint_menu()
{
    int choice = 0;
    printf("\t\tWELCOM\n\n");
    printf("\t\tTo transfer files to the server, press 1\n");
    printf("\t\tTo download files from the server, press 2\n");
    printf("\t\tFor a list of files and folders, press 3\n");
    printf("\t\t");
    scanf("%d", &choice);
   // printf("13 in menu: user_choice = %d\n", choice);
    return choice;
    
}
void send_file_to_server(const char* filename, int client_socket) {
    // Open the file in read mode
    printf("20: in send file file name = %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    // Read from the file and send data to the server
    while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (write(client_socket, buffer, bytesRead) < 0) {
            perror("Failed to send data to server");
            break;
        }
        // Clear the buffer inside the loop
        memset(buffer, 0, sizeof(buffer));
    }

    //Close the socket
    close(client_socket);
    // Close the file
    fclose(file);
}
void open_new_socket(char* user_name_and_password)
{
    printf("43: in open\n %s\n", user_name_and_password);
// structure to represent the address
    struct sockaddr_in server_address;
    
// storage buffer to receive messages
    char recieve_buffer[100];
// create an Ipv4 and TCP socket
    int socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
// Check whether the socket is created successfully
    if(socket_descriptor<0)
   {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
   }  
    int status=0;
// initialize address structure for binding
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
// set address to any address available
   server_address.sin_addr.s_addr =INADDR_ANY;
    server_address.sin_zero[8]='\0';
// connect to the server
    status=connect(socket_descriptor,(struct sockaddr*)&server_address, sizeof(server_address));
    if(status<0)
   {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
   }    
  
// send message to the server
    write(socket_descriptor, user_name_and_password, strlen(user_name_and_password));
    //write(socket_descriptor, &user_choice, sizeof(int));
    printf("76: in open END\n %s\n", user_name_and_password);

//return new_socket;
}

void receive_file_from_server(const char* filename, int client_socket) {
    // Open the file in write mode
    FILE* file = fopen(filename, "w");
    printf("86: recive file, file name = %s", filename);
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    int i = 0; 
    // Receive data from the client and write it to the file
    while ((bytesRead = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
            printf("181: %s", buffer); 
        if (fwrite(buffer, sizeof(char), bytesRead, file) != bytesRead) {
            perror("Failed to write data to file");
            break;
        }

        // Clear the buffer
        printf("180: in func receive file\n");
        memset(buffer, 0, sizeof(buffer));
        i++;
        printf("%d\n", i);
    }

    close(client_socket);
    // Close the file
    fclose(file);
}