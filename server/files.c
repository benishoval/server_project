#include "files.h"

//The function returns the number of items in the folder 
int count_entry(char* path)
{
    int count = 0;
    DIR* dir = opendir(path);
    
        if (dir == NULL) {
            printf("Open dir faild");
            return 0;
        }
    
        struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        //The if condition checks if it is a file or a folder
        if ((entry->d_type == DT_DIR) || (entry->d_type == DT_REG))
            
            count++;
    }
    closedir(dir);
    return count;
}

struct directory_manager* read_directory(char* path)
{
     struct directory_manager* manager = malloc(sizeof(struct directory_manager));
    if (manager == NULL)
    {
        printf("Allocation memory for directory manager faild\n");
        return NULL;
    }
    manager->size = count_entry(path);
    if (manager->size == 0) {
        printf("No item fuond\n");
        free(manager);
        return NULL;
    }
    manager->entries = malloc(manager->size * sizeof(struct directory_entry));
    if (manager->entries == NULL) {
        printf("Allocation memory for directory entries faild\n");
        free(manager);
        return NULL;
    }
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Open dir failed");
        free(manager->entries);
        free(manager);
        return NULL;
    }
    struct dirent* entry;
    int i = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG)
        {
            manager->entries[i].item_name = strdup(entry->d_name); // use strdup to allocate memory for the string
            manager->entries[i].type = entry->d_type == DT_DIR ? FOLDER : ENTRY_FILE;
            i++;
        }
    }
    closedir(dir);
    return manager;
}

void free_directory_manager(struct directory_manager* manager)
{
    for (int i = 0; i < manager->size; i++)
    {
        printf("free %s\n", manager->entries[i].item_name);
        free(manager->entries[i].item_name);
    }
    printf("free entries\n");
    free(manager->entries);
    printf("free manajer");
    free(manager);
}

//The function receives a file name and a buffer
//The function compares the buffer with what is written in the file line by line
//If there is a match the function returns 1 if not the function returns 0
int compare_file_with_buffer(const char* file_name, char* buffer)
{

printf("in comper file 136: buffer = %s\n", buffer);
     FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return -1;
    }

    char line[100];
    int found = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character at the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Compare the line with the buffer
        if (strcmp(line, buffer) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    return found;
}

void receive_file_from_client(const char* filename, int client_socket) {
    // Open the file in write mode
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    
    // Receive data from the client and write it to the file
    while ((bytesRead = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        if (fwrite(buffer, sizeof(char), bytesRead, file) != bytesRead) {
            perror("Failed to write data to file");
            break;
        }

        // Clear the buffer
        printf("180: in func receive file\n");
        memset(buffer, 0, sizeof(buffer));

    }

    // Close the file
    fclose(file);
}

void send_file_to_client(FILE* file, int client_socket) {

    char buffer[BUFFER_SIZE];
    int bytesRead;

    // Read from the file and send data to the server
    while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (write(client_socket, buffer, bytesRead) < 0) {
            perror("Failed to send data to server");
            break;
        }

        // Clear the buffer inside the loop
        memset(buffer, 0, sizeof(buffer));
    }
    // Close the file
    fclose(file);
}