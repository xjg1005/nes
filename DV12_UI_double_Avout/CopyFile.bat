@echo off

copy .\data\ff.BIN          ..\app\car_app\project\res\ui_sty_pal.sty
copy .\RSC\menu.res           ..\app\car_app\project\res\menu.res
copy .\RSC\res.h              ..\app\car_app\code\disp_ctrl\menu.h
copy .\data\ff.H              ..\app\car_app\code\disp_ctrl\UI.H
 
pause

@echo on

