#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <signal.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

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

