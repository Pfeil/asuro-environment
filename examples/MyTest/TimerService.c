#include "TimerService.h"
#include "asuro.h"
#include <avr/interrupt.h> // ISR(), cli(), sei()

void (*functions[1]) (void);
int func_interval[1];
volatile int call_number = 0;
//volatile func_data function[FUNC_NUM];
int func_amount = 0;

// TODO wie setze ich das 1ms intervall?
void initTimer(void) {
	cli();
	TCCR0 &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TCCR0 = ((1<<CS01) | (1<<CS00));
	TIMSK = ((1<<TOIE0));
	sei();
}

// TODO nur funktionen aufrufen die gesetzt sind
void timer0_overflow(void) {
	call_number += 1;
	SerPrint("\n\rInterrupt Number: ");
	PrintInt(call_number);
	SerPrint("\n\r");
	int i;
	for (i = 0; i<func_amount; i++) {
		if (call_number % func_interval[i] == 0) {
			SerPrint("\n\rStart Function\n\r");
			(*functions[i])();
			SerPrint("\n\rEnd Function\n\r");
		}
	}
	if (call_number >= 124) { call_number = 0; }
}

ISR(TIMER0_OVF_vect){ timer0_overflow(); }

void TSinitTimerService(int amount) {
	/*int i;
	for (i = 0; i < FUNC_NUM; i++) {
		function[i].fp = 0;
	}*/
	call_number = amount;
	void (*tmp_fkts[func_amount]) (void);
	*functions = *tmp_fkts;
	
	int tmp_interval[func_amount];
	*func_interval = *tmp_interval;
	
	call_number = 0;
	initTimer();
}

// TODO id < func_amount, interval <= 255
void TSaddFunction(int id, void* func_ptr, int interval) {
	functions[id] = func_ptr;
	func_interval[id] = interval;
}