#ifndef __USB_HOST_API_H__
#define __USB_HOST_API_H__
#include "ioctl.h"
#include "errno-base.h"
#include "device_drive.h"

#ifdef __cplusplus
extern "C" {
#endif

struct usb_hst_arg
{
    u16 isr_priority;
    void *parm;

};

typedef struct usb_wifi_fun{
    s32 (*usb_bulk_only_receive_int)(u8 ep,u8 *pbuf,u32 len);
    s32 (*usb_bulk_only_send)(u8 ep,u8 *pbuf,u32 len);
    s32 (*usb_ctl_msg)(u8 RequestType, u8 Request, u16 Value, u16 Index, u16 Length, u8 *pBuf);
}USB_WIFI_FUN;

#define USB_STOR_MAGIC  'm'// Mass Storage
#define GET_MAX_LUN     _IOR(USB_STOR_MAGIC,5,u32)
#define GET_CUR_LUN     _IOR(USB_STOR_MAGIC,6,u32)
#define SET_CUR_LUN     _IOW(USB_STOR_MAGIC,7,u32)
//#define GET_BLOCK_SIZE  _IOR(USB_STOR_MAGIC,8,u32)
//#define GET_BLOCK_NUM   _IOR(USB_STOR_MAGIC,9,u32)
#define GET_DISK_INFO   _IOR(USB_STOR_MAGIC,10,struct USB_STORAGE_INFO)

const struct DEV_IO * dev_reg_usb0hst(void *parm);
const struct DEV_IO * dev_reg_usb1hst(void *parm);

#ifdef __cplusplus
}
#endif

#endif
