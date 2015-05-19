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
#include <avr/io.h>
#include <inttypes.h>

#include "asuro.h"
//#include "TimerService.h"

volatile int lights_on = 0;
void fun1(void) {
	SerPrint("\n\rfun1 number: ");
	PrintInt(lights_on);
	SerPrint("\n\r");
	
	if (lights_on == 0) {
		StatusLED(YELLOW);
		BackLED(ON,OFF);
		lights_on = 1;
	} else {
		StatusLED(GREEN);
		BackLED(OFF,ON);
		lights_on = 0;
	}
}

int main (void)
{
	Init();
	
	//TSinitTimerService(1);
	//void (*pfun1) (void) = &fun1;
	//TSaddFunction(0, pfun1, 10);
	
	// teste taster
	unsigned int data[] = {0,0};
	MotorDir(FWD,FWD);
	//MotorSpeed(50,50);
	while(1)
	{
		//fetch data
		//LineData(data);
		OdometryData(data);
		//print
		PrintInt(data[0]);
		SerPrint(";");
		PrintInt(data[1]);
		SerPrint("\n\r");
		//sleep
		Msleep(25);
	}
}


