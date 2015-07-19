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

	char gogogo = 0;
	unsigned char d = 0;
	do {
		gogogo = lis_followLine();
		d = util_getCollisions();
		if (d != 0) {
			gogogo = 0;
		}
	} while (gogogo != 0);

	lis_clean();
}

void wallbounce(int direction) {
//	wb_init();
	// lis_init();
	lis_initLineDetector();

	// do a few bounces to get away from line
	unsigned char d = 0;
	int i;
	for (i=0; i>3; i++) {
		wb_bounce(direction);
		do {
			d = util_getCollisions();
		} while (d==0);
	}

	int gogogo = 0;
	char l = 0;
	do {
		gogogo = wb_bounce(direction);
		do {
			d = util_getCollisions();
			l = lis_tickLineDetector();
			if (l==1) {
				d = 1;
				gogogo = 0;
			}
		} while (d==0);
		// util_intToBeep(2);
	} while (gogogo == 1);
	lis_clean();
//	wb_clean();
}

int main (void)
{
	Init();
	EncoderInit();
	SerPrint("START");

	// follow the line
	StatusLED(RED);
	Msleep(100);
	followLine();
	StatusLED(GREEN);
	MotorSpeed(0,0);
	MotorDir(FWD,FWD);

	// eventually drive to wall
	SetMotorPower(40,40);
	int d;
	do {
		StatusLED(RED);
		d = util_getCollisions();
		Msleep(100);
	} while (d == 0);
		StatusLED(GREEN);
		MotorSpeed(0,0);

	// follow the wall, catch the line
	wallbounce(LEFT);
	util_intToBeep(3);

	// line was found. now find and follow it:
	GoTurn(40,0,90);
	lis_initLineDetector();
	SetMotorPower(60,(-60));
	int l = 0;
	do {
		l = lis_tickLineDetector();
	} while (l == 0);

	// follow the line
	followLine();

	// scan barcode
	scanLines(3);
	FrontLED(OFF);

	return 0;
}
