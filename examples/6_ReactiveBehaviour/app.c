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

#define MIN_SPEED 70
#define MAX_SPEED 150
#define LIGHT_FAULT 85
//#define GRADIENT 0.31875

unsigned int light[2];
unsigned char dampers;
// output variables
unsigned char speed[2];
unsigned char direction[2];

// for LEFT line data
inline int line_correction(int linedata) {
	float x = linedata / 1024.0;
	return linedata + (LIGHT_FAULT * 4 * x * (x-1));
}

inline int lightToSpeed(int light)
{
	float x = light / 1024.0;
	return MIN_SPEED + (MAX_SPEED - MIN_SPEED) * (1-x);
}

void handle_light(void)
{
	light[LEFT] = line_correction(light[LEFT]);
	if (light[LEFT] > light[RIGHT]) {
		MotorDir(FWD,FWD);
		speed[RIGHT] = lightToSpeed(light[RIGHT]);
		speed[LEFT] = MIN_SPEED;
		//BackLED(ON,OFF);
	} else {
		MotorDir(FWD,FWD);
		speed[LEFT] = lightToSpeed(light[LEFT]);
		speed[RIGHT] = MIN_SPEED;
		//BackLED(OFF,ON);
	}
}

void fetchData(void) {
	dampers = 0; //just trying
	dampers  = PollSwitch() & 0b00111111;
	Msleep(5);
	dampers &= PollSwitch() & 0b00111111;
	Msleep(5);
	dampers &= PollSwitch() & 0b00111111;
	Msleep(5);
	dampers &= PollSwitch() & 0b00111111;
	LineData(light);
}

//void handle_dampers(void) {
//	int outer = 
//	if (dampers >= 0b00100001 || dampers & 0b00010010) {
//		
//	}
//	Go(-100, 90);
//	Turn(90, 90);
//	Msleep(10);
//}

void decide(void) {
	//if (dampers) { handle_dampers(); }
	handle_light();
}

void act(void) {
	MotorDir(FWD,FWD);
	MotorSpeed(speed[LEFT], speed[RIGHT]);
}

void debugLineData(void) {
	unsigned short delay = 5;
	SerPrint("a,");
	Msleep(delay);
	PrintInt(light[LEFT]);
	Msleep(delay);
	SerPrint(",");
	Msleep(delay);
	PrintInt(light[RIGHT]);
	Msleep(delay);
	SerPrint("\n");
}


unsigned int ododata[2];

void debugOdoData(void) {
	OdometryData(ododata);
	PrintInt(ododata[0]);
	SerPrint(";");
	PrintInt(ododata[1]);
	SerPrint("\n\r");
}

int main (void)
{
	Init();
	EncoderInit();
	//FrontLED(ON);
	//MotorDir(FWD,FWD);
	//MotorSpeed(80,80);
	while(1)
	{
		fetchData();
		//debugOdoData();
		//debugLineData();
		decide();
		act();
		//Msleep(5);
	}
}