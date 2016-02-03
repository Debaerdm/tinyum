#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <signal.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

void initialize_signals(void) {
		if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
				perror("signal");
		}
}


