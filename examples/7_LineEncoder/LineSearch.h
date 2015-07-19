#ifndef LINESEARCH_H
#define LINESEARCH_H

void lis_init(void);
void lis_clean(void);
int lis_followLine(void);
int lis_findLine(void);

int lis_tickLineDetector(void);
void lis_initLineDetector(void);

#endif /* LINESEARCH_H */
