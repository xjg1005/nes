@echo off

copy .\data\DV12.BIN          ..\app\car_app\project\res\ui_sty.sty
copy .\RSC\menu.res           ..\app\car_app\project\res\menu.res
copy .\RSC\res.h              ..\app\car_app\code\disp_ctrl\menu.h
copy .\data\DV12.H              ..\app\car_app\code\disp_ctrl\UI.H

//pause

@echo on

