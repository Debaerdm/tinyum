/*
* webserver/signals.c
* Configuration of signal client
*
* Copyright (C) 2016 Mathieu DE BAERDEMAEKER  mathieu.debaerdemaeker@gmail.com 
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
#include <signal.h> 
#include <sys/wait.h>

void handler(int sig)
{   
    pid_t pid;
    int status;
    
    if ((pid = waitpid(-1, &status, WNOHANG)) > 0)
        printf("Signal %d recivied\n", sig);
}

void initialize_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
}

