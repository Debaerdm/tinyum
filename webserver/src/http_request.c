#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/http_request.h"
#include "../include/config_url.h"

#define CR '\n'
#define LF '\r'
#define OUT 0
#define IN 1
#define BUFFER_SIZE 1024

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

/*
 * Keeps the term that we have treated
 */
enum state
{
    s_start,
    s_method,
    s_space_before_uri,
    s_uri,
    s_http_correct,
    s_http_H,
    s_http_HT,
    s_http_HTT,
    s_http_HTTP,
    s_http_first_major_version,
    s_http_major_version,
    s_http_first_minor_version,
    s_http_minor_version,
    s_header_done,
};

/*
 * words - return numbers of words in string
 */
int words(const char* s)
{
    unsigned int index = 0;
    const char *ch = s;
    int state = OUT;

    while (*ch) {
        if (isspace(*ch))
            state = OUT;
        else if (state == OUT) {
            state = IN;
            ++index;
        }

        ++ch;
    }

    return index;
}

/*
 * append - Appends the string representation of the char argument to this sequence. 
 */
int append(char* s, size_t size, char c) {
    if(strlen(s) + 1 >= size)
        return EXIT_FAILURE;

    int len = strlen(s);
    s[len] = c;
    s[(len + 1)] = '\0';

    return EXIT_SUCCESS;
}

char *fgets_or_exit(char *buf, int size, FILE *stream){
    if (fgets(buf, size, stream) == NULL) {
        exit(1);
    }
    return buf;
}

void skip_headers(FILE *client){
    char buf[BUFFER_SIZE];
    while((fgets_or_exit(buf, sizeof(buf), client) != NULL && strcmp(buf, "\n") && strcmp(buf, "\r\n") != 0));
}

/*
 * read_requesthdrs - read and parse HTTP request headers
 */
int read_http_header(const char* line, http_request *r)
{
    int pos, last = strlen(line), current_state = s_start;
    char ch;
    char s[strlen(line)];
    memset(s, 0, strlen(line));

    for (pos = 0; pos < last; ++pos) {
        ch = line[pos];

        switch (current_state) {

            case s_start:
                if (words(line) != 3) {
                    r->m = HTTP_INVALID;
                    current_state = s_header_done;
                    return EXIT_FAILURE;
                }

                if (ch == LF || ch == CR)
                    break;

                if ((ch < 'A' || ch > 'Z') && ch != '_') {
                    r->m = HTTP_INVALID;
                    current_state = s_header_done;
                    return EXIT_FAILURE;
                }

                current_state = s_method;
                break;

            case s_method:
                if (ch == ' ') {
                    switch (pos) {
                        case 3:
                            if (strncmp(line, "GET", 3) == 0) {
                                r->m = HTTP_GET;
                                current_state = s_uri;
                                break;
                            }

                            if (strncmp(line, "PUT", 3) == 0) {
                                current_state = s_uri;
                                r->m = HTTP_PUT;
                                break;
                            }

                            break;
                        case 4:
                            if (strncmp(line, "HEAD", 4) == 0) {
                                current_state = s_uri;
                                r->m = HTTP_HEAD;
                                break;
                            }

                            if (strncmp(line, "POST", 4) == 0) {
                                current_state = s_uri;
                                r->m = HTTP_POST;
                                break;
                            }
                            break;
                        case 5:
                            if (strncmp(line, "TRACE", 5) == 0) {
                                current_state = s_uri;
                                r->m = HTTP_TRACE;
                                break;
                            }

                            break;
                        case 6:
                            if (strncmp(line, "DELETE", 6) == 0) {
                                current_state = s_uri;
                                r->m = HTTP_DELETE;
                                break;
                            }

                            break;
                        case 7:
                            if (strncmp(line, "CONNECT", 7) == 0) {
                                r->m = HTTP_CONNECT;
                                break;
                            }

                            if (strncmp(line, "OPTIONS", 7) == 0) {
                                r->m = HTTP_OPTIONS;
                                break;
                            }

                            break;
                    }

                    if (current_state != s_uri) {
                        r->m = HTTP_INVALID;
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                    }
                }

                break;

            case s_uri:
                if (ch == ' ') {
                    strcpy(r->uri, rewrite_url(s));
                    current_state = s_http_correct;
                    break;
                } else {
                    append(s, strlen(line), ch);
                }

                break;

            case s_http_correct:
                switch (ch) {
                    case ' ':
                        break;
                    case CR:
                        current_state = s_header_done;
                        break;
                    case LF:
                        current_state = s_header_done;
                        break;
                    case 'H':
                        current_state = s_http_H;
                        break;
                    default:
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                }

                break;

            case s_http_H:
                switch (ch) {
                    case 'T':
                        current_state = s_http_HT;
                        break;
                    default:
                        r->m = HTTP_INVALID;
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                }

                break;

            case s_http_HT:
                switch (ch) {
                    case 'T':
                        current_state = s_http_HTT;
                        break;
                    default:
                        r->m = HTTP_INVALID;
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                }

                break;

            case s_http_HTT:
                switch (ch) {
                    case 'P':
                        current_state = s_http_HTTP;
                        break;
                    default:
                        r->m = HTTP_INVALID;
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                }

                break;

            case s_http_HTTP:
                switch (ch) {
                    case '/':
                        current_state = s_http_first_major_version;
                        break;
                    default:
                        r->m = HTTP_INVALID;
                        current_state = s_header_done;
                        return EXIT_FAILURE;
                }

                break;

            case s_http_first_major_version:
                if (ch < '1' || ch > '9') {
                    r->m = HTTP_INVALID;
                    current_state = s_header_done;
                    return EXIT_FAILURE;
                }

                r->major_version = atoi(&ch);
                current_state = s_http_major_version;

                break;

            case s_http_major_version:
                if (ch == '.') {
                    current_state = s_http_first_minor_version;
                    break;
                }

                if (ch < '0' || ch > '9') {
                    r->m = HTTP_INVALID;
                    current_state = s_header_done;
                    return EXIT_FAILURE;
                }

                r->major_version = r->major_version * 10 + atoi(&ch);
                break;

            case s_http_first_minor_version:
                if (ch < '0' || ch > '9') {
                    r->m = HTTP_INVALID;
                    current_state = s_header_done;
                    return EXIT_FAILURE;
                }

                r->minor_version = atoi(&ch);
                current_state = s_http_minor_version;
                break;

            case s_http_minor_version:
                if (ch == CR) {
                    current_state = s_header_done;
                    break;
                }

                if (ch == LF) {
                    current_state = s_header_done;
                    break;
                }
        }

    }

    return EXIT_SUCCESS;
}
