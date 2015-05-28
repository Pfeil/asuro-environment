#ifndef TIMERSERVICE_H
#define TIMERSERVICE_H

#define MAX_SERVICES 5

typedef struct {
   void (*fp) (void);
   int interval;
   int counter;
} func_struct;

/**
 * @brief Set up Timer and scruct array to handle services.
 */
void ts_init(void);

/**
 * @brief Register function to execute every interval_ms milliseconds.
 */
int ts_addFunction(void (*function)(void), unsigned int interval_ms);

#endif //TIMERSERVICE_H
//--end of file--//