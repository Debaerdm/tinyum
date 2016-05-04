#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string.h>
#include <ctype.h>

/**
 * @file
 * @brief Http method for parse the header in the content of request 
 */

/*! @def LF
 *  @brief A macro for Line Feed 
 */
#define LF '\n'
 /*! @def CR
  *  @brief A macro for Carriage returns
  */
#define CR '\r'
 /*! @def OUT
  *  @brief A macro for stdout
  */
#define OUT 0
 /*! @def IN
  *  @brief A macro for stdin
  */
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
 * @brief To define how to store HTTP informations.
 */
typedef struct {
  enum http_method m; ///< The set of common methods for HTTP/1.1
  int major_version; ///< The version of max value of HTTP/1.1
  int minor_version; ///< The version of minir value of HTTP/1.1
  char uri[512]; ///< The uri used for path to navigate into the server
} http_request; ///< To define how to store HTTP informations.

/**
 * @fn int words(const char* s) 
 * @brief count the number of words in a sentence.
 *
 * @param s - the sentence that we want to count words.
 *
 * @return The number of words.
 */
int words(const char* s);

/**
 * @fn int append(char *s, size_t, char c) 
 * @ brief appends the string representation if the char argument to this
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
 * @fn char fgets_or_exit(char *buf, int size, FILE *stream)
 * @brief input of strings or exit.
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
 * @fn void skip_headers(FILE *client)
 * @brief avoid sequence of line in stream.
 *
 * @param client - The stream to jump at the end.
 */
void skip_headers(FILE *client);

/**
 * @fn int get_file_size(int fd) 
 * @brief gives the size of the file in bytes.
 *
 * @param fd - The file descriptor of the file whose size is desired.
 *
 * @return On successn the size in bytes is returned. On error 1 is returned.
 */
int get_file_size(int fd);

/**
 * @fn int read_http_header(const char* line, http_request* request)
 * @brief extract informations from lines.
 *
 * @param line - The line to be analyzed.
 * @param request - The http_request struct to store informations.
 *
 * @return On success the read_http_header function return 1, Otherwise 0 is
 * returned.
 */
int read_http_header(const char* line, http_request* request);

#endif
