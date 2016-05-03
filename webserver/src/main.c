#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <semaphore.h>
#include "../include/signals.h"
#include "../include/socket.h"
#include "../include/stats.h"

#define handle_error(msg) \
  do { perror(msg); return EXIT_FAILURE;  } while (0)

int main(void)
{
    int socket_client, socket_server;

    if ((socket_server = create_server(8080)) == -1)
        return EXIT_FAILURE;

    sem_t mutex;
    if(sem_init(&mutex, 0,1) < 0 ) {
        handle_error("Semaphore init");
    }

    initialize_signals();
    init_stats();
    web_stats *stats = get_stats();

    for (;;) {
        if ((socket_client = accept(socket_server, NULL, NULL)) == -1)
            handle_error("accept");

        puts("Client connected");
        stats->served_connections++;

        FILE *tinyum;
        if ((tinyum = fdopen(socket_client, "w+")) == NULL)
            handle_error("fdopen");

        pid_t pid;
        if ((pid = fork()) == -1) {
            close(pid);
            handle_error("fork");
        }

        if(pid == 0) {
            sem_wait(&mutex);

            if(client(tinyum, socket_client) == 1)
                handle_error("Communication");

            sem_post(&mutex);

            return EXIT_SUCCESS;
        } else {
            sem_wait(&mutex);
            fclose(tinyum);
            close(socket_client);
            sem_post(&mutex);
        }
    }

    if(sem_destroy(&mutex) < 0) {
        handle_error("Semaphrore destroy");
    }

    close(socket_server);

    return EXIT_SUCCESS;
}
