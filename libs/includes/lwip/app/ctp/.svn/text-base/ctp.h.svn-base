#ifndef __CTP_H__
#define __CTP_H__

#include "lwip/sockets.h"
#include "lwip/netdb.h"

/*
命令格式:CTP:ID CMD_NUM PARM_STR_LEN PARM_STR

ID, CMD_NUM , PARM_STR_LEN 为xxxx的4位数字
ID号需要双方协商好.
CMD_NUM为命令号,命令号需要从0001开始
PARM_STR_LEN 为 PARM_STR 长度
例如:发送无参数命令:"CTP:0003 0188 0000 "//最后有空格
     发送自定义参数命令:"CTP:0003 0189 0011 ABC DEF GHI"//最后没有空格

每次客户端与服务器ID号建立会话之前都要先发送一条CM0命令为 "CTP:XXXX 0000 0000 ",如果服务器告知ID号注销了,那么本次会话结束, 下次会话过程流程相同.

服务器注销某个ID号都会发送给客户端一条命令为 "CTP:XXXX 9999 0000 ".

客户端重复给某个ID号发送CMD0,服务器回复警告命令"CTP:XXXX 9998 0000 ".

客户端给某个ID号发送命令,若服务器没有注册该ID号或者该ID号已经销毁,服务器回复 警告命令"CTP:0000 9997 0000 0000 ".

若客户端重复连接服务器CTP端口, 服务器发送命令 "CTP:XXXX 9996 0000 " ,并且关闭最后一次重复的连接.

若客户端没有给指定服务器ID发送CMD0就发送其他命令, 服务器丢弃命令并且回复警告命令 "CTP:XXXX 9995 0000 ".

若服务器内部错误, 服务器给客户端发送命令 "CTP:XXXX 9994 0000 ".
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
