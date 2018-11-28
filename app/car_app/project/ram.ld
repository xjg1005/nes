

MEMORY
{
	sdram(rwx)  : ORIGIN = 0x2000000,           LENGTH = 8M
	video_ram(rwx)  : ORIGIN = 0x1fa0000,       LENGTH = 96K
	ram0(rwx)   : ORIGIN = 0x1f00000,           LENGTH = 64K+16K-0x80
}

SECTIONS
{
/********************************************/

    . =ORIGIN(sdram);
    .sdram_data  0x02000000 :
    {
        _text_rodata_begin = .;
        *startup.o(.text)
        *(.text)
        *(.rodata*)
        _text_rodata_end = .;

    _imb_work_begin = .;
    *(.imb_isr_work)
    _imb_work_end = .;


     *(.data)

     *(.eh_frame)
     *(.comm)

    } >sdram


    .sdram_bss ALIGN(32) :
    {
        *(.mem_malloc)
        *(.mem_heap)

        *(.bss)
        *(COMMON)
        . = (((. + 3) / 4) *4 );
        _STK_BEGIN_ADDR = .;
        *(.stack)
        _STK_END_ADDR = .;

    } > sdram

/********************************************/


    . =ORIGIN(video_ram);
    .video_ram_data ALIGN(4):
    {
    } > video_ram
    .video_ram_bss ALIGN(4):
    {
    } > video_ram

/********************************************/



    . =ORIGIN(ram0);
    .ram0_data ALIGN(4):
    {
        *(.flushinv_icache)
    } > ram0
    .ram0_bss ALIGN(4):
    {
        *(.usb0_ep0_fifo)
        *(.usb0_ep1_fifo)
        *(.usb0_ep2_fifo)
        *(.usb0_ep3_fifo)
        *(.usb0_ep4_fifo)
        *(.sram)

    } > ram0

/********************************************/

_sdram_bss_vma = ADDR(.sdram_bss );
_sdram_bss_size   = SIZEOF(.sdram_bss);

_video_ram_data_vma  = ADDR(.video_ram_data);
_video_ram_data_lma = ADDR(.sdram_data) +SIZEOF(.sdram_data);
_video_ram_data_size =  SIZEOF(.video_ram_data);

_video_ram_bss_vma = ADDR(.video_ram_bss );
_video_ram_bss_size   = SIZEOF(.video_ram_bss);

_ram0_bss_vma = ADDR(.ram0_bss );
_ram0_bss_size  =  SIZEOF(.ram0_bss);

_ram0_data_vma  = ADDR(.ram0_data);
_ram0_data_lma = ADDR(.sdram_data) +SIZEOF(.sdram_data)+SIZEOF(.video_ram_data);
_ram0_data_size =  SIZEOF(.ram0_data);

}
