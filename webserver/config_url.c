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

const char *rewrite_url(char *url){
    if (strcmp(url, "/") == 0)
	return "/index.html";
    
    char *tmp = strchr(url, '?');
    
    if (tmp == NULL)
	return url;
    
    url[(tmp - url)] = '\0';
    
    return url;
}

int check_and_open(const char *url, const char *document_root){
    char path[strlen(url) + strlen(document_root) + 1];
    sprintf(path, "%s%s", document_root, url);
    struct stat conf;
    if (stat(path, &conf) == -1) {
        perror(path);
        return EXIT_FAILURE;
    }

    if (S_ISREG(conf.st_mode))
	return open(path, O_RDONLY);

    return EXIT_FAILURE;
}

int get_file_size(int fildes) {
    struct stat file_stat;

    if (fstat(fildes, &file_stat) < 0) {
        perror("fstat");
        return EXIT_FAILURE;
    }

    return file_stat.st_size;
}

int copy(int in, int out) {
    int n;
    char tamp[BUFFER_SIZE];

    while ((n = read(in, tamp, BUFFER_SIZE)) > 0) {
        if (write(out, tamp, n) != n) {
            perror("write");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

const char *application_type(char *url){
    const char *dot = strrchr(url, '.');
    if ( !dot || dot == url) return "";

    if (strcmp(dot, "html")) {
	return "text/html";
    }
    else if (strcmp(dot, "xml") == 0){
	return "application/xml";
    }
    else if (strcmp(dot, "js") == 0){
	return "application/javascript";
    }
    else if (strcmp(dot, "css") == 0){
	return "text/css";
    }
    else if (strcmp(dot, "jpg") == 0){
	return "image/jpeg";
    }
    else if (strcmp(dot, "png") == 0){
	return "image/png";
    }
    return "text";
}
