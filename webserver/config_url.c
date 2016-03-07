#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

const char  *rewrite_url(char *url){
    if(strcmp(url, "/") == 0) {
	return "/index.html";
    }
    printf("%s\n", url);
    char *tmp = strchr(url, '?');
    if (tmp == NULL) {
	return url;
    }
    url[(tmp - url)] = '\0';
    return url;
}

int check_and_open(const char *url, const char *document_root){
    char path[strlen(url) + strlen(document_root) + 1];
    sprintf(path, "%s%s", url, document_root);
    struct stat conf;
    stat(path, &conf);
    if (S_ISREG(conf.st_mode)) {
	return open(path, O_RDONLY);
    }
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
    char *src, *dst;
    /* go to the location corresponding to the last byte */
    if (lseek(out, (get_file_size(in) - 1), SEEK_SET) == -1) {
        perror("lseek");
        return EXIT_FAILURE;
    }

    /* write a dummy byte at the last location */
    if (write(out, "", 1) != 1) {
        perror("write");
        return EXIT_FAILURE;
    }

    if ((src = mmap(0, get_file_size(in), PROT_READ, MAP_SHARED, in, 0)) == (caddr_t) -1) {
        perror("mmap error for input");
        return EXIT_FAILURE;
    }

    if ((dst = mmap(0, get_file_size(in), PROT_READ | PROT_WRITE, MAP_SHARED, out, 0)) == (caddr_t) -1) {
        perror("mmap error for output");
        return EXIT_FAILURE;
    }

    memcpy(dst, src, get_file_size(in));
    
    return EXIT_SUCCESS;
}
