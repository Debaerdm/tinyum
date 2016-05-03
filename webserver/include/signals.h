#ifndef SIGNALS_H
#define SIGNALS_H

/**
 * initialize_signals - fill signaction struct with current information.
 */
void initialize_signals(void);

/**
 * handler - wait after dead chilld/
 */
void handler(int sig);

#endif
