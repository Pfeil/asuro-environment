#ifndef TIMERSERVICE_H
#define TIMERSERVICE_H

#define FUNC_NUM 3

typedef struct {
   void (*fp) (void);
   int interval;
   int call_num;
} func_data;

/**
 * @brief Set up Timer, set up arrays.
 */
void TSinitTimerService(int amount);

/**
 * @brief Register function to execute every interval_ms milliseconds.
 */
void TSaddFunction(int func_id, void* func_ptr, int interval_ms);

#endif //TIMERSERVICE_H
//--end of file--//