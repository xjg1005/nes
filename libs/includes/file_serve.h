
/*******************************************************************************************
 File name:file_serve.h

 Version: 1.00

 Discription: �ļ�������ͷ�ļ�


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:���ڶ�, 06-24-2014  11:03:17����



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
OS_TCB *tcb ;// �ļ��������
OS_SEM *sem ;// �ȴ�������ź���
void *fp ; //�ļ����
u8 *ptr ;  //�����ڴ濪ʼ��ַ
u32 len ;  //�����ڴ泤��
fsv_q_t *q ; //�ڴ�������ݿ�ʼ��Ҳ�����ڷ��͵��ļ�������̵���Ϣ
u8 *status ;
u16 count ;  // ��¼���������ô���
}fsv_t;


u32 fsv_init() ;


/*************************************************
Function:            fsv_open
Description:         ��һ���ļ�����
Calls:              //
Called By:          //
Table Accessed:     //
Table Updated:      //
Input:             fp  -- �ļ����ʾ��
                   len -- ����������buffe���ȣ���512 Ϊ��λ
Output:

Return:            // �ļ�������
Others:	           // ����˵��

*************************************************/
fsv_t * fsv_open(void *fp ,u8* ptr,u16 len) ;



u32 fsv_read(fsv_t *fsv ,u32 addr ,u8 *buf ,u16 len ,u8 type) ;
u32 fsv_check_read_over(fsv_t *fsv ,u32 addr ,u8* buf) ;






#endif
