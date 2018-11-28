
#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "ladc.h"
#include "dac.h"



#define AUDIO_LADC_TYPE     1
#define AUDIO_DAC_TYPE      2


#define AUDIO_CMD_DAC_SET_SRATE         0X01
#define AUDIO_CMD_DAC_WRITE             0X02
#define AUDIO_CMD_LADC_START            0X03
#define AUDIO_CMD_LADC_STOP             0X04
//#define AUDIO_CMD_LADC_CLOSE            0X05
#define AUDIO_CMD_LADC_SET_MIC_VOL      0X05
#define AUDIO_CMD_LADC_SET_LINE_IN_VOL  0X06
#define AUDIO_CMD_LADC_SET_RL_VOL          0X07

u32 audio_open(void * parm,u8 type);
u32 audio_ctl(u32 cmd,void * parm);


#endif

