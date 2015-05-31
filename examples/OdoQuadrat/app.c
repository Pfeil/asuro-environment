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
	
}

int main (void)
{
	Init();
	//ts_init();
	EncoderInit();
	
	SerPrint("Next: Go\n\r");
	GoTurn(300,  0,  100);
	SerPrint("Next: Turn\n\r");
	GoTurn(  0, 90,  90);
	SerPrint("Next: Go\n\r");
	GoTurn(300,  0,  100);
	SerPrint("Next: Turn\n\r");
	GoTurn(  0, 90,  90);
	SerPrint("Next: Go\n\r");
	GoTurn(300,  0,  100);
	SerPrint("Next: Turn\n\r");
	GoTurn(  0, 90,  90);
	SerPrint("Next: Go\n\r");
	GoTurn(300,  0,  100);
	SerPrint("Next: Turn\n\r");
	GoTurn(  0, 90,  90);
	
	while(1) {
		// do nothing
	}
	
	//void (*pfun1) (void) = &fun1;
	//ts_addFunction(pfun1,100);
	return 0;
}


