#include "LineSearch.h"
#include "Asuro.h"
#include "TimerService.h"
#include "MyUtils.h"

#define SPEED 120 // will be multiplied with 2! (currently not)
#define LSPEED 40 // low speed
#define SSPEED 50 // search speed
#define M_WINDOW_SIZE 15 /* default 5 */
#define M_SAMPLING_FREQ 10 /* default 10 */
#define M_RAW_DATAPOINTS 3 /* default 3 */
#define M_TOL (1 * M_SAMPLING_FREQ * M_RAW_DATAPOINTS * M_WINDOW_SIZE) // metric tolerance

volatile unsigned int lineData[M_WINDOW_SIZE];
volatile unsigned int tmpMeasures[M_RAW_DATAPOINTS];
volatile unsigned int rawCounter = 0;
volatile unsigned int latestLineData = 0;


enum state {
	STATE_LEFT,
	STATE_RIGHT,
	STATE_OFFROAD,
	STATE_HALT
};

volatile enum state currentState;

void extractLineData(void) {
	unsigned int tmp[2];
	LineData(tmp);
	tmpMeasures[rawCounter] = util_lineCorrection(tmp[LEFT]) + tmp[RIGHT];
	rawCounter++;;

	if (rawCounter >= M_RAW_DATAPOINTS) {
		rawCounter = 0;
		int i;
		int sum = 0;
		for(i = 0; i < M_RAW_DATAPOINTS; i++) {
			sum += tmpMeasures[i];
		}
		sum /= M_RAW_DATAPOINTS;

		latestLineData = (latestLineData+1) % M_WINDOW_SIZE;
		lineData[latestLineData] = sum;
	}

}

inline int getGradient(void) {
//	cli();
	int deltaY = lineData[latestLineData] - lineData[(latestLineData+1) % M_WINDOW_SIZE];
//	sei();
	/* Instead of dividing though deltaX,
	 * I multiplied the tolerance (M_TOL) with deltaX.
	 * Since this is a polling function, this may have some impact. */
	return deltaY /* / (M_SAMPLING_FREQ * M_RAW_DATAPOINTS * M_WINDOW_SIZE) */;
}

/**
 * @brief interrupt function to detect a line
 */
void lookForLine(void) {
	if (getGradient() <= (-1.2) * M_TOL) {
		//SerPrint("LINE!\n\r");
		SetMotorPower(0,0);
		currentState = STATE_RIGHT;
		MotorDir(FWD, FWD);
	}
}

void lis_init(void) {
	FrontLED(ON);
	EncoderInit();
	MotorDir(FWD,FWD);
	currentState = STATE_RIGHT;

	int i;
	for (i=0; i<=(M_RAW_DATAPOINTS * M_WINDOW_SIZE * 2); i++) {
		extractLineData();
	}

	ts_init();
	ts_addFunction(&extractLineData, M_SAMPLING_FREQ);
}

void lis_clean(void) {
	ts_removeFunction(&extractLineData);
	ts_removeFunction(&lookForLine);
	FrontLED(OFF);
}

inline enum state state_right_fkt(void) {
	SerPrint("R");
//	SetMotorPower(SPEED, 0);
	MotorDir(FWD,FWD);
	MotorSpeed(SPEED,LSPEED);
//	BackLED(ON, OFF);
	unsigned int lData[2];
	LineData(lData);
	enum state result;
	cli();
	if (lData[LEFT] < lData[RIGHT]) {
		result = STATE_LEFT;
	} else {
		result = STATE_RIGHT;
	}
	if (getGradient() >= M_TOL) {
		result = STATE_OFFROAD;
	}
	sei();
	return result;
}

inline enum state state_left_fkt(void) {
	SerPrint("L");
//	SetMotorPower(0, SPEED);
	MotorDir(FWD,FWD);
	MotorSpeed(LSPEED,SPEED);
//	BackLED(OFF,ON);
	unsigned int lData[2];
	LineData(lData);
	enum state result;
	cli();
	if (lData[LEFT] > lData[RIGHT]) {
		result = STATE_RIGHT;
	} else {
		result = STATE_LEFT;
	}
	if (getGradient() >= M_TOL) {
		result = STATE_OFFROAD;
	}
	sei();
	return result;
}

inline enum state state_offroad_fkt(void) {
	SerPrint("O");
	SetMotorPower(0,0);
	MotorDir(FWD, FWD);
	if (lis_findLine() == 1) {
		return STATE_RIGHT;
	} else {
		return STATE_HALT;
	}
}

int lis_followLine(void) {
//	PrintInt(getGradient());
//	SerPrint("\n\r");
	switch (currentState) {
		case STATE_RIGHT:
//			SerPrint("RIGHT\n\r");
			currentState = state_right_fkt();
			break;
		case STATE_LEFT:
//			SerPrint("LEFT\n\r");
			currentState = state_left_fkt();
			break;
		case STATE_OFFROAD:
//			SerPrint("OFF\n\r");
			currentState = state_offroad_fkt();
			break;
		case STATE_HALT:
			SerPrint("H");
			return 0;
			break;
	}
	return 1;
}

void lis_initLineDetector(void) {
	EncoderInit();
	lis_init();
	currentState = STATE_OFFROAD;
	ts_addFunction(&lookForLine, M_SAMPLING_FREQ);
}

int lis_tickLineDetector(void) {
	if (currentState == STATE_RIGHT) {
		return 1;
	} else {
		return 0;
	}
}

int lis_findLine(void) {
	EncoderInit();
	ts_addFunction(&lookForLine, M_SAMPLING_FREQ);
	StatusLED(RED);
	const unsigned int range = 30;

	if (currentState == STATE_OFFROAD) {
		EncoderSet(0,0);
		SetMotorPower((int8_t) (-SSPEED), SSPEED);
		while ((encoder[RIGHT] < range)    && (currentState != STATE_RIGHT)) {}
		SetMotorPower(0,0);
		// PrintInt(encoder[RIGHT]);
		// Msleep(100);
		// SerPrint(" - 1\n\r");
	}

	if (currentState == STATE_OFFROAD) {
		EncoderSet(0,0);
		SetMotorPower(SSPEED, (int8_t) (-SSPEED));
		while ((encoder[LEFT] < (2*range)) && (currentState != STATE_RIGHT)) {}
		SetMotorPower(0,0);
		// PrintInt(encoder[LEFT]);
		// SerPrint(" - 2\n\r");
	}

	if (currentState == STATE_OFFROAD) {
		EncoderSet(0,0);
		SetMotorPower((int8_t) (-SSPEED), SSPEED);
		while ((encoder[RIGHT] < range)    && (currentState != STATE_RIGHT)) {}
		SetMotorPower(0,0);
		// PrintInt(encoder[RIGHT]);
		// SerPrint(" - 3\n\r");
	}


	MotorDir(FWD, FWD);
	ts_removeFunction(&lookForLine);
	StatusLED(GREEN);
	if (currentState != STATE_OFFROAD) {
		return 1;
	} else {
		return 0;
	}
}
