#ifndef _MSG_H_
#define _MSG_H_

#include "config.h"

#define put_event(event) //api_msg.event_buf |= BIT(event)
#define clr_event(event) //api_msg.event_buf &= ~BIT(event)


typedef struct __msg
{
u16 type ;
void* ptr ;

}msg_t ;


#define MSG_SYS      0x20
//#define MSG_HAF_SEC  MSG_SYS+1
#define MSG_NO (u32)-1


#define    EVENT_UDISK_IN       0
#define    EVENT_UDISK_OUT		1
#define    EVENT_PC_IN			2
#define    EVENT_PC_OUT			3
#define    EVENT_SDMMCA_IN		4
#define    EVENT_SDMMCA_OUT		5
#define    EVENT_SDMMCB_IN		6
#define    EVENT_SDMMCB_OUT		7
#define    EVENT_AUX_IN			8
#define    EVENT_AUX_OUT		9
#define    EVENT_ALM_ON			10
#define    EVENT_PC_SPK_MUTE	11
#define    EVENT_PC_SPK_UNMUTE	12
#define    EVENT_PC_MIC_MUTE	13
#define    EVENT_PC_MIC_UNMUTE	14
#define    EVENT_PC_SPK_VOL     15
#define    EVENT_PC_MIC_VOL     16
#define    EVENT_SCAN_ALL       17
#define    EVENT_100MS          18
#define    EVENT_200MS          19
#define    EVENT_HALF_SEC       20

#define REGISTER_MSG_BEGIN(type) 	const struct play_msg_handler type##_msg_handl_list[]={
#define REGISTER_MSG_END()		{MSG_NONE,  MSG_NONE, NULL}};
#define MSG_TYPE(type) 				type##_msg_handl_list

#define MSG_BEGIN(name)		MSG_BEGIN_##name
#define MSG_END(name)		MSG_END_##name

#define MSG_INDEX(name, msg)			(msg-MSG_BEGIN(name)-1)
#define MSG_TO_PARAM(name, msg)			name##_table[MSG_INDEX(name, msg)]

#define PARAM_TABLE_BEGIN(name)			const u8 name##_table[]={
#define PARAM_TABLE_END()				};
enum sys_msg{
	MSG_BEGIN(decoder_ctrl),
	MSG_STOP,
	MSG_PLAY,
	MSG_PAUSE,
	MSG_END(decoder_ctrl),
	MSG_BEGIN(fselect),
	MSG_FIRST_FILE,
	MSG_NEXT_FILE,
	MSG_PREV_FILE,
	MSG_END(fselect),
    MSG_DECODE_PLAY,
    MSG_DECODE_RUN,
    MSG_DECODE_STOP,
    MSG_DECODE_END ,
    MSG_DECODE_ERR ,
    MSG_DECODE_FF ,
    MSG_DECODE_FB ,
    MSG_DECODE_PAUSE,
    MSG_DECODE_CONTINUE,
    MSG_SET_PLAY_FILE ,
    MSG_FILL_DATA_BEGIN,
    MSG_FILL_DATA_STOP ,

    MSG_PKG_RUN ,
    MSG_PKG_START,
    MSG_PKG_STOP,

    MSG_HAF_SEC,
    MSG_INIT_FILE,
    MSG_ENC1_OPEN ,
    MSG_ENC1_CLOSE,
    MSG_ENC1_START,
    MSG_ENC1_STOP,
    MSG_ENC_OPEN ,
    MSG_ENC_CLOSE ,
    MSG_ENC_START,
    MSG_ENC_STOP,
    MSG_ENC_SAVE_FILE,
    MSG_DSC_START,
    MSG_DSC_STOP,
    MSG_VES_OPEN,
    MSG_VES_CLOSE,
    MSG_AVC_PLAYING,


    MSG_TACK_PHOTO_INIT,
    MSG_TACK_PHOTO_BEGIN,

	MSG_IMC_ENC_START,
	MSG_IMC_ENC_STOP,
    MSG_IMC_ENC_DSC_SW,

    MSG_DEC_OPEN,
    MSG_DEC_CLOSE,
    MSG_DEC_PIC,
    MSG_DECODE_PIC,
    MSG_DECODE_AVI,

    MSG_MODE_SW,

	MSG_KEY_PREV,
	MSG_KEY_NEXT,
	MSG_KEY_MENU,
	MSG_KEY_MODE,
	MSG_KEY_OK,
	MSG_KEY_PARK,
	MSG_KEY_LOCK,

	MSG_SW_SIZE,
	MSG_SW_ENC_OSD,
	MSG_DEC_DEL,
	MSG_SET_FILE_SOS,
	MSG_ONE_SECOND,
	MSG_SW_FPS,

	MSG_MSD_RUN,
    MSG_MASSSTORAGE_CLASS,
    MSG_VIDEO_CLASS,
	MSG_AUDIO_CLASS,
	MSG_HID_CLASS,
    MSG_USB_FLAG,
    MSG_USB_MENU,
    MSG_USB_MODE,
    MSG_MENU_EXIT,
    MSG_DEC_INFORMATION,

    MSG_AV_IN_ONLINE,
    MSG_AV_IN_OFFLINE,
    MSG_KEY_START,
    MSG_KEY_CLOSE,
    MSG_PARKING_SWITCH,
    MSG_CLR_MSG,
    MSG_DEBUG_LCD,
    MSG_DEC_DEV_CHANGE,
	MSG_LDETE_WARNING,
	MSG_RESTORE_ISP,
	MSG_CHECK_GSENSOR,

    MSG_LOCK_FILE,
    MSG_UNLOCK_FILE,
    MSG_LOCK_ALLFILE,
    MSG_UNLOCK_ALLFILE,
    MSG_KEY_OK_3S,

    MSG_KEY_NEXT_LONG,
    MSG_KEY_PREV_LONG,



	//不能改动下面的消息顺序
	MSG_SCR_DISP_CHANGE,  //AVOUT JOHNSON 20160530 ADD

	MSG_RW_FILE_ERR = 0xfc,
	MSG_DEVICECHANGE = 0xfe,
	MSG_NONE=0xff,
};


#define NO_MSG  MSG_NONE



#endif
