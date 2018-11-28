#include "typedef.h"
#include "dv12.h"
#include "tft.h"

#if (LCD_TYPE == LCD_RM68116)

void delay_us(u16 cnt)
{
    while(cnt--)
    {
        delay(1);
    }
}

void spi_io_init()
{
	SPI_DCX_OUT();
	SPI_SCL_OUT();
	SPI_CSX_OUT();
	SPI_SDA_OUT();
	SPI_RST_OUT();

	SPI_SDA_H();
	SPI_CSX_H();
	SPI_DCX_L();
	SPI_SCL_L();
}

void spi_write_cmd(u8 cmd)
{
	u8 i;
	SPI_CSX_L();
	SPI_DCX_L();//cmd
	SPI_SDA_OUT();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		if(cmd&BIT(7))
			SPI_SDA_H();
		else
			SPI_SDA_L();
		cmd <<= 1;
		SPI_SCL_H();
		delay_us(100);
	}
	SPI_CSX_H();
	delay_us(100);
}
void spi_write_dat(u8 dat)
{
	u8 i;
	SPI_CSX_L();
	SPI_DCX_H();//dat
	SPI_SDA_OUT();
    delay_us(1);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(1);
		if(dat&BIT(7))
			SPI_SDA_H();
		else
			SPI_SDA_L();
		dat <<= 1;
		SPI_SCL_H();
		delay_us(1);
	}
	SPI_CSX_H();
	delay_us(1);
}

u8 spi_read_dat()
{
	u8 dat = 0;
	u8 i;

	SPI_CSX_L();
	SPI_DCX_H();//dat
	SPI_SDA_IN();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		SPI_SCL_H();
		delay_us(100);

		dat <<= 1;
		if(SPI_SDA_R())
			dat |= BIT(0);

		delay_us(100);
	}
	SPI_CSX_H();
	delay_us(100);

	return dat;
}

u32 spi_read_32(u8 cmd)
{
	u32 dat = 0;
	u8 i;

	SPI_CSX_L();
	SPI_DCX_L();//cmd
	SPI_SDA_OUT();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		if(cmd&BIT(7))
			SPI_SDA_H();
		else
			SPI_SDA_L();
		cmd <<= 1;
		SPI_SCL_H();
		delay_us(100);
	}

	//SPI_CSX_L();
	SPI_DCX_H();//dat
	SPI_SDA_IN();
	delay_us(100);

	SPI_SCL_L();
	SPI_SCL_H();

	for(i=0;i<32;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		SPI_SCL_H();
		delay_us(100);

		dat <<= 1;
		if(SPI_SDA_R())
			dat |= BIT(0);

		delay_us(100);
	}
	SPI_CSX_H();
	delay_us(100);

	return dat;
}

u32 spi_read_24(u8 cmd)
{
	u32 dat = 0;
	u8 i;

	SPI_CSX_L();
	SPI_DCX_L();//cmd
	SPI_SDA_OUT();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		if(cmd&BIT(7))
			SPI_SDA_H();
		else
			SPI_SDA_L();
		cmd <<= 1;
		SPI_SCL_H();
		delay_us(100);
	}

	//SPI_CSX_L();
	SPI_DCX_H();//dat
	SPI_SDA_IN();
	delay_us(100);

	SPI_SCL_L();
	SPI_SCL_H();

	for(i=0;i<24;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		SPI_SCL_H();
		delay_us(100);

		dat <<= 1;
		if(SPI_SDA_R())
			dat |= BIT(0);

		delay_us(100);
	}
	SPI_CSX_H();
	delay_us(100);

	return dat;
}

u32 spi_read_8(u8 cmd)
{
	u32 dat = 0;
	u8 i;

	SPI_CSX_L();
	SPI_DCX_L();//cmd
	SPI_SDA_OUT();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		if(cmd&BIT(7))
			SPI_SDA_H();
		else
			SPI_SDA_L();
		cmd <<= 1;
		SPI_SCL_H();
		delay_us(100);
	}

	//SPI_CSX_L();
	SPI_DCX_H();//dat
	SPI_SDA_IN();
	delay_us(100);

	for(i=0;i<8;i++)
	{
		SPI_SCL_L();
		delay_us(100);
		SPI_SCL_H();
		delay_us(100);

		dat <<= 1;
		if(SPI_SDA_R())
			dat |= BIT(0);

		delay_us(100);
	}
	SPI_CSX_H();
	delay_us(100);

	return dat;
}

void rm68116_init()
{
	u8 para[10];
	u16 i,j;
	u8 *pbuf;

    spi_io_init();
    SPI_RST_H();
    delay_2ms(5);//10ms
    SPI_RST_L();//RESET
    delay_2ms(5);//10ms
    SPI_RST_H();

    printf("\ninit...");
	spi_write_cmd(0x01);//soft reset
	delay_2ms(60);//120msec

	spi_write_cmd(0x26);//Gamma Set
	spi_write_dat(0x08);//select Gamma Curve

    printf("\nid = %06x",spi_read_24(0x04));//5Ch,89h,F0h
    printf("\nRDDST = %08x",spi_read_32(0x09));
    printf("\nRDDPM = %02x",spi_read_8(0x0A));
    printf("\nRDDMADCTL = %02x",spi_read_8(0x0B));
    printf("\nRDDCOLMOD = %02x",spi_read_8(0x0C));
    printf("\nRDDIM = %02x",spi_read_8(0x0D));
    printf("\nRDDSM = %02x",spi_read_8(0x0E));

    printf("\nid1 = %02x",spi_read_8(0xDA));
    printf("\nid2 = %02x",spi_read_8(0xDB));
    printf("\nid3 = %02x",spi_read_8(0xDC));

	//spi_write_cmd(0x10);//Sleep In
	spi_write_cmd(0x11);//Sleep Out

//	spi_write_cmd(0x30);//局部显示
//	spi_write_dat(0x00);
//	spi_write_dat(0x00);
//	spi_write_dat((160-1)>>8);
//	spi_wri#if (LCD_TYPE == LCD_ILI8960)te_dat(160-1);
//	spi_write_cmd(0x12);//Partial Display

	spi_write_cmd(0x13);//Normal Display

	spi_write_cmd(0x20);//Display inversion off(normal)

	spi_write_cmd(0x26);//Gamma Set
	spi_write_dat(0x02);//select Gamma Curve

	spi_write_cmd(0x36);//Memory Data Access Control
	spi_write_dat(0xc0);//扫描方向，顺序

	spi_write_cmd(0x3A);//Interface Pixel Format
	spi_write_dat(0x05);//06:18bits/pixel 05:16-bits/pixel

    ///////////////////////////////////////////////////////////
	spi_write_cmd(0xC7);
	spi_write_dat(0x1C);   		//VCOM offset 微调Fincker//0x19

	spi_write_cmd(0xC0);		// Power Control 1
	spi_write_dat(0xD3);	   	// GVDD Setting//0xF1//调节亮度，值越大越亮，颜色就越淡，
	spi_write_dat(0x13);	   	// GVCL Setting//0x11//以上面同步

 	spi_write_cmd(0xC2);
	spi_write_dat(0x02);
	spi_write_dat(0x84);

	spi_write_cmd(0xF4);
	spi_write_dat(0x00);
	spi_write_dat(0x0C);

	spi_write_cmd(0xFD);
	spi_write_dat(0x10);
	spi_write_dat(0xDF);
	spi_write_dat(0x60);
	spi_write_dat(0xD0);

	spi_write_cmd(0xFE);
	spi_write_dat(0x09);
	spi_write_dat(0xB0);
	spi_write_dat(0x10);
	spi_write_dat(0x48);

    spi_write_cmd(0xF8);
	spi_write_dat(0x01);
	//************* Start Gamma Setting **********//
	spi_write_cmd(0xE0);		// Gamma Command
	spi_write_dat(0x00);
	spi_write_dat(0x0C);
	spi_write_dat(0x18);
	spi_write_dat(0x23);
	spi_write_dat(0x20);
	spi_write_dat(0x01);
	spi_write_dat(0x01);
	spi_write_dat(0x0F);
	spi_write_dat(0x08);
	spi_write_dat(0x03);
	spi_write_dat(0x07);
	spi_write_dat(0x07);
	spi_write_dat(0x06);
	spi_write_dat(0x0B);
	spi_write_dat(0x04);
	spi_write_dat(0x08);
	spi_write_cmd(0xE1);		// Gamma Command
	spi_write_dat(0x00);
	spi_write_dat(0x0C);
	spi_write_dat(0x18);
	spi_write_dat(0x23);
	spi_write_dat(0x20);
	spi_write_dat(0x01);
	spi_write_dat(0x01);
	spi_write_dat(0x0F);
	spi_write_dat(0x08);
	spi_write_dat(0x03);
	spi_write_dat(0x07);
	spi_write_dat(0x07);
	spi_write_dat(0x06);
	spi_write_dat(0x0B);
	spi_write_dat(0x04);
	spi_write_dat(0x08);

    spi_write_cmd(0xF8);
	spi_write_dat(0x00);
    ///////////////////////////////////////////////////////////
	spi_write_cmd(0x29);//Display On

	spi_write_cmd(0x2A);//Column Address Set
	spi_write_dat(0>>8);
	spi_write_dat(0);
	spi_write_dat((128-1)>>8);
	spi_write_dat(128-1);

	spi_write_cmd(0x2b);//Row Address Set
	spi_write_dat(0>>8);
	spi_write_dat(0);
	spi_write_dat((160-1)>>8);
	spi_write_dat(160-1);

	spi_write_cmd(0x2c);//Memory Write

	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0xF8);
            spi_write_dat(0x00);
        }
	}
	delay_2ms(500);
	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0x07);
            spi_write_dat(0xE0);
        }
	}
	delay_2ms(500);
	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0x00);
            spi_write_dat(0x3F);
        }
	}

//    pbuf = malloc(128*160*2);
//    memcpy(pbuf,img128x160,128*160*2);
//	flush_dcache(pbuf,128*160);
	pbuf = 0x2800000;
	delay_2ms(500);

//	while(1)
//	{
//        spi_write_dma_dat(pbuf,128*160*2);
//	}

	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(*pbuf++);
            spi_write_dat(*pbuf++);
            spi_write_dat(*pbuf++);
        }
	}
}

/*
void rm68116_init()
{
	u8 para[10];
	u16 i,j;

    spi_io_init();
    SPI_RST_H();
    delay_2ms(5);//10ms
    SPI_RST_L();//RESET
    delay_2ms(5);//10ms
    SPI_RST_H();

	spi_write_cmd(0x01);//soft reset
	delay_2ms(60);//120msec

	spi_write_cmd(0x26);//Gamma Set
	spi_write_dat(0x08);//select Gamma Curve

    printf("\nid = %06x",spi_read_24(0x04));
    printf("\nRDDST = %08x",spi_read_32(0x09));
    printf("\nRDDPM = %02x",spi_read_8(0x0A));
    printf("\nRDDMADCTL = %02x",spi_read_8(0x0B));
    printf("\nRDDCOLMOD = %02x",spi_read_8(0x0C));
    printf("\nRDDIM = %02x",spi_read_8(0x0D));
    printf("\nRDDSM = %02x",spi_read_8(0x0E));

    printf("\nid1 = %02x",spi_read_8(0xDA));
    printf("\nid2 = %02x",spi_read_8(0xDB));
    printf("\nid3 = %02x",spi_read_8(0xDC));

//	spi_write_cmd(0x04);
//	spi_read_dat();
//	para[0] = spi_read_dat();//5Ch
//	para[1] = spi_read_dat();//89h
//	para[2] = spi_read_dat();//F0h
//	printf("\nread id:");
//	printf_buf(para,3);

//	spi_write_cmd(0x09);//display status
//	para[0] = spi_read_dat();
//	para[1] = spi_read_dat();
//	para[2] = spi_read_dat();
//	para[3] = spi_read_dat();
//	para[4] = spi_read_dat();
//	printf("\nRead Display Power Mode:");
//	printf_buf(para,5);

//	spi_write_cmd(0x0a);//Display Power Mode
//	spi_read_dat();
//	para[0] = spi_read_dat();
//	printf("\nRead Display Power Mode:");
//	printf_buf(para,1);

//	spi_write_cmd(0x0b);//Display MADCTL
//	spi_read_dat();
//	para[0] = spi_read_dat();
//	printf("\nRead Display MADCTL:");
//	printf_buf(para,1);
//
//	spi_write_cmd(0x0c);//Display Pixel Format
//	spi_read_dat();
//	para[0] = spi_read_dat();
//	printf("\nRead Display Pixel Format:");
//	printf_buf(para,1);
//
//	spi_write_cmd(0x0d);//Display Image Mode
//	spi_read_dat();
//	para[0] = spi_read_dat();
//	printf("\nRead Display Image Mode:");
//	printf_buf(para,1);
//
//	spi_write_cmd(0x0e);//Display Signal Mode
//	spi_read_dat();
//	para[0] = spi_read_dat();
//	printf("\nRead Display Signal Mode:");
//	printf_buf(para,1);

	//spi_write_cmd(0x10);//Sleep In
	spi_write_cmd(0x11);//Sleep Out

//	spi_write_cmd(0x30);//局部显示
//	spi_write_dat(0x00);
//	spi_write_dat(0x00);
//	spi_write_dat((160-1)>>8);
//	spi_wri#if (LCD_TYPE == LCD_ILI8960)te_dat(160-1);
//	spi_write_cmd(0x12);//Partial Display

	spi_write_cmd(0x13);//Normal Display

	spi_write_cmd(0x20);//Display inversion off(normal)

	spi_write_cmd(0x26);//Gamma Set
	spi_write_dat(0x02);//select Gamma Curve

	spi_write_cmd(0x36);//Memory Data Access Control
	spi_write_dat(0xc0);//扫描方向，顺序

	spi_write_cmd(0x3A);//Interface Pixel Format
	spi_write_dat(0x06);//18bits/pixel

	spi_write_cmd(0x29);//Display On

	spi_write_cmd(0x2A);//Column Address Set
	spi_write_dat(0>>8);
	spi_write_dat(0);
	spi_write_dat((128-1)>>8);
	spi_write_dat(128-1);

	spi_write_cmd(0x2b);//Row Address Set
	spi_write_dat(0>>8);
	spi_write_dat(0);
	spi_write_dat((160-1)>>8);
	spi_write_dat(160-1);

	spi_write_cmd(0x2c);//Memory Write

	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0xFF);
            spi_write_dat(0x00);
            spi_write_dat(0x00);
        }
	}
	delay_2ms(500);
	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0x00);
            spi_write_dat(0xFF);
            spi_write_dat(0x00);
        }
	}
	delay_2ms(500);
	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(0x00);
            spi_write_dat(0x00);
            spi_write_dat(0xFF);
        }
	}
	u8 *pbuf = 0x2800000;
	delay_2ms(500);
	for(i=0;i<128;i++)
	{
	    for(j=0;j<160;j++)
        {
            spi_write_dat(*pbuf++);
            spi_write_dat(*pbuf++);
            spi_write_dat(*pbuf++);
        }
	}
}
*/
#endif
