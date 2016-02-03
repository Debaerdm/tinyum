/*
* webserver/socket.c
* Launch a TCP server in the localhost
*
* Copyright (C) 2016 Mathieu debaerdemaeker mathieu.debaerdemaeker@gmail.com
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
* NON INFRINGEMENT.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

int create_server(int port)
{
    int socket_server;
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    int optval = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
	perror("Can not set SO_REUSEADDR option");
	return EXIT_FAILURE;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
        perror("Bind error");
        return EXIT_FAILURE;
    }

    if (listen(socket_server, 10) == -1) {
        perror("Listen error");
        return EXIT_FAILURE;
    }
    
    return socket_server;
}

void handler(int sig)
{
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	printf("Signal %d recivied\n", sig);
}

void initialize_signals(void)
{

    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
	perror("sigaction");
	exit(EXIT_FAILURE);
    }
    
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
	perror("signal");
	exit(EXIT_FAILURE);
    }
}
