#ifndef __USB_MODE_H__
#define __USB_MODE_H__



#include "config.h"



//#define USB_CHARGE_INIT()       0//PORTR_DIR |= BIT(1);PORTR_PU &= ~BIT(1);PORTR_PD &= ~BIT(1)
//#define USB_IS_INSERT()         0//(PORTR_IN & BIT(1))


typedef struct  __USB_CTRL
{
    u32 enter_usb;
    u32 leave_usb;
    u32 usb_class;
    u32 usb_vid_status;
    u32 usb_sddev_flag;


}USB_CTRL_T ;


enum{
    APP_CLASS_NONE,
    APP_MASSSTORAGE_CLASS,
    APP_VIDEO_CLASS,
    APP_HID_CLASS,
};


extern USB_CTRL_T usb_ctrl;
extern u8 g_usb_startup ;

void usb_app_task(void *p_arg);

void app_usb_reg();

u32 usb_slave_msg_ctrl(void *parm);




#endif




