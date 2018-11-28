#ifndef __CTP_H__
#define __CTP_H__

#include "lwip/sockets.h"
#include "lwip/netdb.h"

/*
�����ʽ:CTP:ID CMD_NUM PARM_STR_LEN PARM_STR

ID, CMD_NUM , PARM_STR_LEN Ϊxxxx��4λ����
ID����Ҫ˫��Э�̺�.
CMD_NUMΪ�����,�������Ҫ��0001��ʼ
PARM_STR_LEN Ϊ PARM_STR ����
����:�����޲�������:"CTP:0003 0188 0000 "//����пո�
     �����Զ����������:"CTP:0003 0189 0011 ABC DEF GHI"//���û�пո�

ÿ�οͻ����������ID�Ž����Ự֮ǰ��Ҫ�ȷ���һ��CM0����Ϊ "CTP:XXXX 0000 0000 ",�����������֪ID��ע����,��ô���λỰ����, �´λỰ����������ͬ.

������ע��ĳ��ID�Ŷ��ᷢ�͸��ͻ���һ������Ϊ "CTP:XXXX 9999 0000 ".

�ͻ����ظ���ĳ��ID�ŷ���CMD0,�������ظ���������"CTP:XXXX 9998 0000 ".

�ͻ��˸�ĳ��ID�ŷ�������,��������û��ע���ID�Ż��߸�ID���Ѿ�����,�������ظ� ��������"CTP:0000 9997 0000 0000 ".

���ͻ����ظ����ӷ�����CTP�˿�, �������������� "CTP:XXXX 9996 0000 " ,���ҹر����һ���ظ�������.

���ͻ���û�и�ָ��������ID����CMD0�ͷ�����������, ��������������һظ��������� "CTP:XXXX 9995 0000 ".

���������ڲ�����, ���������ͻ��˷������� "CTP:XXXX 9994 0000 ".
*/

#define CTP_PREFIX "CTP:"
#define CTP_PREFIX_LEN strlen("CTP:XXXX XXXX XXXX ")

#define CTP_CMD0            0
#define CTP_UNREG_ID        9999
#define CTP_CMD0_REPEAT     9998
#define CTP_NO_ID           9997
#define CTP_CONNECT_REPEAT  9996
#define CTP_NO_CMD0         9995
#define CTP_SRV_FAULT       9994

enum ctp_srv_msg_type
{
    CTP_SRV_RECV_MSG,
    CTP_SRV_CLI_CONNECTED,
    CTP_SRV_CLI_LINK_SUCC,
    CTP_SRV_CLI_DISCONNECT,
    CTP_SRV_HDL_CLOSE,
};

enum ctp_cli_msg_type
{
    CTP_CLI_SEND_TO,
    CTP_CLI_RECV_TO,
    CTP_CLI_CONNECT_TO,
    CTP_CLI_RECV_MSG,
    CTP_CLI_CONNECT_SUCC,
    CTP_CLI_CONNECT_FAIL,
    CTP_CLI_DISCONNECT,
};

char *ctp_get_pram(u32 parm_num, char *parm_list);

int ctp_srv_init(u16_t port);
void ctp_srv_set_thread_prio_stksize(u32 prio, u32 stk_size);
void ctp_srv_uninit(void);
void *ctp_srv_reg(u16_t id, int (*cb_func)(void *hdl, void *cli, enum ctp_srv_msg_type type, u32 cmd_num, char *parm_list, void* priv),  void *priv);
void ctp_srv_unreg(void *handle);
int ctp_srv_send(void *handle, void *cli, u32 cmd_num, char *parm_list);
int ctp_srv_send_all(void *handle,u32 cmd_num, char *parm_list);
u32 ctp_srv_get_cli_cnt(void *handle);
struct sockaddr_in *ctp_srv_get_cli_addr(void *cli);


int ctp_cli_init(void);
void ctp_cli_uninit(void);
void *ctp_cli_reg(u16_t id, struct sockaddr_in *dest_addr, int (*recv_func)(void *hdl, enum ctp_cli_msg_type type, u32 cmd_num, char *parm_list, void* priv), void *priv);
void ctp_cli_unreg(void *handle);
int ctp_cli_send(void *handle, u32 cmd_num, char *parm_list);
void ctp_cli_set_thread_prio_stksize(u32 prio, u32 stk_size);

#endif
