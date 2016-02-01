#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include <sys/types.h> 
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(void)
{
  
    int socket_server;
    if ((socket_server = create_server(8080)) == -1)
	return EXIT_FAILURE;

    int socket_client;
    if ((socket_client = accept(socket_server, NULL, NULL)) == -1) {
        perror("Connection refused");
        return EXIT_FAILURE;
    }

    const char *message = "Welcome to tinyum, tinyum is a server for TCP connection\n";
    write(socket_client, message, strlen(message));

    char buf[BUFFER_SIZE];
    int n;
    for ( ; ; ) {
      
	bzero(buf, BUFFER_SIZE);
      
	n = read(socket_client, buf, BUFFER_SIZE - 1);
	if (n == -1) {
	    perror("read");
	    return EXIT_FAILURE;
	}
	
	buf[BUFFER_SIZE -1] = '\0';
	printf("%s", buf);
	write(socket_client, buf, n);

    }

    close(socket_server);
    close(socket_client);

    return EXIT_SUCCESS;
}
