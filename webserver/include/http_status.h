#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

/**
 * @file
 * @brief make a status and sentence with the code http
 */

/**
 * @fn const char *reason_phrase(int code)
 * @brief return specifiy sentence by given code.
 *
 * @param code - The HTTP code
 *
 * @return if the HTTP code exist return the apropriate sentence, Otherwise NULL
 * is returned.
 */
const char *reason_phrase(int code);

/**
 * @fn void send_status(FILE *client, int code) 
 * @brief send status to client.
 *
 * @param client - The input stream of the client.
 * @param code - The code to be send to the client.
 *
 */
void send_status(FILE *client, int code);

/**
 * @fn void send_response(FILE *client, int code, const char *message_body) 
 * @brief send reponse to the client/
 *
 * @param client - the input stream of the client.
 * @param code - The code to be send to the client.
 * @param message_body - The message to be send to the client.
 *
 */
void send_response(FILE *client, int code, const char *message_body);

#endif
