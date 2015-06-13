#include "TimerService.h"
#include "asuro.h" // NULL
#include <avr/interrupt.h> // ISR(), cli(), sei()

static volatile func_struct set_srv[MAX_SERVICES];
char initialized = 0;

void timer0_overflow(void) {
	int i;
	for (i=0; i<MAX_SERVICES; i++) {
		if (set_srv[i].fp != NULL) {
			set_srv[i].counter++;
			if (set_srv[i].interval == set_srv[i].counter) {
				set_srv[i].counter = 0;
				(*set_srv[i].fp)();
			}
		}
	}
}

ISR(TIMER0_OVF_vect){ timer0_overflow(); }

int findFreeSlot(void) {
	unsigned char i;
	int h = -1;
	for (i=0; i<MAX_SERVICES; i++) { // search for a free entry
		if (set_srv[i].fp == NULL) // free entry found!
		{
			h = i;
			break;
		}
	}
	return h;
}

void ts_init(void) {
	if (initialized != 1) {
		// init empty structs
		int i;
		for (i=0; i<MAX_SERVICES; i++)
		{
			set_srv[i].fp = NULL;
			set_srv[i].interval = 0;
			set_srv[i].counter = 0;
		}

		// init timer
		cli();
		TCCR0 = (1<<CS01);
		TIMSK |= ((1<<TOIE0));
		sei();
		initialized = 1;
	}
}

int ts_addFunction(void (*function)(void), unsigned int interval) {
	int handle = findFreeSlot();
	unsigned char tmpSREG = SREG;
	cli();
	if ( handle >= 0 ) // if location found, set it up
	{
		set_srv[handle].fp = function;
		set_srv[handle].interval = interval*4;
		set_srv[handle].counter = 0;
	}
	return handle;
	SREG = tmpSREG;
}

int ts_removeFunction(void (*function)(void)) {
	int rmFunctions = 0;
	int i;
	for (i=0; i<MAX_SERVICES; i++) {
		if (set_srv[i].fp == function) {
			set_srv[i].fp = NULL;
			set_srv[i].counter = 0;
			set_srv[i].interval = 0;
			rmFunctions++;
		}
	}
	return rmFunctions;
}