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
 * To scan the whole car code, set num to zero.
 * 
 * @param num the amount of lined to scan and drive over.
 * @return the amount of lines driven over.
 */
int bcr_scanLines(unsigned char num);

/**
 * @brief Contrary function to @function bcr_initBarcodeReader().
 * 
 * Erases Services from @File TimerService.h and cleans up stuff.
 */
void bcr_cleanBCR(void);

int bcr_getGradient(void);

#endif /* BARCODEREADER_H */