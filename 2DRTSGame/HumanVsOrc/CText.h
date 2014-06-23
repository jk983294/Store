/*-------------------------------------------
|              CText.h
|      �ַ��������ع�������
|           ��Ȩ����  jk&ljq
---------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <ddraw.h>

#define MAX_TEXT 200

typedef struct TEXTDESC {
    LPDIRECTDRAWSURFACE7 lpSurf;//���ĸ�������ʾ
    POINT       Pos;//��ʲôλ����ʾ
    TCHAR       Text[MAX_TEXT];//ָ����ַ���
    DWORD       color;//�ַ�����ɫ
    DWORD       StartTime;//��ʼ��ʾ��ʱ��
    DWORD       LastTime;//��ʾ�೤ʱ��
	LONG        Size;
    TEXTDESC  * next;//����ڵ�
} TEXTDESC, * PTEXTDESC;

class	CText
{
protected:
    DWORD     Time;//��ǰʱ��
    PTEXTDESC pText;//�ַ�������
    LPDIRECTDRAWSURFACE7 lpDDSBack;//��̨�������

public:
    //��ú�̨�������
    void Set_lpDDSBack(
        LPDIRECTDRAWSURFACE7 lpBackSurf )
    {
        lpDDSBack = lpBackSurf;
    }
    //���캯��
    CText();
    //��������
    ~CText();
    //ɾ���ַ�������
    void DeleteText( void );
    //���Ҫ��ʾ���ַ��������ö�Ӧ�ַ�����������
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        LPCSTR lpText,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //���Ҫ��ʾ�����������ö�Ӧ�ַ�����������
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        LONG lNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //���Ҫ��ʾ�ĸ����������ö�Ӧ�ַ�����������
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        FLOAT fNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //ָ��Ҫ�ں�̨�����������ʾ���ַ���
    void TextBackSurf(
        LONG x,LONG y,
        LPCSTR lpText,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //ָ��Ҫ�ں�̨�����������ʾ������
    void TextBackSurf(
        LONG x,LONG y,
        LONG lNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //ָ��Ҫ�ں�̨�����������ʾ�ĸ�����
    void TextBackSurf(
        LONG x,LONG y,
        FLOAT fNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //��ʾ�ַ��������е��ַ���
    void ReMain( void );
};

