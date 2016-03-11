#include <string.h>
#include <ctype.h>

#define CR '\n'
#define LF '\r'
#define OUT 0
#define IN 1

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
typedef struct {
  enum http_method m;
  int major_version;
  int minor_version;
  char uri[512];
} http_request;

int words(const char* s);
int append(char *s, size_t size, char c);
char *fgets_or_exit(char *buf, int size, FILE *stream);
void skip_headers(FILE *client);
int get_file_size(int fd);
int read_http_header(const char*, http_request *);
