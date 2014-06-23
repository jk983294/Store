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
	//��ʼ��DDraw,���Ҵ���������ͷ�ת�������
    BOOL InitDDraw(HWND hWd,
                   BOOL bFScr,
                   DWORD ScrW,DWORD ScrH,
                   DWORD BitCount );
	//��BitmapFileָ����λͼ��ʽͼ������ո�Surfָ��ı���
    BOOL LoadBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
            			LPCSTR BitmapFile );
    //����Surfָ���ı���,�ǿձ���
    BOOL CreateNULLSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        DWORD MemoryFlag);
    //����lpSurfָ���ı���,���ҽ�BitmapFileָ����λͼ��ʽͼ������ոմ����ı���
    BOOL CreateBMPSurface(
        LPDIRECTDRAWSURFACE7 &lpSurf,
        LONG W,LONG H,
        LPCSTR BitmapFile,
        DWORD MemoryFlag);
    //��pRtSrcָ������Դ����ss�ϵ�ͼƬ���Ƶ�lpSurfDestָ����Ŀ�����
    HRESULT Blit(
        LPDIRECTDRAWSURFACE7 lpSurfDest,
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );
    //��ָ����Դ����ͼƬ���͵���̨����
    HRESULT BlitBack(
        LONG x,LONG y,
        LPDIRECTDRAWSURFACE7 lpSurfSrc,
        RECT * pRtSrc,
        DWORD dwTrans );
    //�����rgbָ����ɫֵ��Ӧpdds���������ֵ
    DWORD DDColorMatch(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);
    //������pdds����ָ���ı���Ĺؼ�ɫ
    HRESULT DDSetColorKey(
        LPDIRECTDRAWSURFACE7 pdds,
        COLORREF rgb);
    //����һ���ü���,����������������
    HRESULT UseClipper(void);
    //��ת����==>��ʾ
    void Flip( LPRECT pRectWin );
	//��ȡһ�����νṹ
    RECT GetRect(
        LONG left,LONG top,
        LONG right,LONG bottom);
    //�Ƿ�ȫ��
    BOOL Get_bFullScr( void )   { return bFullScr;}
	//�õ����ߡ���ɫ���
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

