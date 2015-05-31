#ifndef COLLISIONMODULE_H
#define COLLISIONMODULE_H

/** Value containing collision directions,
 * for Modules above this one.
 */
char cm_directionValue = 0;

/**
 * @brief init collision module.
 * 
 * Registers behaviour function.
 */
void cm_init();

#endif