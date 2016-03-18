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
#include "../include/http_request.h"
#include "../include/http_status.h"
#include "../include/config_url.h"
#include "../include/stats.h"

#define BUFFER_SIZE 1024
#define WWW_DIR "/public_html"

int client(FILE *tinyum, int socket_client)
{
    web_stats *stats = get_stats();
    char buf[BUFFER_SIZE];

    /* Clean the buffer stream */
    memset(buf, 0, sizeof(buf));
    http_request req;
	    
    fgets_or_exit(buf, sizeof(buf), tinyum);
	    
    int request;
    request = read_http_header(buf, &req);
    ++stats->served_requests;
    skip_headers(tinyum);
      
    char *path = getenv("HOME");
    strcat(path, WWW_DIR);
        
    if (request) {
      send_response(tinyum, 400, "Bad Request\r\n");
    } else if (req.m == HTTP_INVALID) {
      send_response(tinyum, 405, "Method Not Allowed\r\n");
    } else if (url_valid(req.uri) == 1) {
      send_response(tinyum, 403, "Forbidden\r\n");
      return EXIT_FAILURE;
    } else if (strcmp(req.uri, "/stats") == 0){
      strcat(req.uri, ".html");
      strcat(path, req.uri);
      send_stats(tinyum, path);
    } else {
      int fildes;
      if ((fildes = check_and_open(req.uri, path)) == 1) {
        send_status(tinyum, 404);
        int not_found_file;
        if((not_found_file = check_and_open("/404.html", path)) != 1){
          fprintf(tinyum, "Connection: close\r\nContent-Type: %s\r\nContent-length: %d\r\n\r\n", application_type("/404.html"), get_file_size(not_found_file));
          fflush(tinyum);
          copy(not_found_file, socket_client);
        }
        close(not_found_file);
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
}
