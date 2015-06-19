#include "LineSearch.h"
#include "Asuro.h"
#include "TimerService.h"
#include "MyUtils.h"

#define SPEED (100/2)
#define DATA_NUM 2
#define SAMPLING_FREQ 3

enum state {
	LEFTGOING,
	RIGHTGOING
};

enum state currentState;

volatile unsigned int measuresL[DATA_NUM];
volatile unsigned int measuresR[DATA_NUM];
volatile unsigned int mCounter;

void measureLineData(void) {
	unsigned int tmp[2];
	LineData(tmp);
	mCounter = (mCounter+1) % DATA_NUM;
	measuresL[mCounter] = util_lineCorrection(tmp[LEFT]);
	measuresR[mCounter] = tmp[RIGHT];
}

void lis_init(void) {
	FrontLED(ON);
	EncoderInit();
	MotorDir(FWD,FWD);
	currentState = RIGHTGOING;
	mCounter = 0;
	
	int i;
	for(i=0; i<=DATA_NUM; i++) {
		measureLineData();
	}
	
	ts_init();
	ts_addFunction(&measureLineData, SAMPLING_FREQ);
	//sei();
}

void lis_clean(void) {
	ts_removeFunction(&measureLineData);
}

inline void diffData(int *data) {
	data[0] = measuresL[(mCounter+1)%DATA_NUM] - measuresR[(mCounter+1)%DATA_NUM];
	data[1] = measuresL[mCounter] - measuresR[mCounter];
}

void lis_followLine(void)
{
	lis_init();
	int diffOld = measuresL[(mCounter+1)%DATA_NUM] - measuresR[(mCounter+1)%DATA_NUM];
	int diffNew = measuresL[mCounter] - measuresR[mCounter];
	
	while (1) {
		switch (currentState) {
			case RIGHTGOING:
				SetMotorPower(SPEED, 0);
				while (measuresL[mCounter] >= measuresR[mCounter]) {
					BackLED(ON, OFF);
					diffOld = diffNew;
					diffNew = measuresL[mCounter] - measuresR[mCounter];
				}
				currentState = LEFTGOING;
				break;
			case LEFTGOING:
				SetMotorPower(0, SPEED);
				while (measuresL[mCounter] <= measuresR[mCounter]) {
					BackLED(OFF, ON);
					diffOld = diffNew;
					diffNew = measuresL[mCounter] - measuresR[mCounter];
				}
				currentState = RIGHTGOING;
				break;
		}
	}

}