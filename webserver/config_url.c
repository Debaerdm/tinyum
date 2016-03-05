#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const char  *rewrite_url(char *url){
    if(strcmp(url, "/") == 0){
	return "/index.html";
    }
    printf("%s\n", url);
    char *tmp = strchr(url, '?');
    if(tmp == NULL){
	return url;
    }
    url[(tmp - url)] = '\0';
    return url;
}

int check_and_open(const char *url, const char *document_root){
    char path[strlen(url)+strlen(document_root)+1];
    sprintf(path, "%s%s", url, document_root);
    struct stat conf;
    stat(path, &conf);
    if(S_ISREG(conf.st_mode)){
	return open(path, O_RDONLY);
    }
    return -1;
}
