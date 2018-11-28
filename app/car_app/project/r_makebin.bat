

dv12-elf-objcopy -O binary   -j .sdram_data main.or32 sdram_data.bin
dv12-elf-objcopy -O binary   -j .video_ram_data main.or32 video_ram_data.bin
dv12-elf-objcopy -O binary   -j .ram0_data main.or32 ram0_data.bin

copy /B sdram_data.bin+video_ram_data.bin+ram0_data.bin sdram.bin
del sdram_data.bin video_ram_data.bin ram0_data.bin

rem dv12-elf-objdump -S main.or32 > main.lst
REM dv12_isd_download.exe   -tosdram  sdram.bin 0x02000000 -go 0x02000000 -dev dv12 -boot 0x01f02000

dv10_app_make.exe -addr 0x02000000 -infile sdram.bin -compression

rem 单独升级代码
REM dv12_isd_download.exe  -f uboot.boot ui.apu sdram.apu -resource res audlogo 32 -tonorflash -dev dv12 -boot 0x01f02000 -div3 -bfumode1 -runaddr 0x2000000

rem 单独升级资源文件
REM dv12_isd_download.exe  -f uboot.boot ui.apu sdram.apu -resource res audlogo 32 -tonorflash -dev dv12 -boot 0x01f02000 -div3 -bfumode2 -runaddr 0x2000000

rem 升级代码和资源文件
dv12_isd_download.exe  -f uboot.boot ui.apu sdram.apu -resource res audlogo 32 -tonorflash -dev dv12 -boot 0x01f02000 -div3 -bfumode3 -runaddr 0x2000000 -erase

rem 单独升级开机画面
rem dv12_isd_download.exe  -f uboot.boot ui.apu sdram.apu -resource res audlogo 32 -tonorflash -dev dv12 -boot 0x01f02000 -div3 -bfumode4 -runaddr 0x2000000

rem  -erase

