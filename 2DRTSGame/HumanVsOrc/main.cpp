#include <windows.h>
#include "resource.h"
#include "CApplication.h"
#include "CGame.h"

CApplication    App;
CGame           Game;
HWND            hWnd = NULL;
BOOL            bFullScr = FALSE;
LONG            ScreenW = 800;
LONG            ScreenH = 640;

// 消息处理
LRESULT CALLBACK WinProc( HWND hWnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam )
{
    switch( uMsg )
    {
    //重画处于后台时被覆盖的部分
    case WM_PAINT:
        if(!bFullScr)
            Game.UpdateScreen(&App.Get_rectWin());
        break;
    }

    return App.MsgProc(hWnd,uMsg,wParam,lParam);
}

// WinMain程序入口
int WINAPI WinMain(	HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow)
{
    //>>>>>>>>>> CApplication的使用 >>>>>>>>>>
    App.isFullScreen( bFullScr );
    // 客户区背景色是黑色
    App.SetClientBKColor( BLACK_BRUSH );
    // 载入自定义图标
    //App.Set_hIcon( hInstance, IDI_STUDY );
    // 载入自定义光标
    //App.Set_hCursor( hInstance, IDC_MYCURSOR );
    // 禁止改变窗口尺寸
    App.ChangeSizeAble( FALSE );
    // 设置窗口大小
    App.SetScreenW( ScreenW );
    App.SetScreenH( ScreenH );

    // 创建窗口
    App.CreateWin( WinProc,
                   hInstance,
                   "HumanVsOrc",
                   WS_OVERLAPPEDWINDOW &
                   ~WS_MAXIMIZEBOX );
    hWnd = App.Get_hWnd();

    //>>>>>> 初始化游戏类 >>>>>>>>>
    Game.Set_hWnd( hWnd );
    Game.Set_bFullScr( bFullScr );
    Game.Set_ScreenW( ScreenW );
    Game.Set_ScreenH( ScreenH );
    Game.InitDirectX( hWnd );

    // 运行消息机制
    MSG  msg;
    LPRECT lpRect;
    while( TRUE )
    {
        if( App.Get_bActive() )
        {
            if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            lpRect = &App.Get_rectWin();
            //游戏进程
            Game.GetInputDeviceData( lpRect );
            Game.GameProcess();
            //更新屏幕
            Game.UpdateScreen( lpRect );
        }
        else  {
            if( GetMessage(&msg,NULL,0,0) )
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
                break;
        }
	}
	return msg.wParam;
}

