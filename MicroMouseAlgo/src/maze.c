/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**mouseGoahead()没有必要跟新坐标   全力奔跑即可

**--------------File Info---------------------------------------------------------------------------------
** File Name:           maze.c
** Last modified Date:  2007/09/24
** Last Version:        V1.0
** Description:         根据底层程序取得的迷宫信息，经过该智能算法控制电脑鼠的下一状态，并送往底层驱动程
**                      序执行。
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Liao Maogang
** Created date:        2007/09/08
** Version:             V1.0
** Descriptions: 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/


/*********************************************************************************************************
  包含头文件
*********************************************************************************************************/
#include "Maze.h"


/*********************************************************************************************************
  全局变量定义
*********************************************************************************************************/
static uint8    GucXStart                           = 0;                /*  起点横坐标                  */
static uint8    GucYStart                           = 0;                /*  起点纵坐标                  */

static uint8    GucXGoal0                           = XDST0;            /*  终点X坐标，有两个值         */
static uint8    GucXGoal1                           = XDST1;
static uint8    GucYGoal0                           = YDST0;            /*  终点Y坐标，有两个值         */
static uint8    GucYGoal1                           = YDST1;

static uint8    GucMouseTask                        = WAIT;             /*  状态机，初始状态为等待      */

static uint8    GucMapStep[MAZETYPE][MAZETYPE]      = {0xff};           /*  保存各坐标的等高值          */

static MAZECOOR GmcStack[MAZETYPE * MAZETYPE]       = {0};              /*  在mapStepEdit()中作堆栈使用 */
static MAZECOOR GmcCrossway[MAZETYPE * MAZETYPE]    = {0};              /*  Main()中暂存未走过支路坐标  */
/*********************************************************************************************************
** Function name:       Delay
** Descriptions:        延时函数
** input parameters:    uiD :延时参数，值越大，延时越久
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void delay (uint32 uiD)
{
    for (; uiD; uiD--);
}

/*********************************************************************************************************
** Function name:       mapStepEdit
** Descriptions:        制作以目标点为起点的等高图
** input parameters:    uiX:    目的地横坐标
**                      uiY:    目的地纵坐标
** output parameters:   GucMapStep[][]:  各坐标上的等高值
** Returned value:      无
*********************************************************************************************************/
void mapStepEdit (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]下标              */
    uint8 ucStep    = 1;                                                /*  等高值                      */
    uint8 ucStat    = 0;                                                /*  统计可前进的方向数          */
    uint8 i,j;
    
    GmcStack[n].cX  = cX;                                               /*  起点X值入栈                 */
    GmcStack[n].cY  = cY;                                               /*  起点Y值入栈                 */
    n++;
    /*
     *  初始化各坐标等高值
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep[i][j] = 0xff;
        }
    }
    /*
     *  制作等高图，直到堆栈中所有数据处理完毕
     */
    while (n) {
        GucMapStep[cX][cY] = ucStep++;                                  /*  填入等高值                  */

        /*
         *  对当前坐标格里可前进的方向统计
         */
        ucStat = 0;
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  前方有路                    */
            (GucMapStep[cX][cY + 1] > (ucStep))) {                      /*  前方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] > (ucStep))) {                      /*  右方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&
            (GucMapStep[cX][cY - 1] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&
            (GucMapStep[cX - 1][cY] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        /*
         *  没有可前进的方向，则跳转到最近保存的分支点
         *  否则任选一可前进方向前进
         */
        if (ucStat == 0) {
            n--;
            cX = GmcStack[n].cX;
            cY = GmcStack[n].cY;
            ucStep = GucMapStep[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  有多个可前进方向，保存坐标  */
                GmcStack[n].cX = cX;                                    /*  横坐标X值入栈               */
                GmcStack[n].cY = cY;                                    /*  纵坐标Y值入栈               */
                n++;
            }
            /*
             *  任意选择一条可前进的方向前进
             */
            if ((GucMapBlock[cX][cY] & 0x01) &&                         /*  上方有路                    */
                (GucMapStep[cX][cY + 1] > (ucStep))) {                  /*  上方等高值大于计划设定值    */
                cY++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x02) &&                         /*  右方有路                    */
                (GucMapStep[cX + 1][cY] > (ucStep))) {                  /*  右方等高值大于计划设定值    */
                cX++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x04) &&                         /*  下方有路                    */
                (GucMapStep[cX][cY - 1] > (ucStep))) {                  /*  下方等高值大于计划设定值    */
                cY--;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x08) &&                         /*  左方有路                    */
                (GucMapStep[cX - 1][cY] > (ucStep))) {                  /*  左方等高值大于计划设定值    */
                cX--;                                                   /*  修改坐标                    */
                continue;
            }
        }
    }
}
/*********************************************************************************************************
** Function name:       weight_mapStepEdit
** Descriptions:        制作以目标点为起点的加权等高图
** input parameters:    uiX:    目的地横坐标
**                      uiY:    目的地纵坐标
** output parameters:   GucMapStep[][]:  各坐标上的等高值
** Returned value:      无
*********************************************************************************************************/
void weight_mapStepEdit (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]下标              */
    uint8 ucStep    = 1;                                                /*  等高值                      */
    uint8 ucStat    = 0;                                                /*  统计可前进的方向数          */
    uint8 i,j;
    uint8 XGoal0 = cX;
    uint8 YGoal0 = cY;
    
    //初始方向要事先确定  1上 2右 4下 8左 0起点
    //uint8 dirold;
    uint8 dirnew;
    
    GmcStack[n].cX  = cX;                                               /*  起点X值入栈                 */
    GmcStack[n].cY  = cY;                                               /*  起点Y值入栈                 */
    n++;
    /*
     *  初始化各坐标等高值
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep[i][j] = 0xff;
        }
    }
    /*
     *  制作等高图，直到堆栈中所有数据处理完毕
     */
    while (n) {
        GucMapStep[cX][cY] = ucStep++;                                  /*  填入等高值                  */
        
        if (cX == XGoal0 && cY ==YGoal0)
	{
		if ((GucMapBlock[cX][cY] & 0x01)&& (GucMapStep[cX][cY + 1]-ucStep)>0){ /*  前方有路                    */
			dirnew =1;
			//dirold = 1;
		}
		else if ((GucMapBlock[cX][cY] & 0x02)&& (GucMapStep[cX+1][cY]-ucStep)>0){ /*  右方有路                    */
			dirnew =2;
			//dirold = 2;
		}
		else if ((GucMapBlock[cX][cY] & 0x04)&& (GucMapStep[cX][cY - 1]-ucStep)>0){ /*  后方有路                    */
			dirnew =4;
			//dirold = 4;
		}
		else if ((GucMapBlock[cX][cY] & 0x08)&& (GucMapStep[cX-1][cY ]-ucStep)>0){ /*  左方有路                    */
			dirnew =8;
			//dirold = 8;
		}
	}

        /*
         *  对当前坐标格里可前进的方向统计
         */
        ucStat = 0;
        if (GucMapBlock[cX][cY] & 0x01) {                      /*  前方有路                    */
	  if(dirnew==1 && (GucMapStep[cX][cY + 1]-ucStep)>0)   //相同方向  等高值大1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX][cY + 1]-ucStep)>1)         //不同方向  等高值大2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x02) {                      /*  右方有路                    */
	  if(dirnew==2 && (GucMapStep[cX + 1][cY]-ucStep)>0)   //相同方向  等高值大1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX + 1][cY]-ucStep)>1)         //不同方向  等高值大2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x04) {                      /*  下方有路                    */
	  if(dirnew==4 && (GucMapStep[cX][cY - 1]-ucStep)>0)   //相同方向  等高值大1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX][cY - 1]-ucStep)>1)         //不同方向  等高值大2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x08) {                      /*  左方有路                    */
	  if(dirnew==8 && (GucMapStep[cX - 1][cY]-ucStep)>0)   //相同方向  等高值大1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX - 1][cY]-ucStep)>1)         //不同方向  等高值大2
	  {
		ucStat++;
	  }
        }
        
        
        /*
         *  没有可前进的方向，则跳转到最近保存的分支点
         *  否则任选一可前进方向前进
         */
        if (ucStat == 0) {
            n--;
            cX = GmcStack[n].cX;
            cY = GmcStack[n].cY;
            dirnew = GmcStack[n].dir;
	    //dirold = GmcStack[n].dir;
            ucStep = GucMapStep[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  有多个可前进方向，保存坐标  */
                GmcStack[n].cX = cX;                                    /*  横坐标X值入栈               */
                GmcStack[n].cY = cY;                                    /*  纵坐标Y值入栈               */
                GmcStack[n].dir = dirnew;
                n++;
            }
            /*
             *  任意选择一条可前进的方向前进
             */
            if (GucMapBlock[cX][cY] & 0x01) {                  /*  上方等高值大于计划设定值    */
		if(dirnew==1 && (GucMapStep[cX][cY + 1]-ucStep)>0) //相同方向  等高值大1
		{
		    cY++;                                                   /*  修改坐标                    */					
		    continue;
		}
		else if ((GucMapStep[cX][cY + 1]-ucStep)>1)   //不同方向  等高值大2
		{
		    cY++;                                                   /*  修改坐标                    */
		    dirnew=1;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x02) {                  /*  右方等高值大于计划设定值    */
		if(dirnew==2 && (GucMapStep[cX+1][cY]-ucStep)>0) //相同方向  等高值大1
		{
		    cX++;                                                   /*  修改坐标                    */					
		    continue;
		}
		else if ((GucMapStep[cX+1][cY]-ucStep)>1)   //不同方向  等高值大2
		{
		    cX++;                                                   /*  修改坐标                    */
		    dirnew=2;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x04) {                  /*  下方等高值大于计划设定值    */
		if(dirnew==4 && (GucMapStep[cX][cY - 1]-ucStep)>0) //相同方向  等高值大1
		{
		    cY--;                                                   /*  修改坐标                    */					
		    continue;
		}
		else if ((GucMapStep[cX][cY - 1]-ucStep)>1)   //不同方向  等高值大2
		{
		    cY--;                                                   /*  修改坐标                    */
		    dirnew=4;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x08) {                  /*  左方等高值大于计划设定值    */
		if(dirnew==8 && (GucMapStep[cX-1][cY]-ucStep)>0) //相同方向  等高值大1
		{
		    cX--;                                                   /*  修改坐标                    */					
		    continue;
		}
		else if ((GucMapStep[cX-1][cY]-ucStep)>1)   //不同方向  等高值大2
		{
		    cX--;                                                  /*  修改坐标                    */
		    dirnew=8;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
        }
    }
}
/*********************************************************************************************************
** Function name:       mouseSpurt
** Descriptions:        电脑鼠从起点以最短路径跑向终点
** input parameters:    无
 ** output parameters:  无
** Returned value:      无
*********************************************************************************************************/
void mouseSpurt (void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
    /*
     *  对终点的四个坐标分别制作等高图
     *  取离起点最近的一个点作为目标点
     */
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     /*  判断该终点坐标是否有出口    */
        weight_mapStepEdit(GucXGoal0,GucYGoal0);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                     /*  判断该终点坐标是否有出口    */
        weight_mapStepEdit(GucXGoal0,GucYGoal1);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                     /*  判断该终点坐标是否有出口    */
        weight_mapStepEdit(GucXGoal1,GucYGoal0);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                     /*  判断该终点坐标是否有出口    */
        weight_mapStepEdit(GucXGoal1,GucYGoal1);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    objectGoTo(cXdst,cYdst);                                            /*  运行到指定目标点            */
}
/*********************************************************************************************************
** Function name:       objectGoTo
** Descriptions:        使电脑鼠运动到指定坐标
** input parameters:    cXdst: 目的地的横坐标
**                      cYdst: 目的地的纵坐标
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void objectGoTo (int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    weight_mapStepEdit(cXdst,cYdst);                                           /*  制作等高图                  */
    /*
     *  根据等高值向目标点运动，直到达到目的地
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        ucStep = GucMapStep[cX][cY];
        /*
         *  任选一个等高值比当前自身等高值小的方向前进
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  上方有路                    */
            (GucMapStep[cX][cY + 1] < ucStep)) {                        /*  上方等高值较小              */
            cDirTemp = UP;                                              /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  右方等高值较小              */
            cDirTemp = RIGHT;                                           /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  下方有路                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  下方等高值较小              */
            cDirTemp = DOWN;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY--;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  左方有路                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  左方等高值较小              */
            cDirTemp = LEFT;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX--;
                continue;                                               /*  跳过本次循环                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  计算方向偏移量              */
        
        if (cNBlock) {
            mouseGoahead(cNBlock);                                      /*  前进cNBlock步               */
        }        
        cNBlock = 0;                                                    /*  任务清零                    */
        
        /*
         *  控制电脑鼠转弯
         */
        switch (cDirTemp) {

        case 1:
            mouseTurnright();
            break;

        case 2:
            mouseTurnback();
            break;

        case 3:
            mouseTurnleft();
            break;

        default:
            break;
        }
    }
    /*
     *  判断任务是否完成，否则继续前进
     */
    if (cNBlock) {
        mouseGoahead(cNBlock);
    }
}
/*********************************************************************************************************
** Function name:       mazeBlockDataGet
** Descriptions:        根据电脑鼠的相对方向，取出该方向上迷宫格的墙壁资料
** input parameters:    ucDir: 电脑鼠的相对方向
** output parameters:   无
** Returned value:      GucMapBlock[cX][cY] : 墙壁资料
*********************************************************************************************************/
uint8 mazeBlockDataGet (uint8  ucDirTemp)
{
    int8 cX = 0,cY = 0;
    
    /*
     *  把电脑鼠的相对方向转换为绝对方向
     */
    switch (ucDirTemp) {

    case MOUSEFRONT:
        ucDirTemp = GucMouseDir;
        break;

    case MOUSELEFT:
        ucDirTemp = (GucMouseDir + 3) % 4;
        break;

    case MOUSERIGHT:
        ucDirTemp = (GucMouseDir + 1) % 4;
        break;

    default:
        break;
    }
    
    /*
     *  根据绝对方向计算该方向上相邻格的坐标
     */
    switch (ucDirTemp) {

    case 0:
        cX = GmcMouse.cX;
        cY = GmcMouse.cY + 1;
        break;
        
    case 1:
        cX = GmcMouse.cX + 1;
        cY = GmcMouse.cY;
        break;
        
    case 2:
        cX = GmcMouse.cX;
        cY = GmcMouse.cY - 1;
        break;
        
    case 3:
        cX = GmcMouse.cX - 1;
        cY = GmcMouse.cY;
        break;
        
    default:
        break;
    }
    
    return(GucMapBlock[cX][cY]);                                        /*  返回迷宫格上的资料          */
}
/*********************************************************************************************************
** Function name:       rightMethod
** Descriptions:        右手法则，优先向右前进
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void rightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       leftMethod
** Descriptions:        左手法则，优先向左运动
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void leftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontRightMethod
** Descriptions:        中右法则，优先向前运行，其次向右
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void frontRightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontLeftMethod
** Descriptions:        中左法则，优先向前运行，其次向左
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void frontLeftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       centralMethod
** Descriptions:        中心法则，根据电脑鼠目前在迷宫中所处的位置觉定使用何种搜索法则
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void centralMethod (void)
{
    if (GmcMouse.cX & 0x08) {
        if (GmcMouse.cY & 0x08) {

            /*
             *  此时电脑鼠在迷宫的右上角
             */ 
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠向上              */
                leftMethod();                                           /*  左手法则                    */
                break;

            case RIGHT:                                                 /*  当前电脑鼠向右              */
                rightMethod();                                          /*  右手法则                    */
                break;

            case DOWN:                                                  /*  当前电脑鼠向下              */
                frontRightMethod();                                     /*  中右法则                    */
                break;

            case LEFT:                                                  /*  当前电脑鼠向左              */
                frontLeftMethod();                                      /*  中左法则                    */
                break;

            default:
                break;
            }
        } else {

            /*
             *  此时电脑鼠在迷宫的右下角
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠向上              */
                frontLeftMethod();                                      /*  中左法则                    */
                break;

            case RIGHT:                                                 /*  当前电脑鼠向右              */
                leftMethod();                                           /*  左手法则                    */
                break;

            case DOWN:                                                  /*  当前电脑鼠向下              */
                rightMethod();                                          /*  右手法则                    */
                break;

            case LEFT:                                                  /*  当前电脑鼠向左              */
                frontRightMethod();                                     /*  中右法则                    */
                break;

            default:
                break;
            }
        }
    } else {
        if (GmcMouse.cY & 0x08) {

            /*
             *  此时电脑鼠在迷宫的左上角
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠向上              */
                rightMethod();                                          /*  右手法则                    */
                break;

            case RIGHT:                                                 /*  当前电脑鼠向右              */
                frontRightMethod();                                     /*  中右法则                    */
                break;

            case DOWN:                                                  /*  当前电脑鼠向下              */
                frontLeftMethod();                                      /*  中左法则                    */
                break;

            case LEFT:                                                  /*  当前电脑鼠向左              */
                leftMethod();                                           /*  左手法则                    */
                break;

            default:
                break;
            }
        } else {

            /*
             *  此时电脑鼠在迷宫的左下角
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠向上              */
                frontRightMethod();                                     /*  中右法则                    */
                break;

            case RIGHT:                                                 /*  当前电脑鼠向右              */
                frontLeftMethod();                                      /*  中左法则                    */
                break;

            case DOWN:                                                  /*  当前电脑鼠向下              */
                leftMethod();                                           /*  左手法则                    */
                break;

            case LEFT:                                                  /*  当前电脑鼠向左              */
                rightMethod();                                          /*  右手法则                    */
                break;

            default:
                break;
            }
        }
    }
}
/*********************************************************************************************************
** Function name:       crosswayCheck
** Descriptions:        统计某坐标存在还未走过的支路数
** input parameters:    ucX，需要检测点的横坐标
**                      ucY，需要检测点的纵坐标
** output parameters:   无
** Returned value:      ucCt，未走过的支路数
*********************************************************************************************************/
uint8 crosswayCheck (int8  cX, int8  cY)
{
    uint8 ucCt = 0;
    if ((GucMapBlock[cX][cY] & 0x01) &&                                 /*  绝对方向，迷宫上方有路      */
        (GucMapBlock[cX][cY + 1]) == 0x00) {                            /*  绝对方向，迷宫上方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x02) &&                                 /*  绝对方向，迷宫右方有路      */
        (GucMapBlock[cX + 1][cY]) == 0x00) {                            /*  绝对方向，迷宫右方没有走过  */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x04) &&                                 /*  绝对方向，迷宫下方有路      */
        (GucMapBlock[cX][cY - 1]) == 0x00) {                            /*  绝对方向，迷宫下方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x08) &&                                 /*  绝对方向，迷宫左方有路      */
        (GucMapBlock[cX - 1][cY]) == 0x00) {                            /*  绝对方向，迷宫左方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    return ucCt;
}
/*********************************************************************************************************
** Function name:       crosswayChoice
** Descriptions:        选择一条支路作为前进方向
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void crosswayChoice (void)
{
    switch (SEARCHMETHOD) {
        
    case RIGHTMETHOD:
        rightMethod();
        break;
    
    case LEFTMETHOD:
        leftMethod();
        break;
    
    case CENTRALMETHOD:
        centralMethod();
        break;

    case FRONTRIGHTMETHOD:
        frontRightMethod();
        break;

    case FRONTLEFTMETHOD:
        frontLeftMethod();
        break;

    default:
        break;
    }
}
/*********************************************************************************************************
** Function name:       smartStack
** Descriptions:        智能退栈  选择一个离当前点最近的分支点
** input parameters:    uint8 n   栈中记录的分支点个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void smartStack(uint8 n)
{
  if(n<1)//GmcCrossway[0]为起始点  不参与排序
    return;
  uint8 record = 1;
  uint8 length = 0xff;
  
  for(uint8 i = 1;i<=n;i++)
  {
    if(GucMapStep[GmcCrossway[i].cX][GmcCrossway[i].cY]<length)//等高值小
    {
      record = i;
      length = GucMapStep[GmcCrossway[i].cX][GmcCrossway[i].cY];
    }
  }
  
  GmcCrossway[n+1].cX = GmcCrossway[record].cX;//最小值压入栈顶
  GmcCrossway[n+1].cY = GmcCrossway[record].cY;
  
  for(uint8 i = record;i<=n;i++)
  {
    GmcCrossway[i].cX = GmcCrossway[i+1].cX;//依次顺移
    GmcCrossway[i].cY = GmcCrossway[i+1].cY;
  }
}
/*********************************************************************************************************
** Function name:       patchmaze
** Descriptions:        检测退栈点是否都是死路
** input parameters:    uint8 cX uint8 cY  退栈点坐标
** output parameters:   1 需要退回   0  都是死路  不需要退回
** Returned value:      无
*********************************************************************************************************/
uint8 patchmaze(uint8 cX,uint8 cY)   //退栈点  1 需要退回   0  都是死路  不需要退回
{
  uint8 flag1 = 0;
  uint8 flag2 = 0;
  uint8 flag3 = 0;
  uint8 flag4 = 0;
  uint8 tempX,tempY; 
  tempX=cX;
  tempY=cY;
  if((GucMapBlock[cX][cY] & 0x01) &&                             /*   上方有路                   */
            GucMapBlock[cX][cY + 1] == 0x00)                   //前方没有走过
  {
    tempX = cX;
    tempY = cY+1;
    while(1){
      if(return_direct_info(tempX,tempY,1)==1)
      {
        flag1 = 1;
        break;
      }
      else {
        tempY++;
        //判断前方是否有挡板
        if(tempY<=7)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x04) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //前方没路 表明这条路是死路
            GucMapBlock[cX][cY + 1] = 0x04;
            break;
          }
        }
        else
        {
          GucMapBlock[cX][cY + 1] = 0x04;
          break;
        }
      }
    }
 }
  if((GucMapBlock[cX][cY] & 0x08) &&                             /*  左方有路                    */
            GucMapBlock[cX-1][cY] == 0x00)                  //前方没有走过
  {
    tempX = cX-1;
    tempY = cY;
    while(1){
      if(return_direct_info(tempX,tempY,8))
      {
        flag2 = 1;
        break;
      }
      else {
        tempX--;
        //判断前方是否有挡板
        if(tempX >= 0)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x02) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //前方没路 表明这条路是死路
            GucMapBlock[cX-1][cY] = 0x02;
            break;
          }
        }
        else 
        {
          GucMapBlock[cX-1][cY] = 0x02; 
          break;
        }
      }
    }
 }
 if((GucMapBlock[cX][cY] & 0x04) &&                             /*  下方有路                    */
            GucMapBlock[cX][cY-1] == 0x00)                  //前方没有走过
  {
    tempX = cX;
    tempY = cY-1;
    while(1){
      if(return_direct_info(tempX,tempY,4))
      {
        flag3 = 1;
        break;
      }
      else {
        tempY--;
        //判断前方是否有挡板
        if(tempY>=0)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x01) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //前方没路 表明这条路是死路
            GucMapBlock[cX][cY-1] = 0x01;
            break;
          }
        }
        else
        { 
          GucMapBlock[cX][cY-1] = 0x01; 
          break;
        }
      }
    }
 }
 if((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                   */
            GucMapBlock[cX+1][cY] == 0x00)                  //前方没有走过
  {
    tempX = cX+1;
    tempY = cY;
    while(1){
      if(return_direct_info(tempX,tempY,2))
      {
        flag4 = 1;
        break;
      }
      else {
        tempX++;
        //判断前方是否有挡板
        if(tempX<=15)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x08) && GucMapBlock[tempX][tempY]!=0x00 )
          {
            //前方没路 表明这条路是死路
            GucMapBlock[cX+1][cY] = 0x08;
            break;
          }
        }
        else
        { 
          GucMapBlock[cX+1][cY] = 0x08;
          break;
        }
        }
    }
 }
 
   if(!flag1 && !flag2 && !flag3 && !flag4)
     return 0;
  else
    return 1;
}

/*********************************************************************************************************
** Function name:       return_direct_info
** Descriptions:        判断当前点左右时否有挡板
** input parameters:    uint8 cX uint8 cY  当前点坐标  dir  绝对方向
** output parameters:   返回1 没有挡板 返回0 有挡板
** Returned value:      无
*********************************************************************************************************/
uint8 return_direct_info(uint8 cX,uint8 cY,uint8 dir)//左8下4右2上1   返回1 没有挡板 返回0 有挡板
{
  if(cX== 7 && cY == 7)     //当前点是终点  必须要走
    return 1;
  if(cX== 8 && cY == 7)    //当前点是终点  必须要走
    return 1;
  
  switch(dir){
  case 1:   //上方有路
    if(cX==0 && !(GucMapBlock[cX+1][cY] & 0x08) && GucMapBlock[cX+1][cY]!=0x00)    //边界条件判断
      return 0;
    if((cX > 0) && (cX<15) && !(GucMapBlock[cX-1][cY] & 0x02) &&!(GucMapBlock[cX+1][cY] & 0x08)  
       && GucMapBlock[cX-1][cY]!=0x00 && GucMapBlock[cX+1][cY]!=0x00)//根据当前点左右两个点判断左右是否有挡板
      return 0;
    if((cX == 15) && !(GucMapBlock[cX-1][cY] & 0x02) && GucMapBlock[cX-1][cY]!=0x00)  //边界条件判断
      return 0;
    
    return 1;
  
  case 2:
    if(cY==0 && !(GucMapBlock[cX][cY+1] & 0x04) && GucMapBlock[cX][cY+1]!=0x00 )    //边界条件判断
      return 0;
    if((cY > 0) && (cY < 7) && !(GucMapBlock[cX][cY-1] & 0x01) &&!(GucMapBlock[cX][cY+1] & 0x04)   
       && GucMapBlock[cX][cY+1]!=0x00 && GucMapBlock[cX][cY-1]!=0x00) //根据当前点左右两个点判断左右是否有挡板 
      return 0;
    if((cY == 7) && !(GucMapBlock[cX][cY-1] & 0x01) && GucMapBlock[cX][cY-1]!=0x00)  //边界条件判断
      return 0;   
    return 1;

  case 4:
    if(cX==0 && !(GucMapBlock[cX+1][cY] & 0x08) && GucMapBlock[cX+1][cY]!=0x00)    //边界条件判断
      return 0;
    if((cX > 0) && (cX<15) && !(GucMapBlock[cX-1][cY] & 0x02) &&!(GucMapBlock[cX+1][cY] & 0x08) 
       && GucMapBlock[cX+1][cY]!=0x00 && GucMapBlock[cX-1][cY]!=0x00)//根据当前点左右两个点判断左右是否有挡板 
      return 0;
    if((cX == 15) && !(GucMapBlock[cX-1][cY] & 0x02) && GucMapBlock[cX-1][cY]!=0x00) //边界条件判断
      return 0;
    
    return 1;
      
   case 8:
    if(cY==0 && !(GucMapBlock[cX][cY+1] & 0x04) && GucMapBlock[cX][cY+1]!=0x00)    //边界条件判断
      return 0;
    if((cY > 0) && (cY < 7) && !(GucMapBlock[cX][cY-1] & 0x01) &&!(GucMapBlock[cX][cY+1] & 0x04) 
       && GucMapBlock[cX][cY+1]!=0x00 && GucMapBlock[cX][cY-1]!=0x00)//根据当前点左右两个点判断左右是否有挡板 
      return 0;
    if((cY == 7) && !(GucMapBlock[cX][cY-1] & 0x01) && GucMapBlock[cX][cY-1]!=0x00)  //边界条件判断
      return 0;
    
    return 1;   
  }
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        主函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
main (void)
{
    uint8 n          = 0;                                               /*  GmcCrossway[]下标           */
    uint8 ucRoadStat = 0;                                               /*  统计某一坐标可前进的支路数  */
    uint8 ucTemp     = 0;                                               /*  用于START状态中坐标转换     */

    mouseInit();                                                        /*  底层驱动的初始化            */
    zlg7289Init();                                                      /*  显示模块初始化              */

    while (1) {
        switch (GucMouseTask) {                                         /*  状态机处理                  */
            
        case WAIT:
            sensorDebug();
            voltageDetect();
            delay(100000);
            if (keyCheck() == true) {                                   /*  检测按键等待启动            */
                zlg7289Reset();                                         /*  复位ZLG7289                 */
                GucMouseTask = START;
            }
            break;
            
        case START:                                                     /*  判断电脑鼠起点的横坐标      */
            mazeSearch();                                               /*  向前搜索                    */
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x08) {         /*  判断电老鼠左边是否存在出口  */
                if (MAZETYPE == 8) {                                    /*  修改四分之一迷宫的终点坐标  */
                    GucXGoal0 = 1;
                    GucXGoal1 = 0;
                }
                GucXStart   = MAZETYPE - 1;                             /*  修改电脑鼠起点的横坐标      */
                GmcMouse.cX = MAZETYPE - 1;                             /*  修改电脑鼠当前位置的横坐标  */    
                /*
                 *  由于默认的起点为(0,0)，现在需要把已记录的墙壁资料转换过来
                 */
                ucTemp = GmcMouse.cY;
                do {
                    GucMapBlock[MAZETYPE - 1][ucTemp] = GucMapBlock[0][ucTemp];
                    GucMapBlock[0 ][ucTemp] = 0;
                }while (ucTemp--);
                /*
                 *  在OFFSHOOT[0]中保存起点坐标
                 */
                GmcCrossway[n].cX = MAZETYPE - 1;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  状态转换为搜寻状态          */
            }
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x02) {         /*  判断电老鼠右边是否存在出口  */
                /*
                 *  在OFFSHOOT[0]中保存起点坐标
                 */
                GmcCrossway[n].cX = 0;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  状态转换为搜寻状态          */
            }
            break;
            
        case MAZESEARCH:
          
          //电脑鼠部分搜索
          
            if (GucMapBlock[GucXGoal0][GucYGoal0] != 0){
             weight_mapStepEdit(GucXGoal0,GucYGoal0);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  电脑鼠将开始冲刺状态        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal0][GucYGoal1] != 0){
             weight_mapStepEdit(GucXGoal0,GucYGoal1);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  电脑鼠将开始冲刺状态        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal1][GucYGoal0] != 0){
             weight_mapStepEdit(GucXGoal1,GucYGoal0);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  电脑鼠将开始冲刺状态        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal1][GucYGoal1] != 0){
             weight_mapStepEdit(GucXGoal1,GucYGoal1);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  电脑鼠将开始冲刺状态        */
               break;
             }
           }
            ucRoadStat = crosswayCheck(GmcMouse.cX,GmcMouse.cY);        /*  统计可前进的支路数          */
            if (ucRoadStat) {                                           /*  有可前进方向                */
                if (ucRoadStat > 1) {                                   /*  有多条可前进方向，保存坐标  */
                    GmcCrossway[n].cX = GmcMouse.cX;
                    GmcCrossway[n].cY = GmcMouse.cY;
                    n++;
                }
                crosswayChoice();                                       /*  用右手法则搜索选择前进方向  */
                mazeSearch();                                           /*  前进一格                    */
            } else {                                                    /*  没有可前进方向，回到最近支路*/
              
                weight_mapStepEdit(GmcMouse.cX,GmcMouse.cY);            //以当前点做加权等高图   智能退栈只需要做一次加权等高图
                
                while (--n) {
                  
                  
                  smartStack(n);
                  
                   ucRoadStat = crosswayCheck(GmcCrossway[n].cX,
                                               GmcCrossway[n].cY);
                                                                        /*  统计最近支点未走过的方向数  */
                                                                        
                    /*
                     *  若存在未走过的路，则前往该支点，并跳出循环
                     *  否则继续查找还有未走过的支路。
                     */
                   if(!patchmaze(GmcCrossway[n].cX,GmcCrossway[n].cY)){
                     continue;
                   }
                    if (ucRoadStat) {
                      
                        
                        objectGoTo(GmcCrossway[n].cX,
                                   GmcCrossway[n].cY);
                        if (ucRoadStat > 1) {
                            n++;
                        }
                        crosswayChoice();
                        mazeSearch();
                        break;
                    }
                }
                if (n == 0) {                                           /*  走完了所有的支路，回到起点  */
                    objectGoTo(GmcCrossway[0].cX, GmcCrossway[0].cY);
                    mouseTurnback();
                    GucMouseTask = SPURT;                               /*  电脑鼠将开始冲刺状态        */
                }
            }
            break;

        case SPURT:
            mouseSpurt();                                               /*  以最优路径冲向终点          */
            objectGoTo(GucXStart,GucYStart);                            /*  回到起点                    */
            mouseTurnback();                                            /*  向后转，恢复出发姿势        */
            while (1) {
                if (keyCheck() == true) {
                    break;
                }
                sensorDebug();
                delay(20000);
            }
            break;

        default:
            break;
        }
    }
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
