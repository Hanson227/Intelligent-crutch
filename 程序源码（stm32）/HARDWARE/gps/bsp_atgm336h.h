#ifndef __BSP_ATGM336H_H
#define __BSP_ATGM336H_H

extern char gps_data[128];

void errorLog(int num);
void parseGpsBuffer(void);
void pushGPSdata(void);

#endif /*__BSP_ATGM336H_H*/

