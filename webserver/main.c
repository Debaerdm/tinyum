#include <stdio.h>
#include "socket.h"
#include <sys/types.h> 
#include <sys/socket.h>

int main(void)
{
    int socket_serveur = create_server(8080); 
    char *message = "Welcome to tinyum, tinyum is a server for TCP connection\n";
    if(accept(socket_serveur, NULL, NULL) == -1){
        perror("Connection refused");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
