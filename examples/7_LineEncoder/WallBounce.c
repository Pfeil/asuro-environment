#include "WallBounce.h"
#include "Asuro.h"
#include "MyUtils.h"
#include "TimerService.h"

#define SPEED 80

volatile unsigned char coll;

enum state {
	// below here are "other" states
	STATE_NO_WALL,
	STATE_WALL_LOST,
	// below here are states where "action" is needed
	STATE_LINE,
	// below here are "collision" states
	STATE_FRONT,
	STATE_LEFT,
	STATE_RIGHT
} currentstate;

/**
 * @brief Interrupt function!
 */
void fetchData(void) {
	coll = util_getCollisions();
	// TODO implement: if (coll==foo) { state = foostate; }
	if ( (coll | K(1)) >= 0) { currentstate = STATE_LEFT; }
	if (util_lineFound() == 1) {
		MotorSpeed(0,0);
		currentstate = STATE_LINE;
	}
}

void wb_init(void) {
	EncoderInit();
	currentstate = STATE_NO_WALL;
	
	ts_init();
	ts_addFunction(&fetchData, 5);
}

int wb_bounce(int direction)
{
	// means: if (direction != 0) then direction=1
	if (direction != LEFT) { direction == RIGHT; }
	
	switch (currentstate)
	{
		case STATE_NO_WALL:
			MotorSpeed(SPEED, SPEED);
			break;
		case STATE_WALL_LOST:
			MotorSpeed(0,0);
			break;
		case STATE_FRONT:
			MotorSpeed(0,0);
			break;
		case STATE_LEFT:
			// hold on
			MotorSpeed(0,0);
			// drive a little back
			MotorDir(BREAK, RWD);
			MotorSpeed(0, SPEED);
			Msleep(100);
			MotorSpeed(0,0);
			// drive a curve for next collision
			MotorDir(FWD, FWD);
			MotorSpeed(SPEED, 2*SPEED);
			break;
		case STATE_RIGHT:
			MotorSpeed(0,0);
			break;
		case STATE_LINE:
			MotorSpeed(0,0);
			return 0;
			break;
	}
	return 1;
}

void wb_clean() {
	
}