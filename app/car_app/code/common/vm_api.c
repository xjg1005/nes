
#include "vm_api.h"
#include "my_ui.h"

static const vm_api_list_t  vm_list[VM_API_MAX] =
{
    {SYS_SET_INFO, sizeof(SYS_INF)},
    {DISP_SET_INFO, 255},
    {EXP_SET_INFO, 60},
};



/*----------------------------------------------------------------------------*/
/**@brief   VM��ʼ����ֻ���ϵ��ʼ��һ��
   @param   dev_hdl : FLASH�豸���
            vm_addr : ��Ҫ��ΪVM�����FLASHʵ�������ַ����С�̶�Ϊ68K BYTE
   @return  ��0ʧ��
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
/**@brief   ����洢д����
   @param   type : �û������VM����
            vm_data : ��Ҫд��VM������
   @return  ��VM_ERR_NONEʧ��
   @author  jilong
   @note    ÿ��д�����ݳ��ȶ����û��ڱ�vm_list��ָ����������Ӧ�Ĵ�С
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
/**@brief   ����洢д����
   @param   type : �û������VM����
            vm_data : ��Ҫ��ȡVM������
   @return  ��VM_ERR_NONEʧ��
   @author  jilong
   @note    ÿ�ζ������ݳ��ȶ����û��ڱ�vm_list��ָ����������Ӧ�Ĵ�С
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








