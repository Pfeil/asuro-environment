#include "MyUtils.h"
#include "asuro.h"

#define LIGHT_FAULT 85 //oder 85

/**
 * @brief Returns stable line data as parameter and offset as retrn value.
 * @param data The array to return the stable line data.
 * @return The offset between left and right line data.
 */
int util_getStableLineData(unsigned int *data) {
	int i;
	for (i = 0; i<0xFF; i++) {
		LineData(data);
	}
	return util_lineCorrection(data[LEFT]) - data[RIGHT];
}

// for LEFT line data
inline unsigned int util_lineCorrection(unsigned int linedata) {
	float x = linedata / 1024.0;
	return (unsigned int) (linedata + (LIGHT_FAULT * 4 * x * (x-1)));
}

void util_intToBeep(int value) {
	MotorSpeed(0,0);
	Msleep(200);
	int i;
	for(i=0; i<value; i++) {
		MotorSpeed(40,40);
		Msleep(50);
		MotorSpeed(0,0);
		Msleep(35);
	}
	MotorSpeed(0,0);
}

unsigned char tmp_SREG;
inline void util_pauseInterrupts(void) {
	tmp_SREG = SREG;
	cli();
}

inline void util_recoverInterruptState(void) {
	SREG = tmp_SREG;
}

inline unsigned char util_getCollisions(void) {
	unsigned char result = 0;
	result  = PollSwitch() & 0b00111111;
	result &= PollSwitch() & 0b00111111;
	result &= PollSwitch() & 0b00111111;
	return result;
}

inline int util_lineFound(void) {
	return 0; // TODO implement this shit
}

inline void waitOdo(unsigned int ticks, enum utilDir direction) {
	EncoderSet(0,0);
	if (direction == uLEFT || direction == uRIGHT) {
		while (encoder[direction] < ticks) {}
	}
}
