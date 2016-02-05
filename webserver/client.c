/*
 * webserver/main.c
 * Client for connecting in the socket server
 *
 * Copyright (C) 2016 Mathieu DE BAERDEMAEKER mathieu.debaerdemaeker@gmail.com
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <setjmp.h>
#include "signals.h"
#include "socket.h"


#define BUFFER_SIZE 1024


int main(void)
{
		int socket_client, socket_server;
		if ((socket_server = create_server(8080)) == -1) return EXIT_FAILURE;

		initialize_signals();

		for (;;) {
				if ((socket_client = accept(socket_server, NULL, NULL)) == -1) {
						perror("Connection refused");
						return EXIT_FAILURE;
				}

				puts("Client connected");

				pid_t pid;
				if ((pid = fork()) == -1) {
						perror("fork");
						return EXIT_FAILURE;
				}


				if (pid == 0) {

						const char *message = "Welcome to tinyum, tinyum is a server for TCP connection\n";

						if (write(socket_client, message, strlen(message)) == -1) {
								perror("write");
								return EXIT_FAILURE;
						}

						char buf[BUFFER_SIZE];
						int read_size;

						/* Clean the buffer stream */
						memset(buf, 0, BUFFER_SIZE);
						while ((read_size = recv(socket_client, buf, BUFFER_SIZE, 0)) > 0) {
								write(socket_client, buf, BUFFER_SIZE);
								memset(buf, 0, BUFFER_SIZE);
						}
	
						switch (read_size){
							case 0:
									puts("Client disconnected");
									fflush(stdout);
									break;
							case -1:
									perror("recv");
									return EXIT_FAILURE;
						}

						close(socket_client);
						return EXIT_SUCCESS;
				} else {
						close(socket_client);
				}
		}

		close(socket_server);

		return EXIT_SUCCESS;
}