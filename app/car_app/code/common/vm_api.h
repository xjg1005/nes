#ifndef __VM_API_H__
#define __VM_API_H__


#include "config.h"
#include "vm.h"



typedef enum _VM_API_TYPE
{
    SYS_SET_INFO = 0,
    DISP_SET_INFO,
    EXP_SET_INFO,

    VM_API_MAX,
    VM_API_END = 0xff,//总类型数不能超过255

}VM_API_TYPE;






typedef struct _vm_api_list
{
    VM_API_TYPE type;
    u8 vm_len;//某类型的数据长度不能超过255 BYTE
}vm_api_list_t;



int vm_api_init(DEV_HANDLE dev_hdl, u32 vm_addr);
int vm_api_write(u8 type, void * vm_data);
int vm_api_read(u8 type, void * vm_data);


#endif

