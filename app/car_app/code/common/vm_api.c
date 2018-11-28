
#include "vm_api.h"
#include "my_ui.h"

static const vm_api_list_t  vm_list[VM_API_MAX] =
{
    {SYS_SET_INFO, sizeof(SYS_INF)},
    {DISP_SET_INFO, 255},
    {EXP_SET_INFO, 60},
};



/*----------------------------------------------------------------------------*/
/**@brief   VM初始化，只在上电初始化一次
   @param   dev_hdl : FLASH设备句柄
            vm_addr : 需要作为VM区域的FLASH实际物理地址，大小固定为68K BYTE
   @return  非0失败
   @author  jilong
   @note
*/
/*----------------------------------------------------------------------------*/
int vm_api_init(DEV_HANDLE dev_hdl, u32 vm_addr)
{
    int ret;

    ret = vm_init(dev_hdl, vm_addr, 1);
    if(ret != VM_ERR_NONE){
        return -1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief   虚拟存储写函数
   @param   type : 用户定义的VM类型
            vm_data : 需要写入VM的数据
   @return  非VM_ERR_NONE失败
   @author  jilong
   @note    每次写的数据长度都是用户在表vm_list所指定类型所对应的大小
*/
/*----------------------------------------------------------------------------*/
int vm_api_write(u8 type, void * vm_data)
{
    vm_hdl tmp_vm_hdl;
    vm_api_list_t * list;
    int ret;

    if( (type > VM_API_MAX) || (vm_data == NULL) ){
        return -1;
    }
    list = &vm_list;

    tmp_vm_hdl = vm_open(type, list[type].vm_len);
//    if(tmp_vm_hdl == NULL){
//        return -1;
//    }

    ret = vm_write(tmp_vm_hdl, vm_data);
    if(ret != VM_ERR_NONE){
        printf("vm write err %d\n", ret);
        return ret;
    }

    return VM_ERR_NONE;
}

/*----------------------------------------------------------------------------*/
/**@brief   虚拟存储写函数
   @param   type : 用户定义的VM类型
            vm_data : 需要读取VM的数据
   @return  非VM_ERR_NONE失败
   @author  jilong
   @note    每次读的数据长度都是用户在表vm_list所指定类型所对应的大小
*/
/*----------------------------------------------------------------------------*/
int vm_api_read(u8 type, void * vm_data)
{
    vm_hdl tmp_vm_hdl;
    vm_api_list_t * list;
    int ret;

    if( (type > VM_API_MAX) || (vm_data == NULL) ){
        return -1;
    }
    list = &vm_list;

    tmp_vm_hdl = vm_open(type, list[type].vm_len);
//    if(tmp_vm_hdl == NULL){
//        return -1;
//    }

    ret = vm_read(tmp_vm_hdl, vm_data);
    if(ret != VM_ERR_NONE){
        printf("vm read err %d\n", ret);
        return ret;
    }

    return VM_ERR_NONE;
}








