/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           ZLG7289.h
** Last modified Date:  2008/01/03
** Last Version:        V1.00
** Description:         数码管显示与键盘管理芯片ZLG7289 针对MicroMouse615的驱动程序头文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          廖茂刚
** Created date: 
** Version: 
** Descriptions: 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/


#ifndef _ZLG7289_H_
#define _ZLG7289_H_


/*********************************************************************************************************
  包含头文件
*********************************************************************************************************/
#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"
#include "gpio.h"
#include "Type.h"


/*********************************************************************************************************
  PA端口定义
*********************************************************************************************************/
#define ZLG7289_CS       GPIO_PIN_2
#define ZLG7289_CLK      GPIO_PIN_3
#define ZLG7289_DIO      GPIO_PIN_4
#define ZLG7289_KEY      GPIO_PIN_5


/*********************************************************************************************************
  可以供外部使用的函数申明
*********************************************************************************************************/
extern void zlg7289Cmd(int8  cCmd);                                     /*  执行ZLG7289 纯指令          */
extern void zlg7289CmdDat(uint8  cCmd, int8  cDat);                     /*  执行ZLG7289 带数据指令      */
extern void zlg7289Download(uint8 ucMod, int8 cX, int8 cDp, int8 cDat); /*  下载数据                    */
extern int8 zlg7289Key(void);                                           /*  读按键值                    */
extern void zlg7289Init(void);                                          /*  ZLG7289 初始化              */


/*********************************************************************************************************
  ZLG7289用户指令集
*********************************************************************************************************/
#define zlg7289Reset()      zlg7289Cmd(0xA4)                            /*  复位(清除)指令              */
#define zlg7289Test()       zlg7289Cmd(0xBF)                            /*  测试指令                    */
#define zlg7289SHL()        zlg7289Cmd(0xA0)                            /*  左移指令                    */
#define zlg7289SHR()        zlg7289Cmd(0xA1)                            /*  右移指令                    */
#define zlg7289ROL()        zlg7289Cmd(0xA2)                            /*  循环左移指令                */
#define zlg7289ROR()        zlg7289Cmd(0xA3)                            /*  循环右移指令                */

#define zlg7289Flash(cX)    zlg7289CmdDat(0x88, (cX))                   /*  闪烁控制                    */
                                                                        /*  cX的8个位对应数码管的8个位  */
                                                                        /*  0:闪烁  1:不闪烁            */

#define zlg7289Hide(cX)     zlg7289CmdDat(0x98, (cX))                   /*  消隐控制                    */
                                                                        /*  cX的8个位对应数码管的8个位  */
                                                                        /*  0:消隐  1:显示              */

#define zlg7289SegOn(cSeg)  zlg7289CmdDat(0xE0, (cSeg))                 /*  段点亮指令                  */
                                                                        /*  8个数码管被看成64个独立LED  */
                                                                        /*  cSeg: 0 ～ 63               */

#define zlg7289SegOff(cSeg) zlg7289CmdDat(0xC0, (cSeg))                 /*  段关闭指令                  */
                                                                        /*  8个数码管被看成64个独立LED  */
                                                                        /*  cSeg: 0 ～ 63               */

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
