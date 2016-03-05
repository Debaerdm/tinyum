/*
 * webserver/http_status.c
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define CR "\n"

const char *reason_phrase(int code){
    switch(code){
    case 200: return "OK" CR;
    case 400: return "Bad Request" CR;
    case 403: return "Forbidden" CR;
    case 404: return "Not Found" CR;
    case 405: return "Method Not Allowed" CR;
    case 500: return "Internal Server Error" CR;
    case 501: return "Not Implemetend" CR;
    case 505: return "HTTP Version Not Supported" CR;
    default: return NULL;
    }
}

void send_status(FILE *client, int code){
    fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase(code)); 
}

void send_response(FILE *client, int code, const char *message_body){
    send_status(client, code);
    fprintf(client, "Connection: close\r\nContent-length: %zu\r\n\r\n%s", strlen(message_body), message_body);
}
