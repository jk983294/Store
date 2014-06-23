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
	//初始化DDraw,并且创建主表面和翻转缓冲表面
    BOOL InitDDraw(HWND hWd,
                   BOOL bFScr,
                   DWORD ScrW,DWORD ScrH,
                   DWORD BitCount );
	//将BitmapFile指定的位图格式图像载入刚刚Surf指向的表面
    BOOL LoadBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
            			LPCSTR BitmapFile );
    //创建Surf指定的表面,是空表面
    BOOL CreateNULLSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        DWORD MemoryFlag);
    //创建lpSurf指定的表面,并且将BitmapFile指定的位图格式图像载入刚刚创建的表面
    BOOL CreateBMPSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        LPCSTR BitmapFile,
        DWORD MemoryFlag);
    //将pRtSrc指定的在源表面ss上的图片复制到lpSurfDest指定的目标表面
    HRESULT Blit(
        LPDIRECTDRAWSURFACE7 lpSurfDest,
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );
    //将指定的源表面图片传送到后台表面
    HRESULT BlitBack(
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );
    //获得由rgb指定颜色值对应pdds表面的索引值
    DWORD DDColorMatch(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);
    //设置由pdds参数指定的表面的关键色
    HRESULT DDSetColorKey(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);
    //创建一个裁剪器,并将它附于主表面
    HRESULT UseClipper(void);
    //翻转表面==>显示
    void Flip( LPRECT pRectWin );
	//获取一个矩形结构
    RECT GetRect(
        LONG left,LONG top,
        LONG right,LONG bottom);
    //是否全屏
    BOOL Get_bFullScr( void )   { return bFullScr;}
	//得到宽、高、颜色深度
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

