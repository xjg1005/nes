
#include "nes_port.h"
#include <string.h>
#include <stdlib.h>

int ReadFile(void *buf, unsigned int len, unsigned short num, const unsigned char* FileSrc) 
{
	volatile char *p = buf;
	unsigned int i=0;
static	unsigned int LeftRead=0; 
	
	if(FileSrc==0) return -1;/*如果没有指向任何ROM则直接返回错误 错误代码: -1 */
	
	for(i=0;i<num;i++)/*num是指定读取的次数*/
	{/*p是数据缓冲数组  */ /*FileSrc是ROM数据首地址 *//*LeftRead是已经读取的数据量 *//*len是每次读取的数据长度 */		
		memcpy((char*)p,FileSrc+LeftRead,len);
		LeftRead+=len;/*已经读取的偏移量累加在一起*/
		p+=len;/*数据指针地址累加*/
	}
	
	return 0;
}


