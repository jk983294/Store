/*-------------------------------------------
|              CText.cpp
|       CText��ĳ�Ա��Դ����
|           (c)  jk&ljq, 2010
---------------------------------------------*/

#include <stdio.h>
#include <ddraw.h>
#include"CText.h"

/****************************************
*  ������:  CText(...)
*    ����:  ���캯��
*           (c)  jk&ljq, 2010
*****************************************/
CText::CText( void )
{
    pText = NULL;
    lpDDSBack = NULL;
}
/****************************************
*  ������: ~CText(...)
*    ����: ��������
*           (c)  jk&ljq, 2010
*****************************************/
CText::~CText()
{
    DeleteText();
}
/****************************************
*  ������: DeleteText(...)
*    ����: �ͷ��ַ�����ռ�
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
*  ������:  GetText(...)
*    ����:  ���Ҫ��ʾ���ַ��������ö�
*           Ӧ�ַ�����������
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,LPCSTR lpText,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    PTEXTDESC pTemp = NULL;

    //ѡ���ַ���������һ����Ա
    pTemp = pText;
    while( 1 )
    {
        //����ǿ�
        if( pTemp!=NULL ) {
            if( pTemp->next==NULL )
                break;
            pTemp = pTemp->next;
        }
        //�����ǿյ�
        else {
            pText = new TEXTDESC;
            pText->next = NULL;
            //�������ַ�����������
            pText->lpSurf = lpSurf;
            pText->color  = color;
            pText->Pos.x  = x;
            pText->Pos.y  = y;
            strcpy(pText->Text,lpText);
            pText->StartTime = GetTickCount();
            pText->LastTime  = LastTime;
			pText->Size = Size;//Ĭ������
            return;
        }
    }

    //Ϊ�µ��ַ��������ٿռ�
    pTemp->next = new TEXTDESC;
    pTemp = pTemp->next;
    pTemp->next = NULL;

    //�������ַ�����������
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
*  ������:  GetText(...)
*    ����:  ���Ҫ��ʾ�����������ö�
*           Ӧ�ַ�����������
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,LONG lNumber,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    TCHAR szBuffer[MAX_TEXT];
    //������ת��Ϊ�ַ���
    sprintf( szBuffer, TEXT("%d"), lNumber );
    GetText(lpSurf,x,y,szBuffer,color,LastTime,Size);
}
/****************************************
*  ������:  GetText(...)
*    ����:  ���Ҫ��ʾ�ĸ����������ö�
*           Ӧ�ַ�����������
*           (c)  jk&ljq, 2010
*****************************************/
void CText::GetText(LPDIRECTDRAWSURFACE7 lpSurf,
                    LONG x,LONG y,FLOAT fNumber,
                    DWORD color,
                    DWORD LastTime,LONG Size)
{
    TCHAR szBuffer[MAX_TEXT];
    //��������ת��Ϊ�ַ���
    sprintf( szBuffer, TEXT("%f"), fNumber );
    GetText(lpSurf,x,y,szBuffer,color,LastTime,Size);
}

/****************************************
*  ������:  TextBackSurf(...)
*    ����:  ָ��Ҫ�ں�̨�����������ʾ���ַ���
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
*  ������:  TextBackSurf(...)
*    ����:  ָ��Ҫ�ں�̨�����������ʾ������
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
*  ������:  TextBackSurf(...)
*    ����:  ָ��Ҫ�ں�̨�����������ʾ�ĸ�����
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
*  ������: ReMain(...)
*    ����: ��ʾ�ַ��������е��ַ���
*           (c)  jk&ljq, 2010
*****************************************/
void CText::ReMain( void )
{
    Time = GetTickCount();
    PTEXTDESC pTemp = pText,oldpTemp=NULL;
    HDC hdc;
    while( pTemp!=NULL )
    {
        //��ʾ��ʱ�仹δ����ָ���ĳ���
        if( (Time-pTemp->StartTime)<pTemp->LastTime
            ||pTemp->LastTime==0 )
        {
            //��Զ��ʾ���ַ���
            if( pTemp->LastTime==0 )
                pTemp->LastTime=1;
            //����豸����
            pTemp->lpSurf->GetDC(&hdc);
            //͸��ģʽ
            SetBkMode(hdc,TRANSPARENT);
            //�����ַ���ɫ
            SetTextColor(hdc,pTemp->color); 
            //���ϵͳ�ṩ������
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
           FF_MODERN,"����");
		   SelectObject(hdc,hFont);
			}
		   //����ַ���
            TextOut(
                hdc,
                pTemp->Pos.x,
                pTemp->Pos.y,
                pTemp->Text,
                strlen(pTemp->Text));
            //�ͷ��豸����
            pTemp->lpSurf->ReleaseDC(hdc);
        }
        //��ʾ��ʱ���Ѿ��ﵽָ���ĳ���
        else {
            //ɾ���ַ�����
            PTEXTDESC next = pTemp->next;
            delete pTemp;
            //��������ĵ�һ��
            if( oldpTemp!=NULL ) {
                oldpTemp->next = next;
                pTemp = oldpTemp;
            }
            //������ĵ�һ��,�����¿�ʼ
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
		   /*FW_DONTCARE��0��FW_THIN��100��FW_EXTRALIGHT��200��FW_ULTRALIGHT��200��FW_LIGHT��300��
����FW_NORMAL��400��FW_REGULAR��400��FW_MEDIUM��500��FW_SEMIBOLD��600��FW_DEMIBOLD��600��
����FW_BOLD��700��FW_EXTRABOLD��800��FW_ULTRABOLD��800��FW_HEAVY��900��FW_BLACK��900��*/
          