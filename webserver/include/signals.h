#ifndef SIGNALS_H
#define SIGNALS_H

/**
 * @file
 * @brief signal for kill child if this are a zombie
 */

/**
 * @fn void initialize_signals(void)
 * @brief fill signaction struct with current information.
 */
void initialize_signals(void);

/**
 * @fn void handler(int sig)
 * @brief wait after dead chilld/
 */
void handler(int sig);

#endif
