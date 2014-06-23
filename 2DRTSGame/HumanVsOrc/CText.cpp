/*-------------------------------------------
|              CText.cpp
|       CText类的成员的源代码
|           (c)  jk&ljq, 2010
---------------------------------------------*/

#include <stdio.h>
#include <ddraw.h>
#include"CText.h"

/****************************************
*  函数名:  CText(...)
*    功能:  构造函数
*           (c)  jk&ljq, 2010
*****************************************/
CText::CText( void )
{
    pText = NULL;
    lpDDSBack = NULL;
}
/****************************************
*  函数名: ~CText(...)
*    功能: 析构函数
*           (c)  jk&ljq, 2010
*****************************************/
CText::~CText()
{
    DeleteText();
}
/****************************************
*  函数名: DeleteText(...)
*    功能: 释放字符链表空间
*           (c)  jk&ljq, 2010
*****************************************/
void CText::DeleteText( void )
{
    PTEXTDESC pTemp = pText,pTemp2=NULL;
    while( pTemp!=NULL )
    {
        pTemp2 = pTemp->next;
        delete pTemp;
        pTemp = pTemp2;
    }
}

/****************************************
*  函数名:  GetText(...)
*    功能:  获得要显示的字符串并设置对
*           应字符描述的属性
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,LPCSTR lpText,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    PTEXTDESC pTemp = NULL;

    //选择字符链表的最后一个成员
    pTemp = pText;
    while( 1 )
    {
        //链表非空
        if( pTemp!=NULL ) {
            if( pTemp->next==NULL )
                break;
            pTemp = pTemp->next;
        }
        //链表是空的
        else {
            pText = new TEXTDESC;
            pText->next = NULL;
            //设置新字符描述的属性
            pText->lpSurf = lpSurf;
            pText->color  = color;
            pText->Pos.x  = x;
            pText->Pos.y  = y;
            strcpy(pText->Text,lpText);
            pText->StartTime = GetTickCount();
            pText->LastTime  = LastTime;
			pText->Size = Size;//默认字体
            return;
        }
    }

    //为新的字符描述开辟空间
    pTemp->next = new TEXTDESC;
    pTemp = pTemp->next;
    pTemp->next = NULL;

    //设置新字符描述的属性
    pTemp->lpSurf = lpSurf;
    pTemp->color  = color;
    pTemp->Pos.x  = x;
    pTemp->Pos.y  = y;
    strcpy(pTemp->Text,lpText);
    pTemp->StartTime = GetTickCount();
    pTemp->LastTime  = LastTime;
	pTemp->Size  = Size;
}

/****************************************
*  函数名:  GetText(...)
*    功能:  获得要显示的整数并设置对
*           应字符描述的属性
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,LONG lNumber,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    TCHAR szBuffer[MAX_TEXT];
    //将整数转化为字符串
    sprintf( szBuffer, TEXT("%d"), lNumber );
    GetText(lpSurf,x,y,szBuffer,color,LastTime,Size);
}
/****************************************
*  函数名:  GetText(...)
*    功能:  获得要显示的浮点数并设置对
*           应字符描述的属性
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,FLOAT fNumber,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    TCHAR szBuffer[MAX_TEXT];
    //将浮点数转化为字符串
    sprintf( szBuffer, TEXT("%f"), fNumber );
    GetText(lpSurf,x,y,szBuffer,color,LastTime,Size);
}

/****************************************
*  函数名:  TextBackSurf(...)
*    功能:  指定要在后台缓冲表面中显示的字符串
*           (c)  jk&ljq, 2010
*****************************************/
void CText::TextBackSurf(LONG x,LONG y,
                                    LPCSTR lpText,
                                    DWORD color,
                                    DWORD LastTime,LONG Size)
{
    GetText(lpDDSBack,x,y,lpText,color,LastTime,Size);
}
/****************************************
*  函数名:  TextBackSurf(...)
*    功能:  指定要在后台缓冲表面中显示的整数
*           (c)  jk&ljq, 2010
*****************************************/
void CText::TextBackSurf(LONG x,LONG y,
                         LONG lNumber,
                         DWORD color,
                         DWORD LastTime,LONG Size)
{
    GetText(lpDDSBack,x,y,lNumber,color,LastTime,Size);
}
/****************************************
*  函数名:  TextBackSurf(...)
*    功能:  指定要在后台缓冲表面中显示的浮点数
*           (c)  jk&ljq, 2010
*****************************************/
void CText::TextBackSurf(LONG x,LONG y,
                         FLOAT fNumber,
                         DWORD color,
                         DWORD LastTime,LONG Size)
{
    GetText(lpDDSBack,x,y,fNumber,color,LastTime,Size);
}

/****************************************
*  函数名: ReMain(...)
*    功能: 显示字符串链表中的字符串
*           (c)  jk&ljq, 2010
*****************************************/
void CText::ReMain( void )
{
    Time = GetTickCount();
    PTEXTDESC pTemp = pText,oldpTemp=NULL;
    HDC hdc;
    while( pTemp!=NULL )
    {
        //显示的时间还未到达指定的长度
        if( (Time-pTemp->StartTime)<pTemp->LastTime
            ||pTemp->LastTime==0 )
        {
            //永远显示的字符串
            if( pTemp->LastTime==0 )
                pTemp->LastTime=1;
            //获得设备内容
            pTemp->lpSurf->GetDC(&hdc);
            //透明模式
            SetBkMode(hdc,TRANSPARENT);
            //设置字符颜色
            SetTextColor(hdc,pTemp->color); 
            //获得系统提供的字体
            if(pTemp->Size==0){
				SelectObject(
                hdc,
                GetStockObject(OEM_FIXED_FONT));}
			else
			{
			HFONT hFont;
           hFont=CreateFont(pTemp->Size,pTemp->Size,0,0,900,false,false,false,
           CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
           CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
           FF_MODERN,"宋体");
		   SelectObject(hdc,hFont);
			}
		   //输出字符串
            TextOut(
                hdc,
                pTemp->Pos.x,
                pTemp->Pos.y,
                pTemp->Text,
                strlen(pTemp->Text));
            //释放设备内容
            pTemp->lpSurf->ReleaseDC(hdc);
        }
        //显示的时间已经达到指定的长度
        else {
            //删除字符描述
            PTEXTDESC next = pTemp->next;
            delete pTemp;
            //不是链表的第一个
            if( oldpTemp!=NULL ) {
                oldpTemp->next = next;
                pTemp = oldpTemp;
            }
            //是链表的第一个,则重新开始
            else {
                pText = next;
                pTemp = pText;
                continue;
            }
        }
        oldpTemp = pTemp;
        pTemp = pTemp->next;
    }
}
   /*
		   /*FW_DONTCARE：0；FW_THIN；100；FW_EXTRALIGHT；200；FW_ULTRALIGHT；200；FW_LIGHT；300；
　　FW_NORMAL：400；FW_REGULAR；400；FW_MEDIUM；500；FW_SEMIBOLD；600；FW_DEMIBOLD；600；
　　FW_BOLD：700；FW_EXTRABOLD；800；FW_ULTRABOLD；800；FW_HEAVY；900；FW_BLACK；900。*/
          