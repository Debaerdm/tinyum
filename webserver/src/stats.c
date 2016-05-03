/*
* stats.c
*
* Copyright (C) 2016 Clément Boin clem.boin@gmail.com
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
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "../include/stats.h"
#include "../include/http_status.h"
#include "../include/config_url.h"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

static web_stats *pstats;

void replace_variable(FILE *client, char *uri)
{
    struct stat file_stat;
    int file;

    if ((file = open(uri, O_RDWR)) == -1) {
        handle_error("open_stats");
    }

    if (fstat(file, &file_stat) < 0) {
        handle_error("stats files");
    }

    char buff[file_stat.st_size];
    int size_buf;

    while((size_buf = read(file, buff, file_stat.st_size)) > 0) {
        fprintf(client, buff,  pstats->served_connections,pstats->served_requests,pstats->ok_200, pstats->ko_400, pstats->ko_403, pstats->ko_404, pstats->ko_405);
    }

    if(size_buf == -1) {
        handle_error("read file");
    }

    close(file);
}

void send_stats(FILE *client, char *uri)
{
    send_status(client, 200);
    fprintf(client, "Content-Type: %s\n\n", application_type(uri));
    replace_variable(client, uri);
}

int init_stats(void)
{
    char *addr;

    if ((addr = mmap(NULL, sizeof(web_stats), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        handle_error("mmap");
    }

    pstats = ((web_stats *) addr);
    pstats->served_connections = 0;
    pstats->served_requests = 0;
    pstats->ok_200 = 0;
    pstats->ko_400 = 0;
    pstats->ko_403 = 0;
    pstats->ko_404 = 0;
    pstats->ko_405 = 0;

    return EXIT_SUCCESS;
}

web_stats *get_stats(void)
{
    return pstats;
}
