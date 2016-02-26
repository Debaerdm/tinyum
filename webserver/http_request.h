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
     
int read_http_request(const char*, http_request *);
