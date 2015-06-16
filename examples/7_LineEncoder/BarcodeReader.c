#include "BarcodeReader.h"
#include "TimerService.h"
#include "MyUtils.h"
#include "Asuro.h"

#define READ_SPEED 80
#define M_WINDOW_SIZE 30 //in measurements / data points (default 30)
#define M_SAMPLING_FREQ 1 //in milliseconds (default 1)
#define M_RAW_DATAPOINTS 3 //in measurements / data points (default 3)
#define M_TOL (5 + M_SAMPLING_FREQ * M_RAW_DATAPOINTS * M_WINDOW_SIZE) // metric tolerance

unsigned char lineCounter;
volatile unsigned int lineData[M_WINDOW_SIZE];
volatile unsigned int latestLineData;

enum state {
	BCR_DIMMING_STATE,
	BCR_BRIGHTEN_STATE
};

enum state currentState;

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
	/* Instead of dividing though deltaX,
	 * I multiplied the tolerance (M_TOL) with M_WINDOW_SIZE.
	 * Since this is a polling function, this may have some impact. */
	return deltaY /* / (M_SAMPLING_FREQ * M_RAW_DATAPOINTS * M_WINDOW_SIZE) */;
}

void bcr_cleanBCR(void) {
	util_pauseInterrupts();
	ts_removeFunction(&measureDataPoint);
	util_recoverInterruptState();
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
	
	ts_init();
	ts_addFunction(&measureDataPoint, M_SAMPLING_FREQ);
	sei();
}

int bcr_scanLines(unsigned int num) {
	return bcr_scanIrregularLines(num, 0);
}

int bcr_scanIrregularLines(unsigned char num, unsigned int spacing)
{
	if (spacing < 2) { spacing = 2; } /* default buffer */
	/* maxlength = 100 means that it will drive max. 100 ticks
	 * to find the first bar! Later in the code it will contain
	 * the regular space between the bars. */
	unsigned int whiteLength = 200;
	unsigned int blackLength = 200;
	lineCounter = 0;
	
	/* assume we stand on white (or not completely dark) */
	currentState = BCR_DIMMING_STATE;
	EncoderSet(0,0);
	MotorSpeed(READ_SPEED, READ_SPEED);
	
	while(1) {
		switch (currentState)
		{
			case BCR_DIMMING_STATE:
				EncoderSet(0,0);
				while (bcr_getGradient() >= -1*M_TOL) {
					if ( encoder[LEFT] > (whiteLength+spacing) ) {
						MotorDir(BREAK, BREAK);
						MotorSpeed(0,0);
						return lineCounter;
					}
				}
				
				if (lineCounter >= 1) { whiteLength = encoder[LEFT]; }
				currentState = BCR_BRIGHTEN_STATE;
				lineCounter += 1;
				break;

			case BCR_BRIGHTEN_STATE:
				EncoderSet(0,0);
				while (bcr_getGradient() <= M_TOL) {
//					if ( encoder[LEFT] > (2*whiteLength+spacing) ) {
//						MotorDir(BREAK, BREAK);
//						MotorSpeed(0,0);
//						return lineCounter;
//					}
				}
				currentState = BCR_DIMMING_STATE;
				break;
		}

		/* enough lines scanned? -> stop */
		if (num>0 && lineCounter >= num) {
			MotorDir(BREAK, BREAK);
			MotorSpeed(0,0);
			break;
		}
	} // end switch

	MotorSpeed(0,0);
	return lineCounter;
}