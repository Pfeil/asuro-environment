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
#include <stdlib.h>

#include "asuro.h" // Init(), NULL, Motor*(), ...
#include "TimerService.h" // ts_init(), ts_addFunction()
#include "BarcodeReader.h"
#include "LineSearch.h"
#include "WallBounce.h"
#include "MyUtils.h"

// keep as usage example
int scanLines(int num) {
	//Init();
	bcr_initBarcodeReader();
	int n = bcr_scanLines(num);
	bcr_cleanBCR();
	util_intToBeep(n);
	return n;
}

void followLine(void) {
	lis_init();
	
	int gogogo = 0;
	do {
		gogogo = lis_followLine();
	} while (gogogo == 1);
	
	lis_clean();
}

void wallbounce(int direction) {
//	wb_init();
	
	int gogogo = 0;
	do {
		gogogo = wb_bounce(direction);
	} while (gogogo == 1);
	
//	wb_clean();
}

int main (void)
{
	Init();
	EncoderInit();
//	followLine();
//	wallbounce(LEFT);
	followLine();
	Msleep(500);
	scanLines(3);
	Msleep(500);
	
	return 0;
}