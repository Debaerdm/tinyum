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

static web_stats *pstats;

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
    fprintf(client, buff,  pstats->served_connections,pstats->served_requests,pstats->ok_200, pstats->ko_400, pstats->ko_403, pstats->ko_404, pstats->ko_405);
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
 
  if ((addr = mmap(NULL, sizeof(web_stats), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
	perror("mmap");
	return EXIT_FAILURE;
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

web_stats *get_stats(void){
  return pstats;
}
