
/*******************************************************************************************
 File name:file_serve.h

 Version: 1.00

 Discription: 文件服务函数头文件


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期二, 06-24-2014  11:03:17上午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __file_serve_h
#define __file_serve_h

#define  FSV_TASK_PRIO           20

#define      FSV_STSTUS_INIT          0x00
#define      FSV_STSTUS_BEGIN_READ    0x01
#define      FSV_STSTUS_READING       0x02
#define      FSV_STSTUS_READ_END      0x03



typedef struct __fsv_q
{
void *fsv ;
u8 *buf ;
u32 addr;
u16 len ;
u16 rlen ;
}fsv_q_t ;

typedef struct __fsv
{
OS_TCB *tcb ;// 文件服务进程
OS_SEM *sem ;// 等待读完的信号量
void *fp ; //文件句柄
u8 *ptr ;  //管理内存开始地址
u32 len ;  //管理内存长度
fsv_q_t *q ; //内存管理数据开始，也是用于发送到文件服务进程的消息
u8 *status ;
u16 count ;  // 记录读数据引用次数
}fsv_t;


u32 fsv_init() ;


/*************************************************
Function:            fsv_open
Description:         打开一个文件服务。
Calls:              //
Called By:          //
Table Accessed:     //
Table Updated:      //
Input:             fp  -- 文件访问句柄
                   len -- 本服务管理的buffe长度，以512 为单位
Output:

Return:            // 文件服务句柄
Others:	           // 其它说明

*************************************************/
fsv_t * fsv_open(void *fp ,u8* ptr,u16 len) ;



u32 fsv_read(fsv_t *fsv ,u32 addr ,u8 *buf ,u16 len ,u8 type) ;
u32 fsv_check_read_over(fsv_t *fsv ,u32 addr ,u8* buf) ;






#endif
