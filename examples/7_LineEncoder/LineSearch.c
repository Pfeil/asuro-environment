#include "LineSearch.h"
#include "Asuro.h"
#include "TimerService.h"
#include "MyUtils.h"

#define SPEED 120 // will be multiplied with 2!
#define LSPEED 40
#define M_WINDOW_SIZE 5 /* default 5 */
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

enum state currentState;

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
}

inline enum state state_right_fkt(void) {
//	SetMotorPower(SPEED, 0);
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
//	SetMotorPower(0, SPEED);
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
	SetMotorPower(0,0);
//	BackLED(OFF,OFF);
	return STATE_HALT;
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
			return 0;
			break;
	}
	return 1;
}
