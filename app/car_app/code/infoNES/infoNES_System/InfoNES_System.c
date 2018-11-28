#include "InfoNES_System.h"
#include "InfoNES.h"
#include "InfoNES_pAPU.h"
#include "nes_port.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "malloc.h"
#include "lcd.h"
#include "hwi.h"
#include "os_api.h"
#include "msg.h"
#include "debug.h"
#include "clock.h"

//#include "joypad.h"
// Palette data  ��ɫ������
#define  PX_WIDTH         24 / 8
#define  LINE_WIDTH        400 * (24 / 8)
#define  SCREEN_WIDTH     240 * (400 * (24 / 8))
#define  LCD_WIDTH          400
#define  LCD_HEIGHT         240

static int zoom_x_tab[LCD_WIDTH];
static int zoom_y_tab[LCD_HEIGHT];

static int px_width;
static int line_width;
static int screen_width;
static int lcd_width;
static int lcd_height;
extern u8 g_dbuf[];
WORD NesPalette[ 64 ] =
{

	//RGB565
	0x73AE,0x20D1,0x0015,0x4013,0x880E,0xA802,0xA000,0x7840,
	0x4160,0x0220,0x0280,0x01E2,0x19EB,0x0000,0x0000,0x0000,
	0xBDF7,0x039D,0x21DD,0x801E,0xB817,0xE00B,0xD940,0xCA61,
	0x8B80,0x04A0,0x0540,0x0487,0x0411,0x0000,0x0000,0x0000,
	0xF79E,0x3DFF,0x5CBF,0xA45F,0xF3DF,0xFBB6,0xFBAC,0xFCC7,
	0xF5E7,0x8682,0x4EE9,0x5FD3,0x075B,0x0000,0x0000,0x0000,
	0xF79E,0xAF3F,0xC6BF,0xD65F,0xFE3F,0xFE3B,0xFDF6,0xFED5,
	0xFF34,0xE7F4,0xAF97,0xB7F9,0x9FFE,0x0000,0x0000,0x0000

	/*
	0x738e,0x20d1,0x0015,0x4013,0x880e,0xa802,0xa000,0x7840,
	0x4140,0x0200,0x0280,0x01c2,0x19cb,0x0000,0x0000,0x0000,

	0xbdd7,0x039d,0x21dd,0x801e,0xb817,0xe00b,0xd940,0xca41,
	0x8b80,0x0480,0x0540,0x0487,0x0411,0x0000,0x0000,0x0000,

	0xffdf,0x3ddf,0x5c9f,0x445f,0xf3df,0xfb96,0xfb8c,0xfcc7,
	0xf5c7,0x8682,0x4ec9,0x5fd3,0x075b,0x0000,0x0000,0x0000,

	0xffdf,0xaf1f,0xc69f,0xd65f,0xfe1f,0xfe1b,0xfdd6,0xfed5,
	0xff14,0xe7d4,0xaf97,0xb7d9,0x9fde,0x0000,0x0000,0x0000,
	*/
  /*0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
  0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
  0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
  0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
  0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
  0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000,*/
};


/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
/**
 * ����zoom ���ű�
 */
int make_zoom_tab()
{
	int i;
	//zoom_x_tab = (int *)malloc(sizeof(int) * lcd_width);

//	if(NULL == zoom_x_tab)
//	{
//		printf("make zoom_x_tab error\n");
//		return -1;
//	}
	for(i=0; i<lcd_width; i++)
	{
		zoom_x_tab[i] = i*NES_DISP_WIDTH/lcd_width;
	}
	//zoom_y_tab = (int *)malloc(sizeof(int) * lcd_height);
//	if(NULL == zoom_y_tab)
//	{
//		printf("make zoom_y_tab error\n");
//		return -1;
//	}
	for(i=0; i<lcd_height; i++)
	{
		zoom_y_tab[i] = i*NES_DISP_HEIGHT/lcd_height;
	}
	return 1;
}

//���ص�Menu
int InfoNES_Menu()
{
/*
 *  Menu screen
 *
 *  Return values
 *     0 : Normally
 *    -1 : Exit InfoNES
 */
  if ( PAD_PUSH( PAD_System, PAD_SYS_QUIT) )
    return -1;

	// Nothing to do here
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*               InfoNES_ReadRom() : Read ROM image file             */
/*                                                                   */
/*===================================================================*/
//��������Ĺ����ǽ���������һ��NES�ļ����ڴ�
int InfoNES_ReadRom( const char *pszFileName )
{
///*
// *  Read ROM image file
// *
// *  Parameters
// *    const char *pszFileName          (Read)
// *
// *  Return values
// *     0 : Normally
// *    -1 : Error
// */

	ReadFile(&NesHeader, sizeof NesHeader, 1,(const unsigned char *)pszFileName);

	  if ( memcmp( NesHeader.byID, "NES\x1a", 4 ) != 0 )
  {
    /* ������ȷ��NES�ļ� */
    return -1;
  }
 /* ��� SRAM */
  memset( SRAM, 0, SRAM_SIZE );//8KB

	  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if ( NesHeader.byInfo1 & 4 )
  {
		ReadFile(&SRAM[ 0x1000 ], 512, 1, (const unsigned char *)pszFileName);
  }

	ROM=	malloc(0x4000*NesHeader.byRomSize);

	ReadFile(ROM, 0x4000, NesHeader.byRomSize, (const unsigned char *)pszFileName);

	if ( NesHeader.byVRomSize > 0 )
  {

	VROM=	malloc(0x2000*NesHeader.byVRomSize);

	ReadFile(VROM, 0x2000, NesHeader.byVRomSize, (const unsigned char *)pszFileName);
  }
	 /* Successful */
 return 0;
}

/*===================================================================*/
/*                                                                   */
/*           InfoNES_ReleaseRom() : Release a memory for ROM           */
/*                                                                   */
/*===================================================================*/
//��������Ĺ����� �˳���Ϸ�� �ͷ���Ϸռ�õ��ڴ�
void InfoNES_ReleaseRom()
{
/*
 *  Release a memory for ROM
 *
 */

  if ( ROM )
  {
    free( ROM );
    ROM = NULL;
  }

  if ( VROM )
  {
		free( VROM );

    VROM = NULL;
  }

}
int lcd_fb_display_px(WORD color, int x, int y)
{
	u8  *pen8;
    pen8 = (u8 *)(g_dbuf + (y*400 + x)/**3/2*3*/);
    *pen8 = color;
//    pen8 = (u8 *)(g_dbuf + (y*line_width + x)+(y*line_width + x)/4);
//    *pen8 = (color>>8)&0xff;
//    pen8 = (u8 *)(g_dbuf + (y*line_width + x)+(y*line_width + x)/2);
//    *pen8 = color;



	return 0;
}
/*===================================================================*/
/*                                                                   */
/*      InfoNES_LoadFrame() :                                        */
/*           Transfer the contents of work frame on the screen       */
/*                                                                   */
/*===================================================================*/
//��������Ĺ����ǻ���һ֡���浽��ʾ�豸 ������Һ����
void InfoNES_LoadFrame()
{
	int x,y;
	int line_width;
	WORD wColor;
//
    for (y = 0; y < lcd_height; y++ )
    {
        line_width = zoom_y_tab[y] * NES_DISP_WIDTH;
        for (x = 0; x < lcd_width; x++ )
        {
            wColor = WorkFrame[line_width  + zoom_x_tab[x]];
            lcd_fb_display_px(wColor, x, y);
        }
    }

//    for (x = 0; x < NES_DISP_WIDTH; x++)
//    {
//        for ( y = 0; y < NES_DISP_HEIGHT; y++ )
//        {
//            wColor = WorkFrame[y * NES_DISP_WIDTH  + x ];
//            lcd_fb_display_px(wColor, x, y);
//        }
//    }
}


/*===================================================================*/
/*                                                                   */
/*             InfoNES_PadState() : Get a joypad state               */
/*                                                                   */
/*===================================================================*/
//��������Ĺ����ǻ�ȡ��Ϸ�ֱ��������
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
/*
 *  Get a joypad state
 *
 *  Parameters
 *    DWORD *pdwPad1                   (Write)
 *      Joypad 1 State
 *
 *    DWORD *pdwPad2                   (Write)
 *      Joypad 2 State
 *
 *    DWORD *pdwSystem                 (Write)
 *      Input for InfoNES
 *
 */
//�ֱ����ƶ���,��һ��DWORD�������ݱ�ʾ,�ֱ�1���ֱ�2������ͬ
//      case Key_Right:
//    pdwPad1 |= (1<<7);  ����λ��ʾ ��
//    break;
//  case Key_Left:
//    *pdwPad1 |= (1<<6); ����λ��ʾ��
//    break;
//  case Key_Down:
//    *pdwPad1 |= (1<<5);����λ��ʾ��
//    break;
//  case Key_Up:
//    *pdwPad1 |= (1<<4);����λ��ʾ��
//    break;
//  case Key_S:
//    *pdwPad1 |= (1<<3);����λ��ʾ Start
//    break;
//  case Key_A:
//    *pdwPad1 |= (1<<2); A��
//    break;
//  case Key_Z:
//    *pdwPad1 |= (1<<1); Z��
//    break;
//  case Key_X:
//    *pdwPad1 |= (1<<0); X��

	u8 PadVal=0;
	//PadVal = JOYPAD_Read();
//[7]:��
//[6]:��
//[5]:��
//[4]:��
//[3]:Start
//[2]:Select
//[1]:B
//[0]:A
	*pdwPad1=PadVal;

	pdwPad2=0;

}
/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemoryCopy() : memcpy                         */
/*                                                                   */
/*===================================================================*/
//�����װ���ڴ濽������ �����������ģ�����ڲ�����
void *InfoNES_MemoryCopy( void *dest, const void *src, int count )
{
/*
 *  memcpy
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the copied block?s destination
 *
 *    const void *src                  (Read)
 *      Points to the starting address of the block of memory to copy
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to copy
 *
 *  Return values
 *    Pointer of destination
 */

  memcpy( dest, src, count );
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemorySet() : Get a joypad state              */
/*                                                                   */
/*===================================================================*/
//�ڴ���趨���� ��һ���ƶ��ڴ��������ó��β�c�������ֵ�����������ģ�����ڲ�����
void *InfoNES_MemorySet( void *dest, int c, int count )
{
/*
 *  memset
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the block of memory to fill
 *
 *    int c                            (Read)
 *      Specifies the byte value with which to fill the memory block
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to fill
 *
 *  Return values
 *    Pointer of destination
 */


  memset( dest, c, count);
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*                DebugPrint() : Print debug message                 */
/*                                                                   */
/*===================================================================*/
//����������ɲ������
void InfoNES_DebugPrint( char *pszMsg )
{
 // _RPT0( _CRT_WARN, pszMsg );
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundInit() : Sound Emulation Initialize           */
/*                                                                   */
/*===================================================================*/
//��ʼ����Ƶ����Ӳ��,Ҳ������������ִ��Ӳ���ĳ�ʼ��,����������Ǳ�Ҫ��
void InfoNES_SoundInit( void ) {}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundOpen() : Sound Open                           */
/*                                                                   */
/*===================================================================*/
	//����������
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate )
{
//�����Ƿ���,0��ʾ�رվ���,1��ʾ��������
	/* APU Mute ( 0:OFF, 1:ON ) */
	if(1==APU_Mute) APU_Mute=0;

	return 1;
 // return(TRUE);
}


/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundClose() : Sound Close                         */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundClose( void )
{
	//�����Ƿ���,0��ʾ�رվ���,1��ʾ��������
	/* APU Mute ( 0:OFF, 1:ON ) */
	if(0==APU_Mute) APU_Mute=1;
}


/*===================================================================*/
/*                                                                   */
/*            InfoNES_SoundOutput4() : Sound Output 4 Waves          */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5)
{
  BYTE wave[ rec_freq ];
   int i ;
  for (  i = 0; i < rec_freq; i++)
  {

	 wave[i] =  wave1[i] + wave2[i]  + wave4[i] + (wave3[i] >> 2) + wave5[i];
  }

//wave[] �Ǻϳɺ��WAVE��������,����ֱ���͵�DAC������Ϸ����,ע���������44100hz
}



/*===================================================================*/
/*                                                                   */
/*            InfoNES_Wait() : Wait Emulation if required            */
/*                                                                   */
/*===================================================================*/
//������������ͺ�,����ԭ�����������������,�ȴ�ģ������
void InfoNES_Wait()
{

}


/*===================================================================*/
/*                                                                   */
/*            InfoNES_MessageBox() : Print System Message            */
/*                                                                   */
/*===================================================================*/
//��Ϣ��ʾ �������ͺ� ���Գ���Ϊ�˵����Ի�����Ϣ�Ż��õ�
void InfoNES_MessageBox( char *pszMsg ,...)
{
 // MessageBox( hWndMain, pszMsg, APP_NAME, MB_OK | MB_ICONSTOP );
}


extern const unsigned char ddl_nes[];

extern const unsigned char _acContra[];
extern int make_zoom_tab();
extern void disp_buf_init(u8 *buf,u8 num,u16 width,u16 height,u32 color);
void infones_task(void *p_arg)
{
    u32 err;
    u32 ret;
    u32 msg ;
    msg_t *msg_d;

    px_width     = PX_WIDTH;
	line_width   = LINE_WIDTH;
	screen_width = SCREEN_WIDTH;
	lcd_width    = LCD_WIDTH;
	lcd_height   = LCD_HEIGHT;
    disp_buf_init(g_dbuf,3,lcd_width,lcd_height,0x008080);

    make_zoom_tab();
	if( 0 !=InfoNES_Load((const char *)_acContra))
	{
		while(520){}
	}else
	{
      InfoNES_Init();
	}

    while(1)
    {
                    /*-------------------------------------------------------------------*/
        /*  To the menu screen                                               */
        /*-------------------------------------------------------------------*/
        if ( InfoNES_Menu() == -1 )
          break;  // Quit

        /*-------------------------------------------------------------------*/
        /*  Start a NES emulation                                            */
        /*-------------------------------------------------------------------*/
        InfoNES_Cycle();

//    	msg = get_msg();
//        if(msg  > (u32)0x2000000)
//        {
//            msg_d = msg ;
//            msg = msg_d->type ;
//        }
//
//
//        switch(msg)
//        {
//
//            case MSG_MODE_SW:
//                break;
//
//            case MSG_DEVICECHANGE:
//                break;
//
//
//            case MSG_USB_FLAG:
//                break;
//
//            case MSG_MSD_RUN:
//                break;
//            case MSG_ENC_OPEN:
//                break;
//
//            case MSG_ENC_CLOSE:
//                break;
//
//            case MSG_ENC_START:
//                break;
//
//            case MSG_ENC_STOP:
//                break;
//
//            case MSG_KEY_NEXT:
//            	break;
//            case MSG_KEY_PREV:
//            	break;
//            case MSG_KEY_OK:
//            	break;
//        }

    }
    // Completion treatment
    InfoNES_Fin();

}

