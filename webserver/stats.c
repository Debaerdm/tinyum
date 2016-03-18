#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "stats.h"
#include "http_status.h"
#include "config_url.h"

static web_stats stats;

int32_t replace_variable(FILE *client, char *uri){
  struct stat file_stat;
  int file;
  if((file = open(uri, O_RDWR)) == -1){
    perror("open");
    return EXIT_FAILURE;
  }
  
  if (fstat(file, &file_stat) < 0) {
    perror("fstat");
    return EXIT_FAILURE;
  }
      
  char buff[file_stat.st_size];
  int size_buf;
  while((size_buf = read(file, buff, file_stat.st_size)) > 0){
    fprintf(client, buff,  stats.served_connections, stats.served_requests, stats.ok_200, stats.ko_400, stats.ko_403, stats.ko_404, stats.ko_405);
  }
  
  if(size_buf == -1){
    perror("read");
    return EXIT_FAILURE;
  }
  close(file);
  return EXIT_SUCCESS;
}

void send_stats(FILE *client, char *uri){
  send_status(client, 200);
  fprintf(client, "Content-Type: %s\n\n", application_type(uri));
  if(replace_variable(client, uri) == 1){
    perror("Replace failed");
    exit(1);
  }
}

int init_stats(void){
  char *addr;
 
  if ((addr = mmap(NULL, sizeof(stats), PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
	perror("mmap");
	return EXIT_FAILURE;
  }

  stats.served_connections = stats.served_requests = stats.ok_200 = stats.ko_400 = stats.ko_403 = stats.ko_404 = stats.ko_405 = 0;
  return stats.served_connections == 0 && stats.served_requests == 0 && stats.ok_200 == 0 &&  stats.ko_400 == 0 && stats.ko_403 == 0 && stats.ko_404 == 0 &&  stats.ko_405 == 0;
}

web_stats *get_stats(void){
  return &stats;
}
