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
#include "MyUtils.h"

// keep as usage example
void scanLines(int num) {
	//Init();
	bcr_initBarcodeReader();
	int n = bcr_scanLines(num);
	bcr_cleanBCR();
	util_intToBeep(n);
}

int main (void)
{
	Init();
	
	int i;
	for (i=0; i<1; i++) {
		scanLines(0);
		Msleep(500);
	}
	
	return 0;
}