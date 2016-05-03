#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string.h>
#include <ctype.h>

#define CR '\n'
#define LF '\r'
#define OUT 0
#define IN 1

/**
 * The set of common methods for HTTP/1.1.
 */
enum http_method {
  HTTP_OPTIONS,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE,
  HTTP_TRACE,
  HTTP_CONNECT,
  HTTP_INVALID
};

/* Request-Line = Method SP Request-URI SP HTTP-Version CRLF */
/**
 * Typedef to define how to store HTTP informations.
 */
typedef struct {
  enum http_method m;
  int major_version;
  int minor_version;
  char uri[512];
} http_request;

/**
 * words - count the number of words in a sentence.
 *
 * @param s - the sentence that we want to count words.
 *
 * @return The number of words.
 */
int words(const char* s);

/**
 * append - appends the string representation if the char argument to this
 * sequence.
 *
 * @param s - the sentence which must be added a character.
 * @param size - the size of the sentence.
 * @param c - the character to be added.
 *
 * @return On succes the append function return 1, Otherwise 0 is returned.
 */
int append(char *s, size_t size, char c);

/**
 * fgets_or_exit - input of strings or exit.
 *
 * @param buf - The buffer that'll contain the characters.
 * @param size - The numbers of characters to read.
 * @param stream - The sequance of bytes to read.
 *
 * @return On success, fgets_or_exit functions return the buffer, Otherwise
 * error is returned.
 */
char *fgets_or_exit(char *buf, int size, FILE *stream);

/**
 * skip_headers - avoid sequence of line in stream.
 *
 * @param client - The stream to jump at the end.
 */
void skip_headers(FILE *client);

/**
 * get_file_size - gives the size of the file in bytes.
 *
 * @param fildes - The file descriptor of the file whose size is desired.
 *
 * @return On successn the size in bytes is returned. On errorn 1 is returned.
 */
int get_file_size(int fd);

/**
 * read_http_header - extract informations from lines.
 *
 * @param line - The line to be analyzed.
 * @param request - The http_request struct to store informations.
 *
 * @return On success the read_http_header function return 1, Otherwise 0 is
 * returned.
 */
int read_http_header(const char*, http_request *);

#endif
