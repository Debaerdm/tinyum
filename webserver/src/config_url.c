#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

#define BUFFER_SIZE 1024

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

/*
 *  rewrite_url - rewrite url if is "/"
 */
const char *rewrite_url(char *url){
  if (strcmp(url, "/") == 0)
    return "/index.html";
      
  char *tmp = strchr(url, '?');
    
  if (tmp == NULL)
    return url;
    
  url[(tmp - url)] = '\0';
    
  return url;
}

/*
 * check_and_open - check if file given by url exsit and open it
 */
int check_and_open(const char *url, const char *document_root){
  char path[strlen(url) + strlen(document_root) + 1];

  sprintf(path, "%s%s", document_root, url);

  struct stat conf;
  if (stat(path, &conf) == -1) 
	handle_error(path);

  if (S_ISREG(conf.st_mode))
    return open(path, O_RDONLY);

  return EXIT_FAILURE;
}

/*
 * get_file_size - retrun size of file
 */
int get_file_size(int fildes) {
  struct stat file_stat;

  if (fstat(fildes, &file_stat) < 0) 
	handle_error("fstat");

  return file_stat.st_size;
}

/*
 * copy - read into file descriptor in and rewrite into out
 */
int copy(int in, int out) {
  int n;
  char tamp[BUFFER_SIZE];

  while ((n = read(in, tamp, BUFFER_SIZE)) > 0) {
    if (write(out, tamp, n) != n) 
	handle_error("write");
  }
  return EXIT_SUCCESS;
}

/*
 * application_type - return content type by checking file extention
 */
const char *application_type(const char *url){
  const char *dot = strrchr(url, '.');
  if ( !dot || dot == url) return "";
  if (strcmp(dot+1, "html") == 0){
    return "text/html";
  }
  else if (strcmp(dot+1, "xml") == 0){
    return "application/xml";
  }
  else if (strcmp(dot+1, "js") == 0){
    return "application/javascript";
  }
  else if (strcmp(dot+1, "css") == 0){
    return "text/css";
  }
  else if (strcmp(dot+1, "jpg") == 0){
    return "image/jpeg";
  }
  else if (strcmp(dot+1, "png") == 0){
    return "image/png";
  }
  return "text";
}

/*
 * url_valid - avoid url contains ".."
 */
int url_valid(char *url) {
  if (strstr(url, "..") == NULL) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
