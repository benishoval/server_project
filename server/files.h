#ifndef FILES
#define FILSE
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BUFFER_SIZE 1024

enum entry_type {
    FOLDER,
    ENTRY_FILE
};

struct directory_manager {
    struct directory_entry* entries; // pointer to an array of structures
    int size; // size of the array
};

struct directory_entry {
    char* item_name;
    enum entry_type type;
};

int isfile_exist(char* path, char* file_name);

struct directory_manager* read_directory(char* path);

int count_entry(char* path);

void free_directory_manager(struct directory_manager* manager);

void receive_file_from_client(const char* filename, int client_socket);

void send_file_to_client(FILE* filename, int client_socket);
#endif
