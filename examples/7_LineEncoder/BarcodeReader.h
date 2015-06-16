#ifndef BARCODEREADER_H
#define BARCODEREADER_H

/**
 * @brief Initialize the barcode reader.
 */
void bcr_initBarcodeReader(void);

/**
 * @brief Drive over max. num lines and then stop.
 * 
 * If less than num lines are there, it will stop after the last one.
 * To scan the whole bar code, set num to zero.
 * 
 * @param num the amount of lines to scan and drive over.
 * @return the amount of lines driven over.
 */
int bcr_scanLines(unsigned int num);

/**
 * @brief Drive over max. num lines and then stop.
 * 
 * If less than num lines are there, it will stop after the last one.
 * To scan the whole bar code, set num to zero.
 * To allow irregular spaces, use spaces to tell how big the space
 * between two bars shall be at max.
 * 
 * @param num the amount of lines to scan and drive over.
 * @param spacing the amount of additional space between the lines (in odometry fields).
 * @return the amount of lines driven over.
 */
int bcr_scanIrregularLines(unsigned char num, unsigned int spacing);

/**
 * @brief Contrary function to @function bcr_initBarcodeReader().
 * 
 * Erases Services from @File TimerService.h and cleans up stuff.
 */
void bcr_cleanBCR(void);

int bcr_getGradient(void);

#endif /* BARCODEREADER_H */