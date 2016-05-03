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

int client(FILE *tinyum, int socket_client);
int create_server(int port);

#endif
