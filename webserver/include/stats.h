#ifndef STATS_H
#define STATS_H

#include <string.h>

/**
 * @file
 * @brief the statitics of request into the server
 */

/**
 * @brief typedef have the statistics of connections into the server
 */
typedef struct
{
  int served_connections; ///< Numbers of connection into the server
  int served_requests; ///< Numbers of request send to the server
  int ok_200; ///< The request with status code 200 (ok)
  int ko_400; ///< The request with status code 400 (bad_request)
  int ko_403; ///< The request with status code 403 (forbidden)
  int ko_404; ///< The request with status code 404 (not found)
  int ko_405; ///< The request with status code 405 (method not allowed)
} web_stats; ///< typedef have the statistics of connections into the server

/**
 * @fn void replace_variable(FILE *client, char* uri)
 * @brief open and check the files for replaces any 'Int formater' with 
 * the statitics from server
 *
 * @param client - For write into the descriptor of client
 * @param uri - The path of files into the server folder
 *
 */
void replace_variable(FILE *client, char* uri);

/**
 * @fn void send_stats(FILE *client, char *uri)
 * @brief send the result of replace_variable and set the content-type and send 
 * the data to the client
 *
 * @param client - The descriptor of file for write any data for send to the client
 * @param uri - The path of files into the server folder
 *
 */
void send_stats(FILE *client, char *uri);

/**
 * @fn int init_stats(void)
 * @brief map the statitics into the memory for the mutli access into many process
 *
 * @return On success the function return 0
 */
int init_stats(void);

/**
 * @fn web_stats *get_stats(void) 
 * @brief used for have once instance of web_stats
 *
 * @return The struct of web_stats with the statitics
 */
web_stats *get_stats(void);

#endif
