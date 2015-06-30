#ifndef MYUTILS_H
#define MYUTILS_H

#define BIT(X) (1<<X)
#define K(X) (1<<X)

/**
 * @brief Returns stable line data as parameter and offset as retrn value.
 * @param data The array to return the stable line data.
 * @return the offset between left and right line data.
 */
int util_getStableLineData(unsigned int *data);
inline unsigned int util_lineCorrection(unsigned int linedata);

void util_intToBeep(int value);

inline void util_pauseInterrupts(void);
inline void util_recoverInterruptState(void);

inline unsigned char util_getCollisions(void);
inline int util_lineFound(void);

#endif