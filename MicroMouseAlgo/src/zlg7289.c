/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Zlg7289.h
** Last modified Date:      2008/01/03
** Last Version:            V1.00
** Description:             �������ʾ����̹���оƬZLG7289 ���MicroMouse615����������
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              ��ï��
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


#include "Zlg7289.h"


/*********************************************************************************************************
** Function name:       __delayNuS
** Descriptions:        ��ʱN��΢��
** input parameters:    iTime: ��ʱʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __delayNuS (int32 iTime)
{
    iTime = SysCtlClockGet() * iTime / 2000000;                         /*  ����ϵͳʱ������ȷ����ʱ    */
    while (--iTime != 0);
}

/*********************************************************************************************************
** Function name:       __zlg7289SPIWrite
** Descriptions:        ��SPI ����д��1 ���ֽڵ����ݡ�
** input parameters:    cDat��Ҫд�������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __zlg7289SPIWrite (int8 cDat)
{
    int8 cT = 8;
    GPIODirModeSet(GPIO_PORTA_BASE, ZLG7289_DIO, GPIO_DIR_MODE_OUT);    /*  ����DIO�˿�Ϊ���ģʽ       */
    /*
     *  ѭ��дһ���ֽڵ�����
     */
    do {
        if((cDat & 0x80) == 0x80) {
            GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_DIO, 0xff);
        } else {
            GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_DIO, 0x00);
        }
        cDat <<= 1;
        GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CLK, 0xff);
        __delayNuS(5);
        GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CLK, 0x00);
        __delayNuS(5);
    } while (--cT != 0);
}


/*********************************************************************************************************
** Function name:       __zlg7289SPIRead
** Descriptions:        ��SPI ���߶�ȡ1 ���ֽڵ����ݡ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ȡ��������
*********************************************************************************************************/
int8 __zlg7289SPIRead (void)
{
    int8 cDat = 0;
    int8 cT   = 8;
    GPIODirModeSet(GPIO_PORTA_BASE, ZLG7289_DIO, GPIO_DIR_MODE_IN);     /* ����DIO�˿�Ϊ���ģʽ        */
    /*
     *  ѭ����һ���ֽڵ�����
     */
    do {
        GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CLK, 0xff);
        __delayNuS(5);
        cDat <<= 1;
        if (GPIOPinRead(GPIO_PORTA_BASE, ZLG7289_DIO)) {
            cDat++;
        }
        GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CLK, 0x00);
        __delayNuS(5);
    } while (--cT != 0);
    return cDat;
}


/*********************************************************************************************************
** Function name:       zlg7289Cmd
** Descriptions:        ִ��ZLG7289 ��ָ�
** input parameters:    cCmd��������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void zlg7289Cmd (int8  cCmd)
{
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0x00);
    __delayNuS(25);
    __zlg7289SPIWrite(cCmd);
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0xff);
    __delayNuS(5);
}


/*********************************************************************************************************
** Function name:       zlg7289CmdDat
** Descriptions:        ִ��ZLG7289 ������ָ�
** input parameters:    cCmd��������
**                      cDat������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void zlg7289CmdDat (uint8  cCmd, int8  cDat)
{
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0x00);
    __delayNuS(25);
    __zlg7289SPIWrite(cCmd);
    __delayNuS(15);
    __zlg7289SPIWrite(cDat);
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0xff);
    __delayNuS(5);
}


/*********************************************************************************************************
** Function name:       zlg7289Download
** Descriptions:        �������ݡ�
** input parameters:    ucMod=0�� ���������Ұ���ʽ0 ����
**                      ucMod=1�� ���������Ұ���ʽ1 ����
**                      ucMod=2�� �������ݵ�������
**                      cX��      ����ܱ�ţ������꣩��ȡֵ0��7
**                      cDp=0��   С���㲻��
**                      cDp=1��   С������
**                      cDat��    Ҫ��ʾ������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void zlg7289Download (uint8  ucMod, int8  cX, int8  cDp, int8  cDat)
{
    uint8 ucModDat[3] = {0x80,0xC8,0x90};
    uint8 ucD1;
    uint8 ucD2;
    
    if (ucMod > 2) {
        ucMod = 2;
    }
    
    ucD1  = ucModDat[ucMod];
    cX   &= 0x07;
    ucD1 |= cX;
    ucD2  = cDat & 0x7F;
    
    if (cDp  == 1) {
        ucD2 |= 0x80;
    }
    zlg7289CmdDat(ucD1, ucD2);
}


/*********************************************************************************************************
** Function name:       zlg7289Key
** Descriptions:        ִ��ZLG7289 �������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ���ض����İ���ֵ��0��63���������0xFF ���ʾû�м�����
*********************************************************************************************************/
int8 zlg7289Key (void)
{
    int8 cKey;
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0x00);
    __delayNuS(25);
    __zlg7289SPIWrite(0x15);
    __delayNuS(15);
    cKey = __zlg7289SPIRead();
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS, 0xff);
    __delayNuS(5);
    return cKey;
}


/*********************************************************************************************************
** Function name:       ZLG7289_Init
** Descriptions:        ZLG7289 ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void zlg7289Init (void)
{
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOA );                      /*  ʹ��GPIO A������            */
    
    GPIODirModeSet(GPIO_PORTA_BASE,
                     ZLG7289_CS
                    |ZLG7289_CLK
                    |ZLG7289_DIO,
                     GPIO_DIR_MODE_OUT);                                /*  ����I/O��Ϊ���ģʽ         */
    
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_DIO, 0xff);
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CLK, 0x00);
    GPIOPinWrite(GPIO_PORTA_BASE, ZLG7289_CS , 0xff);
    
    zlg7289Reset();                                                     /*  ��λZLG7289                 */
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
