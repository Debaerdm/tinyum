#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

/**
 * reason_phrase - return specifiy sentence by given code.
 *
 * @param code - The HTTP code
 *
 * @return if the HTTP code exist return the apropriate sentence, Otherwise NULL
 * is returned.
 */
const char *reason_phrase(int code);

/**
 * send_status - send status to client.
 *
 * @param client - The input stream of the client.
 * @param code - The code to be send to the client.
 *
 */
void send_status(FILE *client, int code);

/**
 * send_response - send reponse to the client/
 *
 * @param client - the input stream of the client.
 * @param code - The code to be send to the client.
 * @param message - The message to be send to the client.
 *
 */
void send_response(FILE *client, int code, const char *message_body);

#endif
