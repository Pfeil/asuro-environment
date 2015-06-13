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

inline void initStuff(void) {
	Init();
	//EncoderInit();
	//ts_init();
}

int main (void)
{
	initStuff();
	bcr_initBarcodeReader();
	int n = bcr_scanLines(4);
	util_intToBeep(n);
	bcr_cleanBCR();

	int m = 0;
	while(0)
	{
		m = changeMetric();
		PrintInt(m);
		Msleep(5);
		SerPrint("\n\r");
		Msleep(5);
	}
	Msleep(1000);
	SerPrint("-- END OF LINE --\n\r");
	return 0;
}