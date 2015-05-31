#ifndef LIGHTMODULE_H
#define LIGHTMODULE_H

/**
 * Contains values of light sensors.
 * For modules of upper range.
 */
int lm_values[2];

/**
 * @brief init light module.
 * 
 * Register behaviour function at timer service.
 */
void lm_init();

#endif