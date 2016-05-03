#ifndef STATS_H
#define STATS_H

#include <string.h>

/**
 * Typedef have the statistics of connections into the server
 */
typedef struct
{
  int served_connections;
  int served_requests;
  int ok_200;
  int ko_400;
  int ko_403;
  int ko_404;
  int ko_405;
} web_stats;

/**
 * replace_variable - open and check the files for replaces any %d with 
 * the statitics from server
 *
 * @param client - For write into the descriptor of client
 * @param uri - The path of files into the server folder
 *
 */
void replace_variable(FILE *client, char* uri);

/**
 * send_stats - send the result of replace_variable and set the content-type and send 
 * the data to the client
 *
 * @param client - The descriptor of file for write any data for send to the client
 * @param uri - The path of files into the server folder
 *
 */
void send_stats(FILE *client, char *uri);

/**
 * init_stats - map the statitics into the memory for the mutli access into many process
 *
 * @return On success the function return 0
 */
int init_stats(void);

/**
 * get_stats - used for have once instance of web_stats
 *
 * @return The struct of web_stats with the statitics
 */
web_stats *get_stats(void);

#endif
