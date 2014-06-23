/*------------------------------------------------------
|               CApplication.cpp
|        CApplication类的成员的源代码
|            版权所有  jk&ljq, 2010
--------------------------------------------------------*/

#include "CApplication.h"
#include <windows.h>

/************************************************************
*   函数名: CApplication(...) 属于CApplication类的成员
*     功能: 构造函数
* 参数说明:
*     bFullScr 是否全屏
*     ScrW     客户区的宽度(全屏时就是屏幕的宽度)
*     ScrH     客户区的高度(全屏时就是屏幕的高度)
*       版权所有 jk&ljq, 2010
*************************************************************/
CApplication::CApplication(int bFullScr, 
                           int ScrW, int ScrH)
{
    m_bFullScreen = bFullScr;

    //全屏模式
    if( bFullScr ) {
        if( ScrW >= 1024 ) {
            ScreenW = 1024;
            ScreenH = 768;
        }
        else if( ScrW >= 800 ) {
            ScreenW = 800;
            ScreenH = 600;
        }
        else {
            ScreenW = 640;
            ScreenH = 480;
        }
    }
    //窗口模式
    else {
        ScreenW = ScrW;
        ScreenH = ScrH;
    }

    bActive          = FALSE;
    hWnd             = NULL;
    Show_Mouse_Cursor= TRUE;
    Change_Size_Able = FALSE;
    ClientBKColor    = WHITE_BRUSH;

    hIcon=LoadIcon(NULL,MAKEINTRESOURCE(IDI_APPLICATION));
    hCursor=LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
    hMenu = NULL;
}


/************************************************************
*      构造函数重载声明
*************************************************************/
CApplication::CApplication( void )
{
    m_bFullScreen    = FALSE;
    ScreenW          = 640;
    ScreenH          = 480;
    bActive          = FALSE;
    hWnd             = NULL;
    Show_Mouse_Cursor= TRUE;
    Change_Size_Able = TRUE;
    ClientBKColor    = WHITE_BRUSH;

    hIcon=LoadIcon(NULL,MAKEINTRESOURCE(IDI_APPLICATION));
    hCursor=LoadCursor( NULL, MAKEINTRESOURCE(IDC_ARROW));
    hMenu = NULL;
}


/************************************************************
*    函数名: CreateWin(...) 属于CApplication类的成员
*      功能: 定义并且创建应用程序窗口,显示窗口
*  参数说明: 
*   WinProc 窗口的消息处理函数名
*   hInstance 窗口实例句柄
*   WindowName 窗口的名字和注册名字
*   winStyle 窗口的类型
*       版权所有 jk&ljq, 2010
*************************************************************/
HWND CApplication::CreateWin( WNDPROC WinProc,
                              HINSTANCE hInstance,
                              TCHAR* WindowName,
                              DWORD winStyle)
{
    WNDCLASS wc;
    wc.hInstance	 = hInstance;
    wc.lpszClassName = WindowName;
    wc.lpfnWndProc	 = WinProc;
    wc.style		 = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon		 = hIcon;
    wc.hCursor		 = hCursor;
    wc.lpszMenuName	 = NULL;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hbrBackground = ( HBRUSH )GetStockObject( ClientBKColor );

    if( !RegisterClass( &wc ) )
    {
        MessageBox( NULL, "注册窗口类出错!","Error!", MB_OK) ;
        return FALSE;
    }

    //全屏模式
    if( m_bFullScreen == TRUE ) {
        hWnd = CreateWindow(
            WindowName,
            WindowName,
            WS_POPUP,
            0,
            0,
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN),
            NULL,
            hMenu,
            hInstance,
            NULL );
        if( !hWnd ) {
            MessageBox( NULL, "创建窗口出错!","Error!", MB_OK) ;
            return FALSE;
        }
    }
    else{//窗口模式
        hWnd = CreateWindowEx(
            0,
            WindowName,
            WindowName,
            winStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
	        CW_USEDEFAULT,
            NULL,
            hMenu,
            hInstance,
            NULL );
        if( !hWnd ) {
            MessageBox( NULL, "创建窗口出错!","Error!", MB_OK) ;
            return FALSE;
        }

        //获取系统屏幕大小
        int	Win_W=GetSystemMetrics(SM_CXSCREEN);
        int Win_H=GetSystemMetrics(SM_CYSCREEN);
        //求出窗口居于屏幕中央时的左上角位置
        int x1=(Win_W-ScreenW)/2;
        int y1=(Win_H-ScreenH)/2;
        //根据上面的值,调整窗口的位置
        SetWindowPos( hWnd, HWND_NOTOPMOST,
                      x1, y1, ScreenW, ScreenH,
                      SWP_HIDEWINDOW );
        //获取客户区的相对于屏幕左上角的位置
        GetClientRect( hWnd, &rectWin );
        ClientToScreen( hWnd, (LPPOINT)&rectWin );
        ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
    }

    ShowWindow( hWnd, SW_NORMAL );
    UpdateWindow( hWnd );

    return hWnd;
}

/************************************************************
*   函数名: MsgProc(...) 属于CApplication类的成员
*     功能: 处理主窗口消息
*   参数说明:
*       hWnd 表示对应窗口的句柄。
*       uMsg 传送给消息处理函数的消息。
*       wParam和lParam 的值与对应的消息有关。
*           版权所有 jk&ljq, 2010
*************************************************************/
LRESULT CApplication::MsgProc(  HWND   hWnd, 
								UINT   uMsg,
								WPARAM wParam,
								LPARAM lParam )
{
    switch( uMsg )
    {
    //窗口激活状态消息的处理
	case WM_ACTIVATEAPP:
		 bActive = (BOOL)wParam;
		 break;
    //鼠标指针移动消息的处理
	case WM_SETCURSOR:
		if( !Show_Mouse_Cursor ){
			SetCursor(NULL);
            return true;
		}
		break;
    //窗口大小改变消息的处理
	case WM_SIZE:
		if( Change_Size_Able ){
			if( m_bFullScreen==0 ){
				//获取窗口客户区的屏幕坐标系位置
				GetClientRect( hWnd, &rectWin );
				ClientToScreen( hWnd, (LPPOINT)&rectWin );
				ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
			}
		}
		break;
    //窗口位置改变消息的处理
	case WM_MOVE:
		if( m_bFullScreen==0 ){
			//获取窗口客户区的屏幕坐标系位置
			GetClientRect( hWnd, &rectWin );
			ClientToScreen( hWnd, (LPPOINT)&rectWin );
			ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
		}
		break;
    //窗口最大尺寸和最小尺寸消息的处理
	case WM_GETMINMAXINFO:
        //不可以改变窗口的大小
		if( !Change_Size_Able ) {
            PMINMAXINFO pMM = ( PMINMAXINFO ) lParam;

            //获取横向、纵向边框的宽度和标题栏的高度
			int FrameW = GetSystemMetrics( SM_CXSIZEFRAME );
			int FrameH = GetSystemMetrics( SM_CYSIZEFRAME );
			int CapH   = GetSystemMetrics( SM_CYCAPTION );

            //把窗口能被改变到的最小尺寸改为以前的值
			pMM->ptMinTrackSize.x = ScreenW + FrameW*2;
			pMM->ptMinTrackSize.y = ScreenH + FrameH*2 + CapH;
            //将窗口能被改变到的最大尺寸设为与最小尺寸相同的值
			pMM->ptMaxTrackSize.x = pMM->ptMinTrackSize.x;
			pMM->ptMaxTrackSize.y = pMM->ptMinTrackSize.y;
		}
        else {
            PMINMAXINFO pMM = ( PMINMAXINFO ) lParam;

            //限制窗口能被改变到的最小尺寸
            pMM->ptMinTrackSize.x = 100;
            pMM->ptMinTrackSize.y = 150;
        }
		break;
    //关闭窗口消息的处理
	case WM_CLOSE:
	    DestroyWindow( hWnd );
		break;
    //销毁窗口消息的处理
    case WM_DESTROY:
	    PostQuitMessage( 0 );
        break;
    }

	//调用缺省消息处理过程
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*********************************************************
*	函数名: RunDefault(...) 属于CApplication类的成员
*	功能: 主窗口消息循环
*       版权所有 jk&ljq, 2010
**********************************************************/
int CApplication::RunDefault( void )
{
	MSG  msg;	msg.message = WM_NULL;

	while(msg.message != WM_QUIT)
	{
		if( GetMessage(&msg,NULL,0,0) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
