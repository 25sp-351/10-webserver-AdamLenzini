#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <pthread.h>
#include "request.h"

#define DEFAULT_PORT 80
#define LISTEN_BACKLOG 5

// typedef struct params {
//     int fd;
// } connect_params;

// // This routine frees fd_ptr
// void handleConnection(connect_params* params) {
//     ssize_t returnval;
//     char buf[MAX_BUF_LEN];

//     printf("Handling connection on %d\n", params->fd);

//     while(1) {
//         //printf("Reading from %d\n", *fd_ptr);
//         returnval = read(params->fd, buf, sizeof(buf));

//         if (returnval <= 0) {
//             printf("Closing connection on %d\n", params->fd);
//             close(params->fd);
//             free(params);
//             break;
//         }

//         buf[returnval] = '\0';
        
//         printf("%zd, %s\n", returnval, buf);

//         write(params->fd, buf, returnval);
//     }
// }

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;
    int fd = 0;

    // Checks if the -p flag is present in the command line arguments
    if (argc >= 3 && strcmp(argv[1], "-p") == 0) {
        sscanf(argv[2], "%d", &port);
        // if (port <= 1024) {
        //     printf("Port number must be greater than 1024. Defaulting to 80\n");
        //     port = DEFAULT_PORT;
        // }
    }   

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("Binding to port %d\n", port);

    int returnval;

    returnval = bind(socket_fd, (struct sockaddr*)&socket_address, 
                        sizeof(socket_address));

    if (returnval < 0) {
        perror("bind");
        return 1;
    }

    returnval = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while (1) {
        pthread_t thread;
        //connect_params* params = malloc(sizeof(connect_params));

        fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);

        printf("Accepted connection from %d\n", params->fd);

        //pthread_create(&thread, NULL, (void* (*)(void*))handleConnection, (void*)params);
        pthread_create(&thread, NULL, (void* (*)(void*))request_read_from_fd, (void*)fd);
    }

    return 0;
}