const char *reason_phrase(int code);
void send_status(FILE *client, int code);
void send_response(FILE *client, int code, const char *phrase, const char *message_body);
