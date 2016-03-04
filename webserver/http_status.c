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

const char *reason_phrase(int code){
    switch(code){
    case 200:
	return "OK";
    case 400:
	return "Bad Request";
    case 403:
	return "Forbidden";
    case 404:
	return "Not Found";
    case 405:
	return "Method Not Allowed";
    case 500:
	return "Internal Server Error";
    case 501:
	return "Not Implemetend";
    case 505:
	return "HTTP Version Not Supported";
    default:
	return NULL;
    }
}

void send_status(FILE *client, int code){
    char message[64] = "HTTP/1.1";
    char str[15];
    sprintf(str, " %d ", code);
    strcat(message, str);
    strcat(message, reason_phrase(code));
    printf("%s\n", message);
    //return message;
    fwrite(message, strlen(message), 1, client);	
}


/*void send_response(FILE *client, int code, const char *phrase, const char *message_body){
		

  }*/
