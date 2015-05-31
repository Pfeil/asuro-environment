/*******************************************************************************
*
* File Name:   main.c
* Project  :   SelfTest + Demo
*
* Description:
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   14.08.2003   Jan Grewe		 build
*
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 ***************************************************************************/
#include <avr/interrupt.h> //ISR(), cli(), sei()

#include "asuro.h" // Init(), NULL, Motor*(), ...
#include "TimerService.h" // ts_init(), ts_addFunction()

unsigned int data[] = {0,0};
void fun1(void) {
	LineData(data);
	//print
	PrintInt(data[0]);
	SerPrint(";");
	PrintInt(data[1]);
	SerPrint("\n\r");
}

int main (void)
{
	Init();
	ts_init();
	
	FrontLED(ON);
	void (*pfun1) (void) = &fun1;
	ts_addFunction(pfun1,100);
	
	//GoTurn(1000,0,100);
	unsigned char t1, t2, LEDl, LEDr;
	while(1)
	{
		t1 = PollSwitch();
		t2 = PollSwitch();
		t1 = PollSwitch();
		
		if (t1 == 0 && t2 == 0) {
			BackLED(OFF,OFF);
		} else if (t1 && t2 && t1 == t2) {
			if (t1 == 0x01) {
				BackLED(ON,OFF);
			} else if (t1 == 0x20) {
				BackLED(OFF,ON);
			}
		}
	}
}