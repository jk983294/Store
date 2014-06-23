/*-------------------------------------------
|           CDDraw.cpp
|       CDDraw类的成员的源代码
|           版权所有  jk&ljq, 2010
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
*  函数名:  CDDraw(...)
*  功能:  构造函数
*           版权所有  jk&ljq, 2010
*****************************************/
CDDraw::CDDraw()
{
	//初始化变量
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
*  函数名:  ~CDDraw()
*  功能:  析构函数
*           版权所有  jk&ljq, 2010
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
*  函数名:  InitDDraw(...)
*  功能:  初始化DDraw,并且创建主表面和翻转缓冲表面
*           版权所有  jk&ljq, 2010
***************************************************/
BOOL CDDraw::InitDDraw(HWND hWd,
					   BOOL bFScr,
					   DWORD ScrW,DWORD ScrH,
					   DWORD BitCount )
{
	//--------------------------按给出的条件初始化变量
	hWnd=hWd;
	bFullScr=bFScr;
	if(bFScr){//全屏模式
		if(ScrW <= 640)		{ ScreenW = 640; ScreenH = 480; }
		else if(ScrW <= 800){ ScreenW = 800; ScreenH = 600; }
		else if(ScrW > 800) { ScreenW = 1024; ScreenH = 768; }
		else {ScreenW=800; ScreenH = 600;}
		if(BitCount<=8)		ScreenC = 8;
		else if(BitCount<=16)ScreenC = 16;
		else if(BitCount<=24)ScreenC = 24;
		else if(BitCount<=32)ScreenC = 32;
		else ScreenC=16;

        //检查模式是否有效
        CheckDisplayMode();
	}
	else{//窗口模式
		ScreenW=ScrW;
		ScreenH=ScrH;
	}

	//--------------------------初始化DirectDraw环境
	HRESULT ddrval;
	//创建DirectDraw对象
	ddrval = DirectDrawCreateEx(NULL,(void **) &lpDD,IID_IDirectDraw7,NULL);
	if( FAILED(ddrval) )	{
		//创建DirectDraw对象出错时进到这里来
		//在这里可以记录创建DirectDraw对象的出错信息
		return FALSE;
	}
	//******全屏模式***********//
	if( bFullScr ){
		//设置协作等级
		ddrval = lpDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT );
		if( FAILED( ddrval )){
			//设置协作等级出错时进到这里来
			//在这里可以记录设置协作等级的出错信息
			return FALSE;
		}

		//设置显示模式
		ddrval = lpDD->SetDisplayMode( ScreenW,ScreenH,ScreenC,NULL,NULL);
		if( FAILED( ddrval ) ){
			//设置显示模式出错时进到这里来
			//在这里可以记录设置显示模式的出错信息
			return FALSE;
		}

		//创建主表面
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
			//创建主表面出错时进到这里来
			//在这里可以记录创建主表面的出错信息
			return FALSE;
		}

		//创建后台缓冲表面
		DDSCAPS2 ddscaps;
		ZeroMemory(&ddscaps,sizeof(ddscaps));
		ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
		ddrval=lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
		if (ddrval!=DD_OK) {
			//获取后台缓冲表面出错时进到这里来
			//在这里可以记录后台缓冲表面的出错信息
			return FALSE;
		}
		return TRUE;
	}
	//******窗口模式***********//
	else{
		//设置协作等级
		ddrval=(HRESULT)lpDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );

		//创建主表面
		DDSURFACEDESC2 	ddsd;
		ZeroMemory( &ddsd, sizeof(ddsd) );
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	    //创建主表面，并取得指向主表面的指针lpDDSPrimary
		ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
		if( FAILED( ddrval )){
			//创建主表面出错时进到这里来
			//在这里可以记录创建主表面的出错信息
			return FALSE;
		}

        //使用裁剪器
        UseClipper();

		//创建后台缓冲表面
		//:::::::ddsd前面已经声明
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize=sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth  = ScreenW;//宽的分辨率
		ddsd.dwHeight = ScreenH;//高的分辨率
		//创建一后台缓冲表面
		ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
		if( FAILED( ddrval )){
			//创建后台缓冲表面出错时进到这里来
			//在这里可以记录创建后台缓冲表面的出错信息
			return FALSE;
		}
	}

    return TRUE;
	//--------------------------初始化DirectDraw环境完成
}

/**************************************************
*  函数名:  Flip(...)
*  功能:  翻转表面==>显示
*           版权所有  jk&ljq, 2010
***************************************************/
void CDDraw::Flip( LPRECT pRectWin )
{
    HRESULT   ddrval=1;

    if( lpDDSPrimary && lpDDSBack )
    {
        //检查表面是否丢失
        if (lpDDSPrimary->IsLost()==DDERR_SURFACELOST)
            lpDDSPrimary->Restore();
        if (lpDDSBack->IsLost()==DDERR_SURFACELOST)
            lpDDSBack->Restore();
        do{
            if(bFullScr==1)
                ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );
            else
                ddrval = lpDDSPrimary->Blt( pRectWin,lpDDSBack,NULL,DDBLT_WAIT,NULL);

            //翻转表面后的信息检测
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
*   函数名:  UseClipper(...)
*   功能:  创建一个裁剪器,并将它附于主表面
*           版权所有  jk&ljq, 2010
***********************************************/
HRESULT CDDraw::UseClipper( void )
{
    HRESULT ddrval;
    LPDIRECTDRAWCLIPPER lpClipper = NULL;

    //创建一个剪裁器对象
    ddrval = lpDD->CreateClipper(0, &lpClipper, NULL);
    if ( ddrval != DD_OK){
        //调用CreateClipper创建剪裁器对象出错进到这来
        //在这里可以记录出错信息
        return ddrval;
    }
    
    //将裁剪器设置为hWnd指向的窗口的客户区的范围
    ddrval = lpClipper->SetHWnd(0, hWnd);
    if ( ddrval != DD_OK){
        //调用SetHWnd设置窗口句柄出错进到这来
        //在这里可以记录出错信息
        return ddrval;
    }

    //将裁剪器附于主表面
    ddrval = lpDDSPrimary->SetClipper(lpClipper);
    if ( ddrval != DD_OK){
        //将裁剪器附于主表面出错进到这来
        //在这里可以记录出错信息
        return ddrval;
    }

    //主表面已经自动增加了裁剪器的索引值,所以应将它释放
    if( lpClipper != NULL ) { 
        lpClipper -> Release();
        lpClipper = NULL; 
    } 

    return DD_OK;
}
/**********************************************
*   函数名: LoadBMPSurface(...)
*   功能:   将BitmapFile指定的位图
*           格式图像载入刚刚Surf指向的表面
*           版权所有  jk&ljq, 2010
***********************************************/
BOOL CDDraw::LoadBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,  //要载入图像的表面指针
							LPCSTR BitmapFile )//源图像的路径
{
    HDC                 hdcImage;
    HDC                 hdc;
    HBITMAP             hbm;
    BITMAP              bm;
    HRESULT				ddrval;

    if( lpSurf == NULL )
        return FALSE;
    // 将位图作为文件来读取
    hbm = (HBITMAP)LoadImage(NULL,
							BitmapFile,
							IMAGE_BITMAP,
							0, 0,
							LR_LOADFROMFILE |
							LR_CREATEDIBSECTION);
    if (hbm == NULL){
        //载入图像失败时进到这里来
        //在这里可以记录载入图像失败的信息
		return FALSE;
	}
	//函数功能：创建一个与指定设备兼容的内存设备上下文环境(Device Context)
	//创建一个与应用程序的当前显示器兼容的内存设备上下文环境
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage){
		//创建设备上下文出错
		return FALSE;
	}
	//选择位图对象到刚刚创建的内存设备上下文环境（DC）
    SelectObject(hdcImage, hbm);

	//获取位图宽与高
    GetObject(hbm, sizeof(bm), &bm);

	//获取表面的宽与高
	DDSURFACEDESC2		ddsd;
    ddsd.dwSize = sizeof(ddsd);
    lpSurf->GetSurfaceDesc(&ddsd);
	if( BitmapFile!=NULL )
	{
        if ((ddrval = lpSurf->GetDC(&hdc)) == DD_OK)
        {
			//将源位图复制到表面，可以进行拉伸或收缩
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
		//复制矩形块失败
		return FALSE;
	}

	return TRUE;
}

/**********************************************************
*   函数名: CreateBMPSurface(...)
*   功能:   创建lpSurf指定的表面,并且将BitmapFile
*           指定的位图格式图像载入刚刚创建的表面
*   参数说明: lpSurf : 要载入图像的表面指针
*             W和H : 创建的表面的宽与高(为零表示和图像匹配)
*             BitmapFile : 源图像的路径
*             MemoryFlag : 表面图像的存储方式
*                          显存:DDSCAPS_VIDEOMEMORY
*                          系统内存:DDSCAPS_SYSTEMMEMORY
*   注意:   当指定表面长宽时(W,H),表面的长宽可以不
*           等于指定图像的长宽
*           版权所有  jk&ljq, 2010
**************************************************/
BOOL CDDraw::CreateBMPSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
								LONG W,LONG H,
								LPCSTR BitmapFile,
								DWORD MemoryFlag)
{
    HBITMAP             hbm;
    BITMAP              bm;
	BOOL				bResult;

    // 将位图作为文件来读取
    hbm = (HBITMAP)LoadImage(NULL,
							BitmapFile,
							IMAGE_BITMAP,
							0, 0,
							LR_LOADFROMFILE |
							LR_CREATEDIBSECTION);
    if (hbm == NULL){
        //载入图像失败时进到这里来
        //在这里可以记录载入图像失败的信息
		return FALSE;
	}

    //求出要创建的表面的宽与高
	GetObject(hbm, sizeof(bm), &bm);
	W = W == 0 ? bm.bmWidth  : W;
	H = H == 0 ? bm.bmHeight : H;

    //根据上面的宽与高创建一屏外表面
    bResult = CreateNULLSurface( lpSurf, W, H, DDSCAPS_SYSTEMMEMORY );

	if( bResult != TRUE )   {
		//创建表面出错时进到这里来
		//在这里可以记录创建表面的出错信息
	}
	else if( BitmapFile!=NULL )	{
        //将BitmapFile指向的图像文件载入Surf表面
        bResult = LoadBMPSurface( lpSurf, BitmapFile );
	}

    DeleteObject(hbm);

	return bResult;
}


/*********************************************************
*   函数名:     CreateNULLSurface(...)
*   功能:       创建Surf指定的表面,是空表面
*   参数说明:   lpSurf : 要创建的表面指针
*               W和H : 创建的表面的宽与高
*               MemoryFlag : 表面图像的存储方式
*                           显存:DDSCAPS_VIDEOMEMORY
*                           系统内存:DDSCAPS_SYSTEMMEMORY
*               版权所有  jk&ljq, 2010
**********************************************************/
BOOL CDDraw::CreateNULLSurface(LPDIRECTDRAWSURFACE7 &lpSurf,
								LONG W,LONG H,
								DWORD MemoryFlag)
{
	DDSURFACEDESC2     ddsd;
	HRESULT            ddrval;

	//定义屏外表面要求
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | MemoryFlag;
	ddsd.dwWidth  = W;
	ddsd.dwHeight = H;

	//创建一屏外表面
	ddrval = lpDD->CreateSurface( &ddsd, &lpSurf, NULL );
	if( ddrval != DD_OK )   
		return FALSE;
		
	return TRUE;
}
/************************************************************
*   函数名: Blit(...)
*   功能:   将pRtSrc指定的在源表面ss上的图片复制到lpSurfDest
*           指定的目标表面
*   参数说明: lpSurfDest : 目标表面指针
*                   x和y : 复制到的目标表面的位置
*              lpSurfSrc : 源表面指针
*                 pRtSrc : 要复制的源表面的图片位置。值为NULL
*                          表示整个表面
*                dwTrans : 传送的方式
*               版权所有  jk&ljq, 2010
*************************************************************/
HRESULT	CDDraw::Blit( LPDIRECTDRAWSURFACE7 lpSurfDest,
                      LONG x,LONG y,
                      LPDIRECTDRAWSURFACE7 lpSurfSrc,
                      RECT * pRtSrc,
                      DWORD dwTrans )
{
	HRESULT			ddrval;

    //------------------检查并修改源矩形的合理性
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

    //------------------源矩形按照目标表面大小调整
	DDSURFACEDESC2	ddsdDest;
	ddsdDest.dwSize=sizeof(ddsdDest);
	ddrval=lpSurfDest->GetSurfaceDesc(&ddsdDest);
	if( ddrval != DD_OK ) {
        //无法知道目标表面的宽与高
        return ddrval;
    }
    else if(pRtSrc != NULL){
        LONG destW = ddsdDest.dwWidth;
        LONG destH = ddsdDest.dwHeight;
        if(x<0){//超出目标表面的左边
            if((-x)>(pRtSrc->right-pRtSrc->left))
                return DD_FALSE;//完全超出目标表面的左边
            pRtSrc->left=pRtSrc->left-x;
            x=0;
        }
        if(x>(destW-(pRtSrc->right-pRtSrc->left)) ){
            //超出目标表面的右边
            if(x>destW)
                return DD_FALSE;//完全超出目标表面的右边
            else
                pRtSrc->right=destW+pRtSrc->left-x;
        }
        if(y<0){//超出目标表面的上边
            if((-y)>(pRtSrc->bottom-pRtSrc->top))
                return DD_FALSE;//完全超出目标表面的上边
            pRtSrc->top=pRtSrc->top-y;
            y=0;
        }
        if(y>(destH-(pRtSrc->bottom-pRtSrc->top)) ){
            //超出目标表面的底边
            if(y>destH)
                return DD_FALSE;//完全超出目标表面的底边
            else
                pRtSrc->bottom=destH+pRtSrc->top-y;
        }
    }

	//-----------------位块传送
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
* 函数名: Blit(...)
* 功能  : 将指定的源表面图片传送到后台表面
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
*  函数名:  CheckDisplayMode(...)
*  功能:    检测显示设备是否支持设定的显示模式
*           版权所有  jk&ljq, 2010
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

        //如果设置的模式不支持,则使用第一个查到的模式
        if(i==dmModes.dwNumMode){
            ScreenW = dmModes.pddsdMode[0].dwWidth;
            ScreenH = dmModes.pddsdMode[0].dwHeight;
            ScreenC = dmModes.pddsdMode[0].ddpfPixelFormat.dwRGBBitCount;
            MessageBox( NULL,
                "显示设备不支持设置的显示模式,现在使用查找到的第一个模式!",
                "Warning!", MB_OK) ;
        }
    }while(i<dmModes.dwNumMode);

    //释放在回调函数中开创的空间
    delete dmModes.pddsdMode;
}

/********************************************************
*  函数名:  DDColorMatch(...)
*  功能:    获得由rgb指定颜色值对应pdds表面的索引值
*  说明: 如果参数rgb值是CLR_INVALID,将使用表面的左
*        上角的象素作为关键色
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

    //---------将左上角点的颜色替换为要设置的颜色值
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);
        SetPixel(hdc, 0, 0, rgb);
        pdds->ReleaseDC(hdc);
    }

    //---------锁定表面直接对表面进行读操作
    ddsd.dwSize = sizeof(ddsd);
    hres = pdds->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
    if (hres == DD_OK)
    {
        /*
        获取第一点,即左上角点的在表面数据缓冲区的索引值
        但是表面的颜色格式可能不是32位颜色深度,所以该索
        引值并不就是要设置的颜色值的对应索引。
        */
        dw  = *(DWORD *)ddsd.lpSurface;

        /*
        如果表面的颜色格式可能不是32位颜色深度,就只留下颜色
        格式对应的位数。如：如果16位深度的就只要保留dw的前16
        位的值,则方法是dw & 0xffff。如果是8位，则是dw & 0xff
        是32位颜色深度的格式,其值就代表颜色值,所以不用再求解
        */
        if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 )
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1; 
        pdds->Unlock(NULL);
    }

    //---------恢复上面替换的左上角点的颜色值
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }

    return dw;
}

/********************************************************
*  函数名:  DDSetColorKey(...)
*  功能:    设置由pdds参数指定的表面的关键色
*  说明: 如果参数rgb值是CLR_INVALID,将使用表面的左
*        上角的象素作为关键色
*********************************************************/
HRESULT CDDraw::DDSetColorKey(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;
    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

/********************************************************
*  函数名:  GetRect(...)
*  功能:    获取一个矩形结构
*********************************************************/
RECT CDDraw::GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}

/********************************************************
*  函数名:  ModeEnumRecord(...)
*  功能:    记录检测到的显示模式
*           IDirectDraw7::EnumDisplayModes方法的回调函数
*********************************************************/
static HRESULT WINAPI ModeEnumRecord( DDSURFACEDESC2 * pNewMode,
                                      VOID * pRecordInfo )
{
    DISPLAYMODE * pAllModes = NULL;
    DDSURFACEDESC2 * pddsdNewAllModes = NULL;

    //获取整个记录的指针
    pAllModes = ( DISPLAYMODE * )pRecordInfo;

    //重新为模式列表分配空间,同时也为新找到的模式分配空间
    pddsdNewAllModes = new DDSURFACEDESC2 [pAllModes->dwNumMode+1];

    /*
    将原来的模式列表空间直接复制到新分配的空间。目前,列表空
    间的最后一项没有记录数据*/
    DWORD dwSize = pAllModes->dwNumMode * sizeof(DDSURFACEDESC2);
    memcpy( pddsdNewAllModes, pAllModes->pddsdMode, dwSize );

    //删除不用了的空间,将指针指向新的空间
    delete pAllModes->pddsdMode;
    pAllModes->pddsdMode = pddsdNewAllModes;

    //将找到的显示模式记录下来(记录在列表空间的最后一项)
    pAllModes->pddsdMode[pAllModes->dwNumMode] = (*pNewMode);

    //更新模式列表索引值
    ++pAllModes->dwNumMode;

    return DDENUMRET_OK;
}


/**************************************************
*  函数名:  DDisplayEnumerate(...)
*  功能:    查找显示器支持的显示模式
*           DirectDrawEnumerateEx函数的回调函数
*           版权所有  jk&ljq, 2010
***************************************************/
static BOOL WINAPI DDisplayEnumerate( GUID * pGUID,  
                                LPSTR ,
                                LPSTR ,
                                LPVOID , HMONITOR )
{
    HRESULT hr;
	LPDIRECTDRAW7    lpDD;
	//创建DirectDraw对象
	hr = DirectDrawCreateEx(pGUID,(void **) &lpDD,IID_IDirectDraw7,NULL);
	if( FAILED(hr) )	{
        //创建DirectDraw对象出错时进到这里来
        //在这里可以记录出错信息
		return false;
	}

    //回调函数是ModeEnumRecord
    hr = lpDD->EnumDisplayModes( 0, NULL, 
                                 (LPVOID)&dmModes,
                                 ModeEnumRecord);

    lpDD->Release();
    return hr;

}
