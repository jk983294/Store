/*-------------------------------------------
|           CDDraw.cpp
|       CDDraw��ĳ�Ա��Դ����
|           ��Ȩ����  jk&ljq, 2010
---------------------------------------------*/
#include"CDDraw.h"

typedef struct _DISPLAYMODE {
    DDSURFACEDESC2 * pddsdMode;
    DWORD dwNumMode;
}DISPLAYMODE;

DISPLAYMODE dmModes;

static HRESULT WINAPI ModeEnumRecord( DDSURFACEDESC2 * pNewMode,
                                      VOID * pRecordInfo );
static BOOL WINAPI DDisplayEnumerate( GUID * pGUID, 
                                LPSTR ,
                                LPSTR ,
                                LPVOID , HMONITOR );
/****************************************
*  ������:  CDDraw(...)
*  ����:  ���캯��
*           ��Ȩ����  jk&ljq, 2010
*****************************************/
CDDraw::CDDraw()
{
	//��ʼ������
	hWnd = NULL;
	bFullScr = TRUE;
	ScreenW = 640;
	ScreenH = 480;
	ScreenC = 16;
	lpDD = NULL;
	lpDDSPrimary = NULL;
	lpDDSBack = NULL;
}

/****************************************
*  ������:  ~CDDraw()
*  ����:  ��������
*           ��Ȩ����  jk&ljq, 2010
*****************************************/
CDDraw::~CDDraw()
{
	if(lpDD!=NULL){
		if(lpDDSBack!=NULL){
			lpDDSBack->Release();
			lpDDSBack = NULL;
		}
		if(lpDDSPrimary!=NULL){
			lpDDSPrimary->Release();
			lpDDSPrimary = NULL;
		}
		lpDD->Release();
		lpDD = NULL;
	}
}

/**************************************************
*  ������:  InitDDraw(...)
*  ����:  ��ʼ��DDraw,���Ҵ���������ͷ�ת�������
*           ��Ȩ����  jk&ljq, 2010
***************************************************/
BOOL CDDraw::InitDDraw(HWND hWd,
					   BOOL bFScr,
					   DWORD ScrW,DWORD ScrH,
					   DWORD BitCount )
{
	//--------------------------��������������ʼ������
	hWnd=hWd;
	bFullScr=bFScr;
	if(bFScr){//ȫ��ģʽ
		if(ScrW <= 640)		{ ScreenW = 640; ScreenH = 480; }
		else if(ScrW <= 800){ ScreenW = 800; ScreenH = 600; }
		else if(ScrW > 800) { ScreenW = 1024; ScreenH = 768; }
		else {ScreenW=800; ScreenH = 600;}
		if(BitCount<=8)		ScreenC = 8;
		else if(BitCount<=16)ScreenC = 16;
		else if(BitCount<=24)ScreenC = 24;
		else if(BitCount<=32)ScreenC = 32;
		else ScreenC=16;

        //���ģʽ�Ƿ���Ч
        CheckDisplayMode();
	}
	else{//����ģʽ
		ScreenW=ScrW;
		ScreenH=ScrH;
	}

	//--------------------------��ʼ��DirectDraw����
	HRESULT ddrval;
	//����DirectDraw����
	ddrval = DirectDrawCreateEx(NULL,(void **) &lpDD,IID_IDirectDraw7,NULL);
	if( FAILED(ddrval) )	{
		//����DirectDraw�������ʱ����������
		//��������Լ�¼����DirectDraw����ĳ�����Ϣ
		return FALSE;
	}
	//******ȫ��ģʽ***********//
	if( bFullScr ){
		//����Э���ȼ�
		ddrval = lpDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT );
		if( FAILED( ddrval )){
			//����Э���ȼ�����ʱ����������
			//��������Լ�¼����Э���ȼ��ĳ�����Ϣ
			return FALSE;
		}

		//������ʾģʽ
		ddrval = lpDD->SetDisplayMode( ScreenW,ScreenH,ScreenC,NULL,NULL);
		if( FAILED( ddrval ) ){
			//������ʾģʽ����ʱ����������
			//��������Լ�¼������ʾģʽ�ĳ�����Ϣ
			return FALSE;
		}

		//����������
	    DDSURFACEDESC2 ddsd;
	    ZeroMemory(&ddsd,sizeof(ddsd));
	    ddsd.dwSize = sizeof( ddsd );
	    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							  DDSCAPS_FLIP |
							  DDSCAPS_COMPLEX;
	    ddsd.dwBackBufferCount = 1;
		ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
		if (ddrval!=DD_OK) {
			//�������������ʱ����������
			//��������Լ�¼����������ĳ�����Ϣ
			return FALSE;
		}

		//������̨�������
		DDSCAPS2 ddscaps;
		ZeroMemory(&ddscaps,sizeof(ddscaps));
		ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
		ddrval=lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
		if (ddrval!=DD_OK) {
			//��ȡ��̨����������ʱ����������
			//��������Լ�¼��̨�������ĳ�����Ϣ
			return FALSE;
		}
		return TRUE;
	}
	//******����ģʽ***********//
	else{
		//����Э���ȼ�
		ddrval=(HRESULT)lpDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );

		//����������
		DDSURFACEDESC2 	ddsd;
		ZeroMemory( &ddsd, sizeof(ddsd) );
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	    //���������棬��ȡ��ָ���������ָ��lpDDSPrimary
		ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
		if( FAILED( ddrval )){
			//�������������ʱ����������
			//��������Լ�¼����������ĳ�����Ϣ
			return FALSE;
		}

        //ʹ�òü���
        UseClipper();

		//������̨�������
		//:::::::ddsdǰ���Ѿ�����
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize=sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth  = ScreenW;//��ķֱ���
		ddsd.dwHeight = ScreenH;//�ߵķֱ���
		//����һ��̨�������
		ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
		if( FAILED( ddrval )){
			//������̨����������ʱ����������
			//��������Լ�¼������̨�������ĳ�����Ϣ
			return FALSE;
		}
	}

    return TRUE;
	//--------------------------��ʼ��DirectDraw�������
}

/**************************************************
*  ������:  Flip(...)
*  ����:  ��ת����==>��ʾ
*           ��Ȩ����  jk&ljq, 2010
***************************************************/
void CDDraw::Flip( LPRECT pRectWin )
{
    HRESULT   ddrval=1;

    if( lpDDSPrimary && lpDDSBack )
    {
        //�������Ƿ�ʧ
        if (lpDDSPrimary->IsLost()==DDERR_SURFACELOST)
            lpDDSPrimary->Restore();
        if (lpDDSBack->IsLost()==DDERR_SURFACELOST)
            lpDDSBack->Restore();
        do{
            if(bFullScr==1)
                ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );
            else
                ddrval = lpDDSPrimary->Blt( pRectWin,lpDDSBack,NULL,DDBLT_WAIT,NULL);

            //��ת��������Ϣ���
            if( ddrval == DD_OK )
                break;
            else if( ddrval == DDERR_SURFACELOST ){
                if( FAILED(lpDDSBack->Restore()) )
                    break;
                if( FAILED(lpDDSPrimary->Restore()) )
                    break;
            }
            else if( ddrval != DDERR_WASSTILLDRAWING )
                break;
        }while( 1 );
    }
}

/**********************************************
*   ������:  UseClipper(...)
*   ����:  ����һ���ü���,����������������
*           ��Ȩ����  jk&ljq, 2010
***********************************************/
HRESULT CDDraw::UseClipper( void )
{
    HRESULT ddrval;
    LPDIRECTDRAWCLIPPER lpClipper = NULL;

    //����һ������������
    ddrval = lpDD->CreateClipper(0, &lpClipper, NULL);
    if ( ddrval != DD_OK){
        //����CreateClipper������������������������
        //��������Լ�¼������Ϣ
        return ddrval;
    }
    
    //���ü�������ΪhWndָ��Ĵ��ڵĿͻ����ķ�Χ
    ddrval = lpClipper->SetHWnd(0, hWnd);
    if ( ddrval != DD_OK){
        //����SetHWnd���ô��ھ�������������
        //��������Լ�¼������Ϣ
        return ddrval;
    }

    //���ü�������������
    ddrval = lpDDSPrimary->SetClipper(lpClipper);
    if ( ddrval != DD_OK){
        //���ü�����������������������
        //��������Լ�¼������Ϣ
        return ddrval;
    }

    //�������Ѿ��Զ������˲ü���������ֵ,����Ӧ�����ͷ�
    if( lpClipper != NULL ) { 
        lpClipper -> Release();
        lpClipper = NULL; 
    } 

    return DD_OK;
}
/**********************************************
*   ������: LoadBMPSurface(...)
*   ����:   ��BitmapFileָ����λͼ
*           ��ʽͼ������ո�Surfָ��ı���
*           ��Ȩ����  jk&ljq, 2010
***********************************************/
BOOL CDDraw::LoadBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,  //Ҫ����ͼ��ı���ָ��
							LPCSTR BitmapFile )//Դͼ���·��
{
    HDC                 hdcImage;
    HDC                 hdc;
    HBITMAP             hbm;
    BITMAP              bm;
    HRESULT				ddrval;

    if( lpSurf == NULL )
        return FALSE;
    // ��λͼ��Ϊ�ļ�����ȡ
    hbm = (HBITMAP)LoadImage(NULL,
							BitmapFile,
							IMAGE_BITMAP,
							0, 0,
							LR_LOADFROMFILE |
							LR_CREATEDIBSECTION);
    if (hbm == NULL){
        //����ͼ��ʧ��ʱ����������
        //��������Լ�¼����ͼ��ʧ�ܵ���Ϣ
		return FALSE;
	}
	//�������ܣ�����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���(Device Context)
	//����һ����Ӧ�ó���ĵ�ǰ��ʾ�����ݵ��ڴ��豸�����Ļ���
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage){
		//�����豸�����ĳ���
		return FALSE;
	}
	//ѡ��λͼ���󵽸ոմ������ڴ��豸�����Ļ�����DC��
    SelectObject(hdcImage, hbm);

	//��ȡλͼ�����
    GetObject(hbm, sizeof(bm), &bm);

	//��ȡ����Ŀ����
	DDSURFACEDESC2		ddsd;
    ddsd.dwSize = sizeof(ddsd);
    lpSurf->GetSurfaceDesc(&ddsd);
	if( BitmapFile!=NULL )
	{
        if ((ddrval = lpSurf->GetDC(&hdc)) == DD_OK)
        {
			//��Դλͼ���Ƶ����棬���Խ������������
            StretchBlt( hdc,
						0, 0, ddsd.dwWidth, ddsd.dwHeight,
						hdcImage,
						0, 0, bm.bmWidth, bm.bmHeight,
						SRCCOPY);
            lpSurf->ReleaseDC(hdc);
        }
	}

	DeleteDC(hdcImage);

	if( ddrval != DD_OK )  { 
		//���ƾ��ο�ʧ��
		return FALSE;
	}

	return TRUE;
}

/**********************************************************
*   ������: CreateBMPSurface(...)
*   ����:   ����lpSurfָ���ı���,���ҽ�BitmapFile
*           ָ����λͼ��ʽͼ������ոմ����ı���
*   ����˵��: lpSurf : Ҫ����ͼ��ı���ָ��
*             W��H : �����ı���Ŀ����(Ϊ���ʾ��ͼ��ƥ��)
*             BitmapFile : Դͼ���·��
*             MemoryFlag : ����ͼ��Ĵ洢��ʽ
*                          �Դ�:DDSCAPS_VIDEOMEMORY
*                          ϵͳ�ڴ�:DDSCAPS_SYSTEMMEMORY
*   ע��:   ��ָ�����泤��ʱ(W,H),����ĳ�����Բ�
*           ����ָ��ͼ��ĳ���
*           ��Ȩ����  jk&ljq, 2010
**************************************************/
BOOL CDDraw::CreateBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
								LONG W,LONG H,
								LPCSTR BitmapFile,
								DWORD MemoryFlag)
{
    HBITMAP             hbm;
    BITMAP              bm;
	BOOL				bResult;

    // ��λͼ��Ϊ�ļ�����ȡ
    hbm = (HBITMAP)LoadImage(NULL,
							BitmapFile,
							IMAGE_BITMAP,
							0, 0,
							LR_LOADFROMFILE |
							LR_CREATEDIBSECTION);
    if (hbm == NULL){
        //����ͼ��ʧ��ʱ����������
        //��������Լ�¼����ͼ��ʧ�ܵ���Ϣ
		return FALSE;
	}

    //���Ҫ�����ı���Ŀ����
	GetObject(hbm, sizeof(bm), &bm);
	W = W == 0 ? bm.bmWidth  : W;
	H = H == 0 ? bm.bmHeight : H;

    //��������Ŀ���ߴ���һ�������
    bResult = CreateNULLSurface( lpSurf, W, H, DDSCAPS_SYSTEMMEMORY );

	if( bResult != TRUE )   {
		//�����������ʱ����������
		//��������Լ�¼��������ĳ�����Ϣ
	}
	else if( BitmapFile!=NULL )	{
        //��BitmapFileָ���ͼ���ļ�����Surf����
        bResult = LoadBMPSurface( lpSurf, BitmapFile );
	}

    DeleteObject(hbm);

	return bResult;
}


/*********************************************************
*   ������:     CreateNULLSurface(...)
*   ����:       ����Surfָ���ı���,�ǿձ���
*   ����˵��:   lpSurf : Ҫ�����ı���ָ��
*               W��H : �����ı���Ŀ����
*               MemoryFlag : ����ͼ��Ĵ洢��ʽ
*                           �Դ�:DDSCAPS_VIDEOMEMORY
*                           ϵͳ�ڴ�:DDSCAPS_SYSTEMMEMORY
*               ��Ȩ����  jk&ljq, 2010
**********************************************************/
BOOL CDDraw::CreateNULLSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
								LONG W,LONG H,
								DWORD MemoryFlag)
{
	DDSURFACEDESC2     ddsd;
	HRESULT            ddrval;

	//�����������Ҫ��
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | MemoryFlag;
	ddsd.dwWidth  = W;
	ddsd.dwHeight = H;

	//����һ�������
	ddrval = lpDD->CreateSurface( &ddsd, &lpSurf, NULL );
	if( ddrval != DD_OK )   
		return FALSE;
		
	return TRUE;
}
/************************************************************
*   ������: Blit(...)
*   ����:   ��pRtSrcָ������Դ����ss�ϵ�ͼƬ���Ƶ�lpSurfDest
*           ָ����Ŀ�����
*   ����˵��: lpSurfDest : Ŀ�����ָ��
*                   x��y : ���Ƶ���Ŀ������λ��
*              lpSurfSrc : Դ����ָ��
*                 pRtSrc : Ҫ���Ƶ�Դ�����ͼƬλ�á�ֵΪNULL
*                          ��ʾ��������
*                dwTrans : ���͵ķ�ʽ
*               ��Ȩ����  jk&ljq, 2010
*************************************************************/
HRESULT	CDDraw::Blit( LPDIRECTDRAWSURFACE7 lpSurfDest,
                      LONG x,LONG y,
                      LPDIRECTDRAWSURFACE7 lpSurfSrc,
                      RECT * pRtSrc,
                      DWORD dwTrans )
{
	HRESULT			ddrval;

    //------------------��鲢�޸�Դ���εĺ�����
	DDSURFACEDESC2	ddsdSrc;
	ddsdSrc.dwSize=sizeof(ddsdSrc);
	ddrval=lpSurfSrc->GetSurfaceDesc(&ddsdSrc);
    if( ddrval == DD_OK && pRtSrc != NULL ) {
        LONG srcW = ddsdSrc.dwWidth;
        LONG srcH = ddsdSrc.dwHeight;
        if( srcH < pRtSrc->bottom )
            pRtSrc->bottom = srcH;
        if( srcH < pRtSrc->top )
            pRtSrc->top = srcH;
        if( srcW < pRtSrc->right )
            pRtSrc->right = srcW;
        if( srcW < pRtSrc->left )
            pRtSrc->left = srcW;
        if( pRtSrc->top < 0 )
            pRtSrc->top = 0;
        if( pRtSrc->bottom < 0 )
            pRtSrc->bottom = 0;
        if( pRtSrc->left < 0 )
            pRtSrc->left = 0;
        if( pRtSrc->right < 0 )
            pRtSrc->right = 0;
    }

    //------------------Դ���ΰ���Ŀ������С����
	DDSURFACEDESC2	ddsdDest;
	ddsdDest.dwSize=sizeof(ddsdDest);
	ddrval=lpSurfDest->GetSurfaceDesc(&ddsdDest);
	if( ddrval != DD_OK ) {
        //�޷�֪��Ŀ�����Ŀ����
        return ddrval;
    }
    else if(pRtSrc != NULL){
        LONG destW = ddsdDest.dwWidth;
        LONG destH = ddsdDest.dwHeight;
        if(x<0){//����Ŀ���������
            if((-x)>(pRtSrc->right-pRtSrc->left))
                return DD_FALSE;//��ȫ����Ŀ���������
            pRtSrc->left=pRtSrc->left-x;
            x=0;
        }
        if(x>(destW-(pRtSrc->right-pRtSrc->left)) ){
            //����Ŀ�������ұ�
            if(x>destW)
                return DD_FALSE;//��ȫ����Ŀ�������ұ�
            else
                pRtSrc->right=destW+pRtSrc->left-x;
        }
        if(y<0){//����Ŀ�������ϱ�
            if((-y)>(pRtSrc->bottom-pRtSrc->top))
                return DD_FALSE;//��ȫ����Ŀ�������ϱ�
            pRtSrc->top=pRtSrc->top-y;
            y=0;
        }
        if(y>(destH-(pRtSrc->bottom-pRtSrc->top)) ){
            //����Ŀ�����ĵױ�
            if(y>destH)
                return DD_FALSE;//��ȫ����Ŀ�����ĵױ�
            else
                pRtSrc->bottom=destH+pRtSrc->top-y;
        }
    }

	//-----------------λ�鴫��
	while( 1 )
	{
        if( pRtSrc != NULL )
            ddrval=lpSurfDest->BltFast(x,y,lpSurfSrc,pRtSrc,dwTrans);
        else {
            if(dwTrans == DDBLTFAST_SRCCOLORKEY || dwTrans == DDBLT_KEYSRC)
                ddrval=lpSurfDest->Blt( NULL,lpSurfSrc,
                                        NULL,DDBLT_WAIT | DDBLT_KEYSRC,
                                        NULL);
            else if(dwTrans == DDBLTFAST_DESTCOLORKEY || dwTrans == DDBLT_KEYDEST)
                ddrval=lpSurfDest->Blt( NULL,lpSurfSrc,
                                        NULL,DDBLT_WAIT | DDBLT_KEYDEST,
                                        NULL);
            else
                ddrval=lpSurfDest->Blt( NULL,lpSurfSrc,
                                        NULL,DDBLT_WAIT,
                                        NULL);
        }
        if(	ddrval == DD_OK )
            return ddrval;
        else if( ddrval == DDERR_SURFACELOST ){
            ddrval = lpDDSBack->Restore();
            ddrval = lpDDSPrimary->Restore();
            if( ddrval != DD_OK )
                return ddrval;
        }
        else if( ddrval != DDERR_WASSTILLDRAWING )
            return ddrval;
    }

    return ddrval;
}
/************************************************************
* ������: Blit(...)
* ����  : ��ָ����Դ����ͼƬ���͵���̨����
*               (c)  jk&ljq, 2010
*************************************************************/
HRESULT CDDraw::BlitBack( LONG x,LONG y,
                      LPDIRECTDRAWSURFACE7 lpSurfSrc,
                      RECT * pRtSrc,
                      DWORD dwTrans )
{
    return Blit( lpDDSBack,x,y,lpSurfSrc,pRtSrc,dwTrans );
}

/**************************************************
*  ������:  CheckDisplayMode(...)
*  ����:    �����ʾ�豸�Ƿ�֧���趨����ʾģʽ
*           ��Ȩ����  jk&ljq, 2010
***************************************************/
void CDDraw::CheckDisplayMode( void )
{
    DirectDrawEnumerateEx( DDisplayEnumerate, NULL, 0 );
    DWORD i=0;
    do{
        if(dmModes.pddsdMode[i].dwWidth == ScreenW &&
            dmModes.pddsdMode[i].dwHeight == ScreenH ){
            if(dmModes.pddsdMode[i].ddpfPixelFormat.dwRGBBitCount== ScreenC )
                break;
        }
        ++i;

        //������õ�ģʽ��֧��,��ʹ�õ�һ���鵽��ģʽ
        if(i==dmModes.dwNumMode){
            ScreenW = dmModes.pddsdMode[0].dwWidth;
            ScreenH = dmModes.pddsdMode[0].dwHeight;
            ScreenC = dmModes.pddsdMode[0].ddpfPixelFormat.dwRGBBitCount;
            MessageBox( NULL,
                "��ʾ�豸��֧�����õ���ʾģʽ,����ʹ�ò��ҵ��ĵ�һ��ģʽ!",
                "Warning!", MB_OK) ;
        }
    }while(i<dmModes.dwNumMode);

    //�ͷ��ڻص������п����Ŀռ�
    delete dmModes.pddsdMode;
}

/********************************************************
*  ������:  DDColorMatch(...)
*  ����:    �����rgbָ����ɫֵ��Ӧpdds���������ֵ
*  ˵��: �������rgbֵ��CLR_INVALID,��ʹ�ñ������
*        �Ͻǵ�������Ϊ�ؼ�ɫ
*********************************************************/
DWORD CDDraw::DDColorMatch(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb)
{
    if( pdds == NULL )
	    return 0x00000000;

    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT hres;

    //---------�����Ͻǵ����ɫ�滻ΪҪ���õ���ɫֵ
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);
        SetPixel(hdc, 0, 0, rgb);
        pdds->ReleaseDC(hdc);
    }

    //---------��������ֱ�ӶԱ�����ж�����
    ddsd.dwSize = sizeof(ddsd);
    hres = pdds->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
    if (hres == DD_OK)
    {
        /*
        ��ȡ��һ��,�����Ͻǵ���ڱ������ݻ�����������ֵ
        ���Ǳ������ɫ��ʽ���ܲ���32λ��ɫ���,���Ը���
        ��ֵ��������Ҫ���õ���ɫֵ�Ķ�Ӧ������
        */
        dw  = *(DWORD *)ddsd.lpSurface;

        /*
        ����������ɫ��ʽ���ܲ���32λ��ɫ���,��ֻ������ɫ
        ��ʽ��Ӧ��λ�����磺���16λ��ȵľ�ֻҪ����dw��ǰ16
        λ��ֵ,�򷽷���dw & 0xffff�������8λ������dw & 0xff
        ��32λ��ɫ��ȵĸ�ʽ,��ֵ�ʹ�����ɫֵ,���Բ��������
        */
        if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 )
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1; 
        pdds->Unlock(NULL);
    }

    //---------�ָ������滻�����Ͻǵ����ɫֵ
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }

    return dw;
}

/********************************************************
*  ������:  DDSetColorKey(...)
*  ����:    ������pdds����ָ���ı���Ĺؼ�ɫ
*  ˵��: �������rgbֵ��CLR_INVALID,��ʹ�ñ������
*        �Ͻǵ�������Ϊ�ؼ�ɫ
*********************************************************/
HRESULT CDDraw::DDSetColorKey(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;
    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

/********************************************************
*  ������:  GetRect(...)
*  ����:    ��ȡһ�����νṹ
*********************************************************/
RECT CDDraw::GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}

/********************************************************
*  ������:  ModeEnumRecord(...)
*  ����:    ��¼��⵽����ʾģʽ
*           IDirectDraw7::EnumDisplayModes�����Ļص�����
*********************************************************/
static HRESULT WINAPI ModeEnumRecord( DDSURFACEDESC2 * pNewMode,
                                      VOID * pRecordInfo )
{
    DISPLAYMODE * pAllModes = NULL;
    DDSURFACEDESC2 * pddsdNewAllModes = NULL;

    //��ȡ������¼��ָ��
    pAllModes = ( DISPLAYMODE * )pRecordInfo;

    //����Ϊģʽ�б����ռ�,ͬʱҲΪ���ҵ���ģʽ����ռ�
    pddsdNewAllModes = new DDSURFACEDESC2 [pAllModes->dwNumMode+1];

    /*
    ��ԭ����ģʽ�б�ռ�ֱ�Ӹ��Ƶ��·���Ŀռ䡣Ŀǰ,�б��
    ������һ��û�м�¼����*/
    DWORD dwSize = pAllModes->dwNumMode * sizeof(DDSURFACEDESC2);
    memcpy( pddsdNewAllModes, pAllModes->pddsdMode, dwSize );

    //ɾ�������˵Ŀռ�,��ָ��ָ���µĿռ�
    delete pAllModes->pddsdMode;
    pAllModes->pddsdMode = pddsdNewAllModes;

    //���ҵ�����ʾģʽ��¼����(��¼���б�ռ�����һ��)
    pAllModes->pddsdMode[pAllModes->dwNumMode] = (*pNewMode);

    //����ģʽ�б�����ֵ
    ++pAllModes->dwNumMode;

    return DDENUMRET_OK;
}


/**************************************************
*  ������:  DDisplayEnumerate(...)
*  ����:    ������ʾ��֧�ֵ���ʾģʽ
*           DirectDrawEnumerateEx�����Ļص�����
*           ��Ȩ����  jk&ljq, 2010
***************************************************/
static BOOL WINAPI DDisplayEnumerate( GUID * pGUID,  
                                LPSTR ,
                                LPSTR ,
                                LPVOID , HMONITOR )
{
    HRESULT hr;
	LPDIRECTDRAW7    lpDD;
	//����DirectDraw����
	hr = DirectDrawCreateEx(pGUID,(void **) &lpDD,IID_IDirectDraw7,NULL);
	if( FAILED(hr) )	{
        //����DirectDraw�������ʱ����������
        //��������Լ�¼������Ϣ
		return false;
	}

    //�ص�������ModeEnumRecord
    hr = lpDD->EnumDisplayModes( 0, NULL, 
                                 (LPVOID)&dmModes,
                                 ModeEnumRecord);

    lpDD->Release();
    return hr;

}
