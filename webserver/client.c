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
#include "http_request.h"
#include "signals.h"
#include "socket.h"
#include "http_status.h"
#include "config_url.h"

#define BUFFER_SIZE 1024
#define WWW_DIR "/home/infoetu/debaerdm/public_html"

int main(void)
{
    int socket_client, socket_server;
    if ((socket_server = create_server(8080)) == -1) return EXIT_FAILURE;

    initialize_signals();

    /*const char * motd = "It's work!\r\n";*/

    for (;;) {
	if ((socket_client = accept(socket_server, NULL, NULL)) == -1) {
	    perror("Connection refused");
	    return EXIT_FAILURE;
	}

	puts("Client connected");

	FILE *tinyum;
	if ((tinyum = fdopen(socket_client, "w+")) == NULL){
	    perror("fdopen");
	    return EXIT_FAILURE;
	}

	pid_t pid;
	if ((pid = fork()) == -1) {
	    perror("fork");
	    return EXIT_FAILURE;
	}


	if (pid == 0) {
	    char buf[BUFFER_SIZE];

	    /* Clean the buffer stream */
	    memset(buf, 0, sizeof(buf));
	    http_request req;
	    
	    fgets_or_exit(buf, sizeof(buf), tinyum);

	    int request;
	    request = read_http_header(buf, &req);
	    skip_headers(tinyum);

	    if (request) {
	      send_response(tinyum, 400, "Bad Request\r\n");
	    } else if (req.m == HTTP_INVALID) {
	      send_response(tinyum, 405, "Method Not Allowed\r\n");
	    } else {
                int fildes; 
                if ((fildes = check_and_open(req.uri, WWW_DIR)) == 1) {
                    send_response(tinyum, 404, "Not Found\r\n");
                    return EXIT_FAILURE;
                } else {
                    send_status(tinyum, 200);
                    fprintf(tinyum, "Connection: close\r\nContent-Type: %s\r\nContent-length: %d\r\n\r\n", application_type(req.uri), get_file_size(fildes));
                    fflush(tinyum);
                    copy(fildes, socket_client);
                }
		close(fildes);
            }
	   	    
	    memset(buf, 0, sizeof(buf));
	    fclose(tinyum);
	    close(socket_client);
	    return EXIT_SUCCESS;
            
	} else {
	    close(socket_client);
	    fclose(tinyum);
	}
    }

    close(socket_server);

    return EXIT_SUCCESS;
}
