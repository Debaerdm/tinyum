#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "stats.h"
#include "http_status.h"
static web_stats stats;

int32_t replace_variable(FILE *client){
  struct stat file_stat;
  int file;
  if((file = open("stats.html", O_RDWR)) == -1){
    perror("open");
    return EXIT_FAILURE;
  }

  if (fstat(file, &file_stat) < 0) {
    perror("fstat");
    return EXIT_FAILURE;
  }

  char buff[file_stat.st_size];
  int sizeBuf;
  while((sizeBuf = read(file, buff, file_stat.st_size)) > 0){
    fprintf(client, buff, stats.served_connections, stats.served_requests, stats.ok_200, stats.ko_400, stats.ko_403, stats.ko_404, stats.ko_405);
  }
  if(sizeBuf == -1){
    perror("fread");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void send_stats(FILE *client){
  send_status(client, 200);
  fprintf(client, "Content-Type: text/html\n\n");
  if(replace_variable(client) == 1){
    perror("Replace failed");
    exit(1);
  }
}

int32_t init_stats(void){
  stats.served_connections = stats.served_requests = stats.ok_200 = stats.ko_400 = stats.ko_403 = stats.ko_404 = stats.ko_405 = 0;
  return EXIT_SUCCESS;
}

web_stats *get_stats(void){
  return &stats;
}
