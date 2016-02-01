#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include <sys/types.h> 
#include <sys/socket.h>

int main(void)
{
    int socket_client; 
    int socket_serveur = create_server(8080);

    if ((socket_client = accept(socket_serveur, NULL, NULL)) == -1) {
        perror("Connection refused");
        return EXIT_FAILURE;
    }

    const char *message = "Welcome to tinyum, tinyum is a server for TCP connection\n";
    write(socket_client, message, strlen(message)); 

    return EXIT_SUCCESS;
}
