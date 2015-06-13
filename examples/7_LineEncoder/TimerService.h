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

/**
 * @brief Remove all instances containing the given function pointer.
 * 
 * This means that if you have a function fun(void) it is always recommended
 * to give &fun as a parameter for both, @fun removeFunction() and @fun addFunction().
 * 
 * @return the amount of found and deleted instances with the same function pointer.
 */
int ts_removeFunction(void (*function)(void));

#endif //TIMERSERVICE_H
//--end of file--//