#include "client.h"

int main()
{
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
  
    do{
        printf("Enter your name\n");
        scanf("%[^\n]s", name);
        
        getchar();
        printf("Enter passeword\n");
        scanf("%[^\n]s", password);

        strcpy(user_name_and_password, name);
        user_name_and_password[strlen(name)] = ',';

        strcat(user_name_and_password, password);
        printf("39: user name and passeword: %s\n", user_name_and_password);
        
        //Send user name and password to rhe server for testing
        write(socket_descriptor, user_name_and_password, strlen(user_name_and_password));

        //receive a message from the server if signal_from_server = 1 than user name and password is legal
        //If signal_from_server = 0 Invalid username or password
        read(socket_descriptor, &signal_from_server, sizeof(int));

        //Clear name
        memset(name, 0, strlen(name));
        //Clear password
        memset(password, 0, strlen(password));

        if(signal_from_server == 0){
            printf("User name or passeword wrong To retry press 1 to exit the system press 0\n");
            scanf("%d", &to_retry);
            //If to_retry = 0 Instructs the server to close the socket
            if(to_retry == 0){
                write(socket_descriptor, to_close_socket, strlen(to_close_socket));
                return;
            }
        }
    //while signal_from_server = 0 We are asking for username and password again
    }while(signal_from_server == 0);
    
while (1)
{
        //Ask from user to chose from menu
        //1 for send file to server
        //2 for recive file from server
        //3 to get the list of files in the system
        user_choice = cleint_menu();

        //Send the user choise to server
        write(socket_descriptor, &user_choice, sizeof(int));
        read(socket_descriptor, &user_choice, sizeof(int));

        switch (user_choice)
        {
            case 1:
                printf("Enter file name\n");
                scanf("%s", message);

                //Send the file name to server
                write(socket_descriptor, message, strlen(message));

                //If the signal_from_server = 0 The file is exist in the system
                read(socket_descriptor, &signal_from_server, sizeof(int));
                if(signal_from_server == 0){
                    printf("The file is exist in the system\n");
                    memset(message, 0, strlen(message));
                }
                else{
                    //Send file to server
                    //The send_file_to_server closed the socket 
                    send_file_to_server(message, socket_descriptor);
                    
                    //Open new socket
                    open_new_socket(user_name_and_password);

                    //Clear message
                    memset(message, 0, strlen(message));

                }
                break;
                //Recive file from server
            case 2:
                printf("Enter file name\n");
                scanf("%s", message);

                //Send file name to the server
                write(socket_descriptor, message, strlen(message));
                read(socket_descriptor, &signal_from_server, sizeof(int));

                if(signal_from_server == 0){
                    printf("The file does not exist in the system");
                    memset(message, 0, strlen(message));
                }
                else{
                    //The function opens a new file at the client and closes the socket
                    receive_file_from_server(message, socket_descriptor);
                    memset(message, 0, strlen(message));

                    //Open a new socket
                    open_new_socket(user_name_and_password);
                }
                break;
        default:
            break;
        }
}


    



   // printf("Message from server: %s\n", recieve_buffer);
// terminate the socket connection
    close(socket_descriptor);
    return 0;
}