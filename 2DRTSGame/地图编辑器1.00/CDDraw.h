/*-------------------------------------------
|              CDDraw.h
|     DirectDraw��������ش�����
|           ��Ȩ����  jk&ljq
---------------------------------------------*/
#include <windows.h>
#include <ddraw.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")

class	CDDraw
{
   	//����������
protected:
    BOOL	bFullScr;//bFullScrֵ��1��ʾȫ��ģʽ;ֵΪ0��ʾ����ģʽ
    DWORD   ScreenW;//������Ŀ�
    DWORD   ScreenH;//������ĸ�
    DWORD   ScreenC;//��ɫ���
    HWND	hWnd;

    LPDIRECTDRAW7         lpDD;//DirectDraw����
    LPDIRECTDRAWSURFACE7  lpDDSPrimary;//������
    LPDIRECTDRAWSURFACE7  lpDDSBack;//��ת������

    void    CheckDisplayMode( void );

public:

    CDDraw();
    ~CDDraw();

    //���ܺ���������
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

