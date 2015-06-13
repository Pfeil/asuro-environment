#include "BarcodeReader.h"
#include "TimerService.h"
#include "MyUtils.h"
#include "Asuro.h"

#define TIMESTEP 20 //counter increment (default 5, so timer+=5 every 5ms)
#define READ_SPEED 80
#define M_WINDOW_SIZE 10 //in measurements / data points
#define M_SAMPLING_FREQ 1 //in milliseconds
#define M_RAW_DATAPOINTS 3 //in measurements / data points
#define M_TOL 80 // metric tolerance

unsigned int timediff;
volatile unsigned int timer;
unsigned char lineCounter;
volatile unsigned int lineData[M_WINDOW_SIZE];
volatile unsigned int latestLineData;

enum state {
	BCR_DIMMING_STATE,
	BCR_BRIGHTEN_STATE
};

enum state currentState;


inline void setTime(unsigned int x) {
	cli();
	timer = x;
	sei();
}

inline unsigned int getTime(void) {
	cli();
	unsigned int result = timer;
	sei();
	return result;
}

void count(void) {
	timer += TIMESTEP;
}

volatile unsigned int tmpMeasures[M_RAW_DATAPOINTS];
volatile unsigned int rawCounter;

void measureDataPoint(void) {
	unsigned int tmp[2];
	LineData(tmp);
	tmpMeasures[rawCounter] = util_lineCorrection(tmp[LEFT]);
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

int bcr_getGradient(void) {
//	cli();
	int deltaY = lineData[latestLineData] - lineData[(latestLineData+1) % M_WINDOW_SIZE];
//	sei();
	// instead of dividing, I made the tolerance bigger.
	return deltaY /* / M_WINDOW_SIZE*/;
}

void bcr_cleanBCR(void) {
	// TODO erase services from timerservice
}

void bcr_initBarcodeReader(void) {
	EncoderInit();
	MotorDir(FWD,FWD);
	FrontLED(ON);
	
	lineCounter = 0;
	rawCounter = 0;
	latestLineData = 0;
	
	int i;
	util_pauseInterrupts();
	for(i = 0; i < (M_RAW_DATAPOINTS * M_WINDOW_SIZE); i++) {
		measureDataPoint();
		Msleep(5);
	}
	
	timer = 0;
	ts_init();
	ts_addFunction(&count, TIMESTEP);
	ts_addFunction(&measureDataPoint, M_SAMPLING_FREQ);
	sei();
}


int bcr_scanLines(unsigned char num)
{
	// assume we stand on white
	currentState = BCR_DIMMING_STATE;
	MotorSpeed(READ_SPEED, READ_SPEED);
	
	while(1) {
		switch (currentState)
		{
			case BCR_DIMMING_STATE:
				setTime(0);
				while (bcr_getGradient() > -1*M_TOL) {
					//if (timer > 2*timediff) { return lineCounter; } // TODO lieber getTime() nehmen?
				}
				currentState = BCR_BRIGHTEN_STATE;
				lineCounter += 1;
				break;
			case BCR_BRIGHTEN_STATE:
				setTime(0);
				while (bcr_getGradient() < M_TOL) {
					//if (timer > 2*timediff) { return lineCounter; } // TODO lieber getTime() nehmen?
				}
				currentState = BCR_DIMMING_STATE;
				break;
		}
		if (lineCounter>0 && lineCounter >= num) {
			MotorDir(BREAK, BREAK);
			MotorSpeed(0,0);
			break;
		}
	}
	MotorSpeed(0,0);
	return lineCounter;
}