#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>
#include "stats.h"

/**
 * @file
 * @brief functions used for to create a server and connect the client on this server
 */


/**
 * client - connected in the server and manipulate request from input 
 *
 * @param tinyum - The descriptor of file for input information like request
 * @param socket_client - The client connected actually into the server
 *
 * @return On succes client return 0, or 1 if an error occured
 */
int client(FILE *tinyum, int socket_client);

/**
 * create_server - make the server in the localhost with the port, 
 * once instance of this is only used
 *
 * @param port - The port of server listen for many connection
 *
 * @return On succes create_server return the socket of server, or 1 if an error occured
 */
int create_server(int port);

#endif
