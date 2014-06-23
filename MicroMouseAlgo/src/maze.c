/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**mouseGoahead()û�б�Ҫ��������   ȫ�����ܼ���

**--------------File Info---------------------------------------------------------------------------------
** File Name:           maze.c
** Last modified Date:  2007/09/24
** Last Version:        V1.0
** Description:         ���ݵײ����ȡ�õ��Թ���Ϣ�������������㷨���Ƶ��������һ״̬���������ײ�������
**                      ��ִ�С�
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
  ����ͷ�ļ�
*********************************************************************************************************/
#include "Maze.h"


/*********************************************************************************************************
  ȫ�ֱ�������
*********************************************************************************************************/
static uint8    GucXStart                           = 0;                /*  ��������                  */
static uint8    GucYStart                           = 0;                /*  ���������                  */

static uint8    GucXGoal0                           = XDST0;            /*  �յ�X���꣬������ֵ         */
static uint8    GucXGoal1                           = XDST1;
static uint8    GucYGoal0                           = YDST0;            /*  �յ�Y���꣬������ֵ         */
static uint8    GucYGoal1                           = YDST1;

static uint8    GucMouseTask                        = WAIT;             /*  ״̬������ʼ״̬Ϊ�ȴ�      */

static uint8    GucMapStep[MAZETYPE][MAZETYPE]      = {0xff};           /*  ���������ĵȸ�ֵ          */

static MAZECOOR GmcStack[MAZETYPE * MAZETYPE]       = {0};              /*  ��mapStepEdit()������ջʹ�� */
static MAZECOOR GmcCrossway[MAZETYPE * MAZETYPE]    = {0};              /*  Main()���ݴ�δ�߹�֧·����  */
/*********************************************************************************************************
** Function name:       Delay
** Descriptions:        ��ʱ����
** input parameters:    uiD :��ʱ������ֵԽ����ʱԽ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void delay (uint32 uiD)
{
    for (; uiD; uiD--);
}

/*********************************************************************************************************
** Function name:       mapStepEdit
** Descriptions:        ������Ŀ���Ϊ���ĵȸ�ͼ
** input parameters:    uiX:    Ŀ�ĵغ�����
**                      uiY:    Ŀ�ĵ�������
** output parameters:   GucMapStep[][]:  �������ϵĵȸ�ֵ
** Returned value:      ��
*********************************************************************************************************/
void mapStepEdit (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]�±�              */
    uint8 ucStep    = 1;                                                /*  �ȸ�ֵ                      */
    uint8 ucStat    = 0;                                                /*  ͳ�ƿ�ǰ���ķ�����          */
    uint8 i,j;
    
    GmcStack[n].cX  = cX;                                               /*  ���Xֵ��ջ                 */
    GmcStack[n].cY  = cY;                                               /*  ���Yֵ��ջ                 */
    n++;
    /*
     *  ��ʼ��������ȸ�ֵ
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep[i][j] = 0xff;
        }
    }
    /*
     *  �����ȸ�ͼ��ֱ����ջ���������ݴ������
     */
    while (n) {
        GucMapStep[cX][cY] = ucStep++;                                  /*  ����ȸ�ֵ                  */

        /*
         *  �Ե�ǰ��������ǰ���ķ���ͳ��
         */
        ucStat = 0;
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  ǰ����·                    */
            (GucMapStep[cX][cY + 1] > (ucStep))) {                      /*  ǰ���ȸ�ֵ���ڼƻ��趨ֵ    */
            ucStat++;                                                   /*  ��ǰ����������1             */
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  �ҷ���·                    */
            (GucMapStep[cX + 1][cY] > (ucStep))) {                      /*  �ҷ��ȸ�ֵ���ڼƻ��趨ֵ    */
            ucStat++;                                                   /*  ��ǰ����������1             */
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&
            (GucMapStep[cX][cY - 1] > (ucStep))) {
            ucStat++;                                                   /*  ��ǰ����������1             */
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&
            (GucMapStep[cX - 1][cY] > (ucStep))) {
            ucStat++;                                                   /*  ��ǰ����������1             */
        }
        /*
         *  û�п�ǰ���ķ�������ת���������ķ�֧��
         *  ������ѡһ��ǰ������ǰ��
         */
        if (ucStat == 0) {
            n--;
            cX = GmcStack[n].cX;
            cY = GmcStack[n].cY;
            ucStep = GucMapStep[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  �ж����ǰ�����򣬱�������  */
                GmcStack[n].cX = cX;                                    /*  ������Xֵ��ջ               */
                GmcStack[n].cY = cY;                                    /*  ������Yֵ��ջ               */
                n++;
            }
            /*
             *  ����ѡ��һ����ǰ���ķ���ǰ��
             */
            if ((GucMapBlock[cX][cY] & 0x01) &&                         /*  �Ϸ���·                    */
                (GucMapStep[cX][cY + 1] > (ucStep))) {                  /*  �Ϸ��ȸ�ֵ���ڼƻ��趨ֵ    */
                cY++;                                                   /*  �޸�����                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x02) &&                         /*  �ҷ���·                    */
                (GucMapStep[cX + 1][cY] > (ucStep))) {                  /*  �ҷ��ȸ�ֵ���ڼƻ��趨ֵ    */
                cX++;                                                   /*  �޸�����                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x04) &&                         /*  �·���·                    */
                (GucMapStep[cX][cY - 1] > (ucStep))) {                  /*  �·��ȸ�ֵ���ڼƻ��趨ֵ    */
                cY--;                                                   /*  �޸�����                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x08) &&                         /*  ����·                    */
                (GucMapStep[cX - 1][cY] > (ucStep))) {                  /*  �󷽵ȸ�ֵ���ڼƻ��趨ֵ    */
                cX--;                                                   /*  �޸�����                    */
                continue;
            }
        }
    }
}
/*********************************************************************************************************
** Function name:       weight_mapStepEdit
** Descriptions:        ������Ŀ���Ϊ���ļ�Ȩ�ȸ�ͼ
** input parameters:    uiX:    Ŀ�ĵغ�����
**                      uiY:    Ŀ�ĵ�������
** output parameters:   GucMapStep[][]:  �������ϵĵȸ�ֵ
** Returned value:      ��
*********************************************************************************************************/
void weight_mapStepEdit (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]�±�              */
    uint8 ucStep    = 1;                                                /*  �ȸ�ֵ                      */
    uint8 ucStat    = 0;                                                /*  ͳ�ƿ�ǰ���ķ�����          */
    uint8 i,j;
    uint8 XGoal0 = cX;
    uint8 YGoal0 = cY;
    
    //��ʼ����Ҫ����ȷ��  1�� 2�� 4�� 8�� 0���
    //uint8 dirold;
    uint8 dirnew;
    
    GmcStack[n].cX  = cX;                                               /*  ���Xֵ��ջ                 */
    GmcStack[n].cY  = cY;                                               /*  ���Yֵ��ջ                 */
    n++;
    /*
     *  ��ʼ��������ȸ�ֵ
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep[i][j] = 0xff;
        }
    }
    /*
     *  �����ȸ�ͼ��ֱ����ջ���������ݴ������
     */
    while (n) {
        GucMapStep[cX][cY] = ucStep++;                                  /*  ����ȸ�ֵ                  */
        
        if (cX == XGoal0 && cY ==YGoal0)
	{
		if ((GucMapBlock[cX][cY] & 0x01)&& (GucMapStep[cX][cY + 1]-ucStep)>0){ /*  ǰ����·                    */
			dirnew =1;
			//dirold = 1;
		}
		else if ((GucMapBlock[cX][cY] & 0x02)&& (GucMapStep[cX+1][cY]-ucStep)>0){ /*  �ҷ���·                    */
			dirnew =2;
			//dirold = 2;
		}
		else if ((GucMapBlock[cX][cY] & 0x04)&& (GucMapStep[cX][cY - 1]-ucStep)>0){ /*  ����·                    */
			dirnew =4;
			//dirold = 4;
		}
		else if ((GucMapBlock[cX][cY] & 0x08)&& (GucMapStep[cX-1][cY ]-ucStep)>0){ /*  ����·                    */
			dirnew =8;
			//dirold = 8;
		}
	}

        /*
         *  �Ե�ǰ��������ǰ���ķ���ͳ��
         */
        ucStat = 0;
        if (GucMapBlock[cX][cY] & 0x01) {                      /*  ǰ����·                    */
	  if(dirnew==1 && (GucMapStep[cX][cY + 1]-ucStep)>0)   //��ͬ����  �ȸ�ֵ��1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX][cY + 1]-ucStep)>1)         //��ͬ����  �ȸ�ֵ��2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x02) {                      /*  �ҷ���·                    */
	  if(dirnew==2 && (GucMapStep[cX + 1][cY]-ucStep)>0)   //��ͬ����  �ȸ�ֵ��1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX + 1][cY]-ucStep)>1)         //��ͬ����  �ȸ�ֵ��2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x04) {                      /*  �·���·                    */
	  if(dirnew==4 && (GucMapStep[cX][cY - 1]-ucStep)>0)   //��ͬ����  �ȸ�ֵ��1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX][cY - 1]-ucStep)>1)         //��ͬ����  �ȸ�ֵ��2
	  {
		ucStat++;
	  }
        }
        if (GucMapBlock[cX][cY] & 0x08) {                      /*  ����·                    */
	  if(dirnew==8 && (GucMapStep[cX - 1][cY]-ucStep)>0)   //��ͬ����  �ȸ�ֵ��1
	  {
		ucStat++;
	  }
	  else if ((GucMapStep[cX - 1][cY]-ucStep)>1)         //��ͬ����  �ȸ�ֵ��2
	  {
		ucStat++;
	  }
        }
        
        
        /*
         *  û�п�ǰ���ķ�������ת���������ķ�֧��
         *  ������ѡһ��ǰ������ǰ��
         */
        if (ucStat == 0) {
            n--;
            cX = GmcStack[n].cX;
            cY = GmcStack[n].cY;
            dirnew = GmcStack[n].dir;
	    //dirold = GmcStack[n].dir;
            ucStep = GucMapStep[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  �ж����ǰ�����򣬱�������  */
                GmcStack[n].cX = cX;                                    /*  ������Xֵ��ջ               */
                GmcStack[n].cY = cY;                                    /*  ������Yֵ��ջ               */
                GmcStack[n].dir = dirnew;
                n++;
            }
            /*
             *  ����ѡ��һ����ǰ���ķ���ǰ��
             */
            if (GucMapBlock[cX][cY] & 0x01) {                  /*  �Ϸ��ȸ�ֵ���ڼƻ��趨ֵ    */
		if(dirnew==1 && (GucMapStep[cX][cY + 1]-ucStep)>0) //��ͬ����  �ȸ�ֵ��1
		{
		    cY++;                                                   /*  �޸�����                    */					
		    continue;
		}
		else if ((GucMapStep[cX][cY + 1]-ucStep)>1)   //��ͬ����  �ȸ�ֵ��2
		{
		    cY++;                                                   /*  �޸�����                    */
		    dirnew=1;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x02) {                  /*  �ҷ��ȸ�ֵ���ڼƻ��趨ֵ    */
		if(dirnew==2 && (GucMapStep[cX+1][cY]-ucStep)>0) //��ͬ����  �ȸ�ֵ��1
		{
		    cX++;                                                   /*  �޸�����                    */					
		    continue;
		}
		else if ((GucMapStep[cX+1][cY]-ucStep)>1)   //��ͬ����  �ȸ�ֵ��2
		{
		    cX++;                                                   /*  �޸�����                    */
		    dirnew=2;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x04) {                  /*  �·��ȸ�ֵ���ڼƻ��趨ֵ    */
		if(dirnew==4 && (GucMapStep[cX][cY - 1]-ucStep)>0) //��ͬ����  �ȸ�ֵ��1
		{
		    cY--;                                                   /*  �޸�����                    */					
		    continue;
		}
		else if ((GucMapStep[cX][cY - 1]-ucStep)>1)   //��ͬ����  �ȸ�ֵ��2
		{
		    cY--;                                                   /*  �޸�����                    */
		    dirnew=4;
		    ucStep++;
		    //dirold = dirnew;
		    continue;
		}
            }
            if (GucMapBlock[cX][cY] & 0x08) {                  /*  �󷽵ȸ�ֵ���ڼƻ��趨ֵ    */
		if(dirnew==8 && (GucMapStep[cX-1][cY]-ucStep)>0) //��ͬ����  �ȸ�ֵ��1
		{
		    cX--;                                                   /*  �޸�����                    */					
		    continue;
		}
		else if ((GucMapStep[cX-1][cY]-ucStep)>1)   //��ͬ����  �ȸ�ֵ��2
		{
		    cX--;                                                  /*  �޸�����                    */
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
** Descriptions:        ���������������·�������յ�
** input parameters:    ��
 ** output parameters:  ��
** Returned value:      ��
*********************************************************************************************************/
void mouseSpurt (void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
    /*
     *  ���յ���ĸ�����ֱ������ȸ�ͼ
     *  ȡ����������һ������ΪĿ���
     */
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     /*  �жϸ��յ������Ƿ��г���    */
        weight_mapStepEdit(GucXGoal0,GucYGoal0);                               /*  �����ȸ�ͼ                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ������������������        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                     /*  �жϸ��յ������Ƿ��г���    */
        weight_mapStepEdit(GucXGoal0,GucYGoal1);                               /*  �����ȸ�ͼ                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ������������������        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                     /*  �жϸ��յ������Ƿ��г���    */
        weight_mapStepEdit(GucXGoal1,GucYGoal0);                               /*  �����ȸ�ͼ                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ������������������        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                     /*  �жϸ��յ������Ƿ��г���    */
        weight_mapStepEdit(GucXGoal1,GucYGoal1);                               /*  �����ȸ�ͼ                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ������������������        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    objectGoTo(cXdst,cYdst);                                            /*  ���е�ָ��Ŀ���            */
}
/*********************************************************************************************************
** Function name:       objectGoTo
** Descriptions:        ʹ�������˶���ָ������
** input parameters:    cXdst: Ŀ�ĵصĺ�����
**                      cYdst: Ŀ�ĵص�������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void objectGoTo (int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    weight_mapStepEdit(cXdst,cYdst);                                           /*  �����ȸ�ͼ                  */
    /*
     *  ���ݵȸ�ֵ��Ŀ����˶���ֱ���ﵽĿ�ĵ�
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        ucStep = GucMapStep[cX][cY];
        /*
         *  ��ѡһ���ȸ�ֵ�ȵ�ǰ����ȸ�ֵС�ķ���ǰ��
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  �Ϸ���·                    */
            (GucMapStep[cX][cY + 1] < ucStep)) {                        /*  �Ϸ��ȸ�ֵ��С              */
            cDirTemp = UP;                                              /*  ��¼����                    */
            if (cDirTemp == GucMouseDir) {                              /*  ����ѡ����Ҫת��ķ���    */
                cNBlock++;                                              /*  ǰ��һ������                */
                cY++;
                continue;                                               /*  ��������ѭ��                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  �ҷ���·                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  �ҷ��ȸ�ֵ��С              */
            cDirTemp = RIGHT;                                           /*  ��¼����                    */
            if (cDirTemp == GucMouseDir) {                              /*  ����ѡ����Ҫת��ķ���    */
                cNBlock++;                                              /*  ǰ��һ������                */
                cX++;
                continue;                                               /*  ��������ѭ��                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  �·���·                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  �·��ȸ�ֵ��С              */
            cDirTemp = DOWN;                                            /*  ��¼����                    */
            if (cDirTemp == GucMouseDir) {                              /*  ����ѡ����Ҫת��ķ���    */
                cNBlock++;                                              /*  ǰ��һ������                */
                cY--;
                continue;                                               /*  ��������ѭ��                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  ����·                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  �󷽵ȸ�ֵ��С              */
            cDirTemp = LEFT;                                            /*  ��¼����                    */
            if (cDirTemp == GucMouseDir) {                              /*  ����ѡ����Ҫת��ķ���    */
                cNBlock++;                                              /*  ǰ��һ������                */
                cX--;
                continue;                                               /*  ��������ѭ��                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  ���㷽��ƫ����              */
        
        if (cNBlock) {
            mouseGoahead(cNBlock);                                      /*  ǰ��cNBlock��               */
        }        
        cNBlock = 0;                                                    /*  ��������                    */
        
        /*
         *  ���Ƶ�����ת��
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
     *  �ж������Ƿ���ɣ��������ǰ��
     */
    if (cNBlock) {
        mouseGoahead(cNBlock);
    }
}
/*********************************************************************************************************
** Function name:       mazeBlockDataGet
** Descriptions:        ���ݵ��������Է���ȡ���÷������Թ����ǽ������
** input parameters:    ucDir: ���������Է���
** output parameters:   ��
** Returned value:      GucMapBlock[cX][cY] : ǽ������
*********************************************************************************************************/
uint8 mazeBlockDataGet (uint8  ucDirTemp)
{
    int8 cX = 0,cY = 0;
    
    /*
     *  �ѵ��������Է���ת��Ϊ���Է���
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
     *  ���ݾ��Է������÷��������ڸ������
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
    
    return(GucMapBlock[cX][cY]);                                        /*  �����Թ����ϵ�����          */
}
/*********************************************************************************************************
** Function name:       rightMethod
** Descriptions:        ���ַ�����������ǰ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void rightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  ��������ұ���·            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  ��������ұ�û���߹�        */
        mouseTurnright();                                               /*  ��������ת                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  �������ǰ����·            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  �������ǰ��û���߹�        */
        return;                                                         /*  ��������ת��              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  ������������·            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  ����������û���߹�        */
        mouseTurnleft();                                                /*  ��������ת                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       leftMethod
** Descriptions:        ���ַ������������˶�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void leftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  ������������·            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  ����������û���߹�        */
        mouseTurnleft();                                                /*  ��������ת                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  �������ǰ����·            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  �������ǰ��û���߹�        */
        return;                                                         /*  ��������ת��              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  ��������ұ���·            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  ��������ұ�û���߹�        */
        mouseTurnright();                                               /*  ��������ת                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontRightMethod
** Descriptions:        ���ҷ���������ǰ���У��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void frontRightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  �������ǰ����·            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  �������ǰ��û���߹�        */
        return;                                                         /*  ��������ת��              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  ��������ұ���·            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  ��������ұ�û���߹�        */
        mouseTurnright();                                               /*  ��������ת                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  ������������·            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  ����������û���߹�        */
        mouseTurnleft();                                                /*  ��������ת                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontLeftMethod
** Descriptions:        ������������ǰ���У��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void frontLeftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  �������ǰ����·            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  �������ǰ��û���߹�        */
        return;                                                         /*  ��������ת��              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  ������������·            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  ����������û���߹�        */
        mouseTurnleft();                                                /*  ��������ת                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  ��������ұ���·            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  ��������ұ�û���߹�        */
        mouseTurnright();                                               /*  ��������ת                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       centralMethod
** Descriptions:        ���ķ��򣬸��ݵ�����Ŀǰ���Թ���������λ�þ���ʹ�ú�����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void centralMethod (void)
{
    if (GmcMouse.cX & 0x08) {
        if (GmcMouse.cY & 0x08) {

            /*
             *  ��ʱ���������Թ������Ͻ�
             */ 
            switch (GucMouseDir) {
                
            case UP:                                                    /*  ��ǰ����������              */
                leftMethod();                                           /*  ���ַ���                    */
                break;

            case RIGHT:                                                 /*  ��ǰ����������              */
                rightMethod();                                          /*  ���ַ���                    */
                break;

            case DOWN:                                                  /*  ��ǰ����������              */
                frontRightMethod();                                     /*  ���ҷ���                    */
                break;

            case LEFT:                                                  /*  ��ǰ����������              */
                frontLeftMethod();                                      /*  ������                    */
                break;

            default:
                break;
            }
        } else {

            /*
             *  ��ʱ���������Թ������½�
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  ��ǰ����������              */
                frontLeftMethod();                                      /*  ������                    */
                break;

            case RIGHT:                                                 /*  ��ǰ����������              */
                leftMethod();                                           /*  ���ַ���                    */
                break;

            case DOWN:                                                  /*  ��ǰ����������              */
                rightMethod();                                          /*  ���ַ���                    */
                break;

            case LEFT:                                                  /*  ��ǰ����������              */
                frontRightMethod();                                     /*  ���ҷ���                    */
                break;

            default:
                break;
            }
        }
    } else {
        if (GmcMouse.cY & 0x08) {

            /*
             *  ��ʱ���������Թ������Ͻ�
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  ��ǰ����������              */
                rightMethod();                                          /*  ���ַ���                    */
                break;

            case RIGHT:                                                 /*  ��ǰ����������              */
                frontRightMethod();                                     /*  ���ҷ���                    */
                break;

            case DOWN:                                                  /*  ��ǰ����������              */
                frontLeftMethod();                                      /*  ������                    */
                break;

            case LEFT:                                                  /*  ��ǰ����������              */
                leftMethod();                                           /*  ���ַ���                    */
                break;

            default:
                break;
            }
        } else {

            /*
             *  ��ʱ���������Թ������½�
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  ��ǰ����������              */
                frontRightMethod();                                     /*  ���ҷ���                    */
                break;

            case RIGHT:                                                 /*  ��ǰ����������              */
                frontLeftMethod();                                      /*  ������                    */
                break;

            case DOWN:                                                  /*  ��ǰ����������              */
                leftMethod();                                           /*  ���ַ���                    */
                break;

            case LEFT:                                                  /*  ��ǰ����������              */
                rightMethod();                                          /*  ���ַ���                    */
                break;

            default:
                break;
            }
        }
    }
}
/*********************************************************************************************************
** Function name:       crosswayCheck
** Descriptions:        ͳ��ĳ������ڻ�δ�߹���֧·��
** input parameters:    ucX����Ҫ����ĺ�����
**                      ucY����Ҫ�����������
** output parameters:   ��
** Returned value:      ucCt��δ�߹���֧·��
*********************************************************************************************************/
uint8 crosswayCheck (int8  cX, int8  cY)
{
    uint8 ucCt = 0;
    if ((GucMapBlock[cX][cY] & 0x01) &&                                 /*  ���Է����Թ��Ϸ���·      */
        (GucMapBlock[cX][cY + 1]) == 0x00) {                            /*  ���Է����Թ��Ϸ�δ�߹�    */
        ucCt++;                                                         /*  ��ǰ����������1             */
    }
    if ((GucMapBlock[cX][cY] & 0x02) &&                                 /*  ���Է����Թ��ҷ���·      */
        (GucMapBlock[cX + 1][cY]) == 0x00) {                            /*  ���Է����Թ��ҷ�û���߹�  */
        ucCt++;                                                         /*  ��ǰ����������1             */
    }
    if ((GucMapBlock[cX][cY] & 0x04) &&                                 /*  ���Է����Թ��·���·      */
        (GucMapBlock[cX][cY - 1]) == 0x00) {                            /*  ���Է����Թ��·�δ�߹�    */
        ucCt++;                                                         /*  ��ǰ����������1             */
    }
    if ((GucMapBlock[cX][cY] & 0x08) &&                                 /*  ���Է����Թ�����·      */
        (GucMapBlock[cX - 1][cY]) == 0x00) {                            /*  ���Է����Թ���δ�߹�    */
        ucCt++;                                                         /*  ��ǰ����������1             */
    }
    return ucCt;
}
/*********************************************************************************************************
** Function name:       crosswayChoice
** Descriptions:        ѡ��һ��֧·��Ϊǰ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        ������ջ  ѡ��һ���뵱ǰ������ķ�֧��
** input parameters:    uint8 n   ջ�м�¼�ķ�֧�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void smartStack(uint8 n)
{
  if(n<1)//GmcCrossway[0]Ϊ��ʼ��  ����������
    return;
  uint8 record = 1;
  uint8 length = 0xff;
  
  for(uint8 i = 1;i<=n;i++)
  {
    if(GucMapStep[GmcCrossway[i].cX][GmcCrossway[i].cY]<length)//�ȸ�ֵС
    {
      record = i;
      length = GucMapStep[GmcCrossway[i].cX][GmcCrossway[i].cY];
    }
  }
  
  GmcCrossway[n+1].cX = GmcCrossway[record].cX;//��Сֵѹ��ջ��
  GmcCrossway[n+1].cY = GmcCrossway[record].cY;
  
  for(uint8 i = record;i<=n;i++)
  {
    GmcCrossway[i].cX = GmcCrossway[i+1].cX;//����˳��
    GmcCrossway[i].cY = GmcCrossway[i+1].cY;
  }
}
/*********************************************************************************************************
** Function name:       patchmaze
** Descriptions:        �����ջ���Ƿ�����·
** input parameters:    uint8 cX uint8 cY  ��ջ������
** output parameters:   1 ��Ҫ�˻�   0  ������·  ����Ҫ�˻�
** Returned value:      ��
*********************************************************************************************************/
uint8 patchmaze(uint8 cX,uint8 cY)   //��ջ��  1 ��Ҫ�˻�   0  ������·  ����Ҫ�˻�
{
  uint8 flag1 = 0;
  uint8 flag2 = 0;
  uint8 flag3 = 0;
  uint8 flag4 = 0;
  uint8 tempX,tempY; 
  tempX=cX;
  tempY=cY;
  if((GucMapBlock[cX][cY] & 0x01) &&                             /*   �Ϸ���·                   */
            GucMapBlock[cX][cY + 1] == 0x00)                   //ǰ��û���߹�
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
        //�ж�ǰ���Ƿ��е���
        if(tempY<=7)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x04) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //ǰ��û· ��������·����·
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
  if((GucMapBlock[cX][cY] & 0x08) &&                             /*  ����·                    */
            GucMapBlock[cX-1][cY] == 0x00)                  //ǰ��û���߹�
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
        //�ж�ǰ���Ƿ��е���
        if(tempX >= 0)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x02) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //ǰ��û· ��������·����·
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
 if((GucMapBlock[cX][cY] & 0x04) &&                             /*  �·���·                    */
            GucMapBlock[cX][cY-1] == 0x00)                  //ǰ��û���߹�
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
        //�ж�ǰ���Ƿ��е���
        if(tempY>=0)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x01) && GucMapBlock[tempX][tempY]!=0x00)
          {
            //ǰ��û· ��������·����·
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
 if((GucMapBlock[cX][cY] & 0x02) &&                             /*  �ҷ���·                   */
            GucMapBlock[cX+1][cY] == 0x00)                  //ǰ��û���߹�
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
        //�ж�ǰ���Ƿ��е���
        if(tempX<=15)
        {
          if(!(GucMapBlock[tempX][tempY] & 0x08) && GucMapBlock[tempX][tempY]!=0x00 )
          {
            //ǰ��û· ��������·����·
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
** Descriptions:        �жϵ�ǰ������ʱ���е���
** input parameters:    uint8 cX uint8 cY  ��ǰ������  dir  ���Է���
** output parameters:   ����1 û�е��� ����0 �е���
** Returned value:      ��
*********************************************************************************************************/
uint8 return_direct_info(uint8 cX,uint8 cY,uint8 dir)//��8��4��2��1   ����1 û�е��� ����0 �е���
{
  if(cX== 7 && cY == 7)     //��ǰ�����յ�  ����Ҫ��
    return 1;
  if(cX== 8 && cY == 7)    //��ǰ�����յ�  ����Ҫ��
    return 1;
  
  switch(dir){
  case 1:   //�Ϸ���·
    if(cX==0 && !(GucMapBlock[cX+1][cY] & 0x08) && GucMapBlock[cX+1][cY]!=0x00)    //�߽������ж�
      return 0;
    if((cX > 0) && (cX<15) && !(GucMapBlock[cX-1][cY] & 0x02) &&!(GucMapBlock[cX+1][cY] & 0x08)  
       && GucMapBlock[cX-1][cY]!=0x00 && GucMapBlock[cX+1][cY]!=0x00)//���ݵ�ǰ�������������ж������Ƿ��е���
      return 0;
    if((cX == 15) && !(GucMapBlock[cX-1][cY] & 0x02) && GucMapBlock[cX-1][cY]!=0x00)  //�߽������ж�
      return 0;
    
    return 1;
  
  case 2:
    if(cY==0 && !(GucMapBlock[cX][cY+1] & 0x04) && GucMapBlock[cX][cY+1]!=0x00 )    //�߽������ж�
      return 0;
    if((cY > 0) && (cY < 7) && !(GucMapBlock[cX][cY-1] & 0x01) &&!(GucMapBlock[cX][cY+1] & 0x04)   
       && GucMapBlock[cX][cY+1]!=0x00 && GucMapBlock[cX][cY-1]!=0x00) //���ݵ�ǰ�������������ж������Ƿ��е��� 
      return 0;
    if((cY == 7) && !(GucMapBlock[cX][cY-1] & 0x01) && GucMapBlock[cX][cY-1]!=0x00)  //�߽������ж�
      return 0;   
    return 1;

  case 4:
    if(cX==0 && !(GucMapBlock[cX+1][cY] & 0x08) && GucMapBlock[cX+1][cY]!=0x00)    //�߽������ж�
      return 0;
    if((cX > 0) && (cX<15) && !(GucMapBlock[cX-1][cY] & 0x02) &&!(GucMapBlock[cX+1][cY] & 0x08) 
       && GucMapBlock[cX+1][cY]!=0x00 && GucMapBlock[cX-1][cY]!=0x00)//���ݵ�ǰ�������������ж������Ƿ��е��� 
      return 0;
    if((cX == 15) && !(GucMapBlock[cX-1][cY] & 0x02) && GucMapBlock[cX-1][cY]!=0x00) //�߽������ж�
      return 0;
    
    return 1;
      
   case 8:
    if(cY==0 && !(GucMapBlock[cX][cY+1] & 0x04) && GucMapBlock[cX][cY+1]!=0x00)    //�߽������ж�
      return 0;
    if((cY > 0) && (cY < 7) && !(GucMapBlock[cX][cY-1] & 0x01) &&!(GucMapBlock[cX][cY+1] & 0x04) 
       && GucMapBlock[cX][cY+1]!=0x00 && GucMapBlock[cX][cY-1]!=0x00)//���ݵ�ǰ�������������ж������Ƿ��е��� 
      return 0;
    if((cY == 7) && !(GucMapBlock[cX][cY-1] & 0x01) && GucMapBlock[cX][cY-1]!=0x00)  //�߽������ж�
      return 0;
    
    return 1;   
  }
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
main (void)
{
    uint8 n          = 0;                                               /*  GmcCrossway[]�±�           */
    uint8 ucRoadStat = 0;                                               /*  ͳ��ĳһ�����ǰ����֧·��  */
    uint8 ucTemp     = 0;                                               /*  ����START״̬������ת��     */

    mouseInit();                                                        /*  �ײ������ĳ�ʼ��            */
    zlg7289Init();                                                      /*  ��ʾģ���ʼ��              */

    while (1) {
        switch (GucMouseTask) {                                         /*  ״̬������                  */
            
        case WAIT:
            sensorDebug();
            voltageDetect();
            delay(100000);
            if (keyCheck() == true) {                                   /*  ��ⰴ���ȴ�����            */
                zlg7289Reset();                                         /*  ��λZLG7289                 */
                GucMouseTask = START;
            }
            break;
            
        case START:                                                     /*  �жϵ��������ĺ�����      */
            mazeSearch();                                               /*  ��ǰ����                    */
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x08) {         /*  �жϵ���������Ƿ���ڳ���  */
                if (MAZETYPE == 8) {                                    /*  �޸��ķ�֮һ�Թ����յ�����  */
                    GucXGoal0 = 1;
                    GucXGoal1 = 0;
                }
                GucXStart   = MAZETYPE - 1;                             /*  �޸ĵ��������ĺ�����      */
                GmcMouse.cX = MAZETYPE - 1;                             /*  �޸ĵ�����ǰλ�õĺ�����  */    
                /*
                 *  ����Ĭ�ϵ����Ϊ(0,0)��������Ҫ���Ѽ�¼��ǽ������ת������
                 */
                ucTemp = GmcMouse.cY;
                do {
                    GucMapBlock[MAZETYPE - 1][ucTemp] = GucMapBlock[0][ucTemp];
                    GucMapBlock[0 ][ucTemp] = 0;
                }while (ucTemp--);
                /*
                 *  ��OFFSHOOT[0]�б����������
                 */
                GmcCrossway[n].cX = MAZETYPE - 1;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  ״̬ת��Ϊ��Ѱ״̬          */
            }
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x02) {         /*  �жϵ������ұ��Ƿ���ڳ���  */
                /*
                 *  ��OFFSHOOT[0]�б����������
                 */
                GmcCrossway[n].cX = 0;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  ״̬ת��Ϊ��Ѱ״̬          */
            }
            break;
            
        case MAZESEARCH:
          
          //�����󲿷�����
          
            if (GucMapBlock[GucXGoal0][GucYGoal0] != 0){
             weight_mapStepEdit(GucXGoal0,GucYGoal0);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  �����󽫿�ʼ���״̬        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal0][GucYGoal1] != 0){
             weight_mapStepEdit(GucXGoal0,GucYGoal1);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  �����󽫿�ʼ���״̬        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal1][GucYGoal0] != 0){
             weight_mapStepEdit(GucXGoal1,GucYGoal0);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  �����󽫿�ʼ���״̬        */
               break;
             }
           }
           if (GucMapBlock[GucXGoal1][GucYGoal1] != 0){
             weight_mapStepEdit(GucXGoal1,GucYGoal1);
             if (GucMapStep[GucXStart][GucYStart] < LIMIT){
               objectGoTo(GucXStart, GucYStart);
               mouseTurnback();
               GucMouseTask = SPURT;                               /*  �����󽫿�ʼ���״̬        */
               break;
             }
           }
            ucRoadStat = crosswayCheck(GmcMouse.cX,GmcMouse.cY);        /*  ͳ�ƿ�ǰ����֧·��          */
            if (ucRoadStat) {                                           /*  �п�ǰ������                */
                if (ucRoadStat > 1) {                                   /*  �ж�����ǰ�����򣬱�������  */
                    GmcCrossway[n].cX = GmcMouse.cX;
                    GmcCrossway[n].cY = GmcMouse.cY;
                    n++;
                }
                crosswayChoice();                                       /*  �����ַ�������ѡ��ǰ������  */
                mazeSearch();                                           /*  ǰ��һ��                    */
            } else {                                                    /*  û�п�ǰ�����򣬻ص����֧·*/
              
                weight_mapStepEdit(GmcMouse.cX,GmcMouse.cY);            //�Ե�ǰ������Ȩ�ȸ�ͼ   ������ջֻ��Ҫ��һ�μ�Ȩ�ȸ�ͼ
                
                while (--n) {
                  
                  
                  smartStack(n);
                  
                   ucRoadStat = crosswayCheck(GmcCrossway[n].cX,
                                               GmcCrossway[n].cY);
                                                                        /*  ͳ�����֧��δ�߹��ķ�����  */
                                                                        
                    /*
                     *  ������δ�߹���·����ǰ����֧�㣬������ѭ��
                     *  ����������һ���δ�߹���֧·��
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
                if (n == 0) {                                           /*  ���������е�֧·���ص����  */
                    objectGoTo(GmcCrossway[0].cX, GmcCrossway[0].cY);
                    mouseTurnback();
                    GucMouseTask = SPURT;                               /*  �����󽫿�ʼ���״̬        */
                }
            }
            break;

        case SPURT:
            mouseSpurt();                                               /*  ������·�������յ�          */
            objectGoTo(GucXStart,GucYStart);                            /*  �ص����                    */
            mouseTurnback();                                            /*  ���ת���ָ���������        */
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
