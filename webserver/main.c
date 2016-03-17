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
#include "stats.h"

int main(void)
{
  int socket_client, socket_server;
  if ((socket_server = create_server(8080)) == -1) return EXIT_FAILURE;

  initialize_signals();
  
  web_stats *stats;
  if(!init_stats()){
    return EXIT_FAILURE;
  }
  stats = get_stats();

  for (;;) {
    if ((socket_client = accept(socket_server, NULL, NULL)) == -1) {
      perror("Connection refused");
      return EXIT_FAILURE;
    }

    puts("Client connected");
    stats->served_connections++;
	
    FILE *tinyum;
    if ((tinyum = fdopen(socket_client, "w+")) == NULL){
      perror("fdopen");
      return EXIT_FAILURE;
    }

    pid_t pid;
    if ((pid = fork()) == -1) {
      perror("fork");
      close(pid);
      return EXIT_FAILURE;
    }
    if(pid == 0){
      if(client(tinyum, stats, socket_client) == 1){
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    else {
      fclose(tinyum);
      close(socket_client);
    }
  }

  close(socket_server);

  return EXIT_SUCCESS;
}
