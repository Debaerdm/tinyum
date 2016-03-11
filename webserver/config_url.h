const char  *rewrite_url(char *url, FILE *client);
int check_and_open(const char *url, const char *document_root);
int get_file_size(int fildes);
int copy(int in, int out);
const char *application_type(char *url);
int url_valid(char *url);
