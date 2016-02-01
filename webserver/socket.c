#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

int create_server(int port){
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int socket_serveur;

    if((socket_serveur = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket error");
        return EXIT_FAILURE;
    }
    if(bind(socket_serveur, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("Bind error");
        return EXIT_FAILURE;
    }
    if(listen(socket_serveur, 10) == -1){
        perror("Listen error");
        return EXIT_FAILURE;
    }
    
    return socket_serveur;
}
