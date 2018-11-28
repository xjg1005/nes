
#include "nes_port.h"
#include <string.h>
#include <stdlib.h>

int ReadFile(void *buf, unsigned int len, unsigned short num, const unsigned char* FileSrc) 
{
	volatile char *p = buf;
	unsigned int i=0;
static	unsigned int LeftRead=0; 
	
	if(FileSrc==0) return -1;/*���û��ָ���κ�ROM��ֱ�ӷ��ش��� �������: -1 */
	
	for(i=0;i<num;i++)/*num��ָ����ȡ�Ĵ���*/
	{/*p�����ݻ�������  */ /*FileSrc��ROM�����׵�ַ *//*LeftRead���Ѿ���ȡ�������� *//*len��ÿ�ζ�ȡ�����ݳ��� */		
		memcpy((char*)p,FileSrc+LeftRead,len);
		LeftRead+=len;/*�Ѿ���ȡ��ƫ�����ۼ���һ��*/
		p+=len;/*����ָ���ַ�ۼ�*/
	}
	
	return 0;
}


