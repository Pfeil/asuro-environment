#include "WallBounce.h"
#include "Asuro.h"
#include "MyUtils.h"
#include "TimerService.h"

#define SPEED 90
#define LSPEED 50

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
	if ( 1/* (coll | K(1)) >= 0*/) { currentstate = STATE_LEFT; }
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

inline void driveBack(int distance) {
	SetMotorPower(0, (int8_t) (-1 * SPEED));
	waitOdo(distance, uRIGHT);
	SetMotorPower(0,0);
}

inline void driveCurve(void) {
	SetMotorPower(LSPEED, SPEED);
}

int wb_bounce(int direction)
{
	// means: if (direction != 0) then direction=1
	if (direction != LEFT) { direction = RIGHT; }

	// drive a little back
	driveBack(40);
	// drive a curve for next collision
	driveCurve();

	// reset foo
	// SetMotorPower(0,0);
	// MotorDir(FWD, FWD);

	return 1;
}

void wb_clean() {

}
