/*-------------------------------------------
|              CText.h
|      字符串输出相关功能声明
|           版权所有  jk&ljq
---------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <ddraw.h>

#define MAX_TEXT 200

typedef struct TEXTDESC {
    LPDIRECTDRAWSURFACE7 lpSurf;//在哪个表面显示
    POINT       Pos;//在什么位置显示
    TCHAR       Text[MAX_TEXT];//指向的字符串
    DWORD       color;//字符的颜色
    DWORD       StartTime;//开始显示的时间
    DWORD       LastTime;//显示多长时间
	LONG        Size;
    TEXTDESC  * next;//链表节点
} TEXTDESC, * PTEXTDESC;

class	CText
{
protected:
    DWORD     Time;//当前时间
    PTEXTDESC pText;//字符串链表
    LPDIRECTDRAWSURFACE7 lpDDSBack;//后台缓冲表面

public:
    //获得后台缓冲表面
    void Set_lpDDSBack(
        LPDIRECTDRAWSURFACE7 lpBackSurf )
    {
        lpDDSBack = lpBackSurf;
    }
    //构造函数
    CText();
    //析构函数
    ~CText();
    //删除字符串链表
    void DeleteText( void );
    //获得要显示的字符串并设置对应字符描述的属性
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        LPCSTR lpText,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //获得要显示的整数并设置对应字符描述的属性
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        LONG lNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //获得要显示的浮点数并设置对应字符描述的属性
    void GetText(
        LPDIRECTDRAWSURFACE7 lpSurf,
        LONG x,LONG y,
        FLOAT fNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //指定要在后台缓冲表面中显示的字符串
    void TextBackSurf(
        LONG x,LONG y,
        LPCSTR lpText,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //指定要在后台缓冲表面中显示的整数
    void TextBackSurf(
        LONG x,LONG y,
        LONG lNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //指定要在后台缓冲表面中显示的浮点数
    void TextBackSurf(
        LONG x,LONG y,
        FLOAT fNumber,
        DWORD color,
        DWORD LastTime,
		LONG Size);
    //显示字符串链表中的字符串
    void ReMain( void );
};

