/*-------------------------------------------
|              CDDraw.h
|     DirectDraw环境的相关处理功能
|           版权所有  jk&ljq
---------------------------------------------*/
#include <windows.h>
#include <ddraw.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")

class	CDDraw
{
   	//变量的声明
protected:
    BOOL	bFullScr;//bFullScr值是1表示全屏模式;值为0表示窗口模式
    DWORD   ScreenW;//主表面的宽
    DWORD   ScreenH;//主表面的高
    DWORD   ScreenC;//颜色深度
    HWND	hWnd;

    LPDIRECTDRAW7         lpDD;//DirectDraw对象
    LPDIRECTDRAWSURFACE7  lpDDSPrimary;//主表面
    LPDIRECTDRAWSURFACE7  lpDDSBack;//翻转缓冲面

    void    CheckDisplayMode( void );

public:

    CDDraw();
    ~CDDraw();

    //功能函数的声明
    BOOL InitDDraw(HWND hWd,
                   BOOL bFScr,
                   DWORD ScrW,DWORD ScrH,
                   DWORD BitCount );
    BOOL LoadBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
            			LPCSTR BitmapFile );
    BOOL CreateNULLSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        DWORD MemoryFlag);

    BOOL CreateBMPSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        LPCSTR BitmapFile,
        DWORD MemoryFlag);

    HRESULT Blit(
        LPDIRECTDRAWSURFACE7 lpSurfDest,
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );

    HRESULT BlitBack(
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );

    DWORD DDColorMatch(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);

    HRESULT DDSetColorKey(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);

    HRESULT UseClipper(void);
    void Flip( LPRECT pRectWin );
    RECT GetRect(
        LONG left,LONG top,
        LONG right,LONG bottom);

    BOOL Get_bFullScr( void )   { return bFullScr;}
    DWORD Get_ScreenW( void )   { return ScreenW;}
    DWORD Get_ScreenH( void )   { return ScreenH;}
    DWORD Get_ScreenC( void )   { return ScreenC;}
    LPDIRECTDRAW7 Get_lpDD(void){ return lpDD;}
   	LPDIRECTDRAWSURFACE7 Get_lpDDSPrimary(void)
    {
        return lpDDSPrimary;
    }
    LPDIRECTDRAWSURFACE7  Get_lpDDSBack(void)
    {
        return lpDDSBack;
    }
};

