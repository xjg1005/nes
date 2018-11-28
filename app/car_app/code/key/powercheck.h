#ifndef _POWER_CHECK_
#define _POWER_CHECK_


//#include "adc_scan.h"
#include "typedef.h"


#define USE_POWER_CHECK   1

#define PWR_SCAN_TIMES 3//20
#define NO_PWR    0xff
#define ENABLE_SAMPLE_VAL 1

u8 power_scan();
void pwr_check_init();
u8 get_power_level(void);
#endif
