#include <stdlib.h>
#include <stdio.h>
#include "stats.h"

static web_stats stats;

void send_stats(FILE *client){
    

}

int init_stats(void){
    stats.served_connections = stats.served_requests = stats.ok_200 = stats.ko_400 = stats.ko_403 = stats.ko_404 = stats.ko_405 = 0;
}

web_stats *get_stats(void){
    return &stats;
}
