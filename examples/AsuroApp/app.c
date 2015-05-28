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

#include "asuro.h" // Init(), NULL, Motor*()
#include "TimerService.h" // ts_init(), ts_addFunction()

unsigned int data[] = {0,0};
void fun1(void) {
	OdometryData(data);
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
	
	void (*pfun1) (void) = &fun1;
	ts_addFunction(pfun1,100);
	
	MotorDir(FWD,FWD);
	MotorSpeed(90,90);
	while(1)
	{
		// run app
	}
}


