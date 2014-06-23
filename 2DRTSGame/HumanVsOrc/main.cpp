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

// ��Ϣ����
LRESULT CALLBACK WinProc( HWND hWnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam )
{
    switch( uMsg )
    {
    //�ػ����ں�̨ʱ�����ǵĲ���
    case WM_PAINT:
        if(!bFullScr)
            Game.UpdateScreen(&App.Get_rectWin());
        break;
    }

    return App.MsgProc(hWnd,uMsg,wParam,lParam);
}

// WinMain�������
int WINAPI WinMain(	HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow)
{
    //>>>>>>>>>> CApplication��ʹ�� >>>>>>>>>>
    App.isFullScreen( bFullScr );
    // �ͻ�������ɫ�Ǻ�ɫ
    App.SetClientBKColor( BLACK_BRUSH );
    // �����Զ���ͼ��
    //App.Set_hIcon( hInstance, IDI_STUDY );
    // �����Զ�����
    //App.Set_hCursor( hInstance, IDC_MYCURSOR );
    // ��ֹ�ı䴰�ڳߴ�
    App.ChangeSizeAble( FALSE );
    // ���ô��ڴ�С
    App.SetScreenW( ScreenW );
    App.SetScreenH( ScreenH );

    // ��������
    App.CreateWin( WinProc,
                   hInstance,
                   "HumanVsOrc",
                   WS_OVERLAPPEDWINDOW &
                   ~WS_MAXIMIZEBOX );
    hWnd = App.Get_hWnd();

    //>>>>>> ��ʼ����Ϸ�� >>>>>>>>>
    Game.Set_hWnd( hWnd );
    Game.Set_bFullScr( bFullScr );
    Game.Set_ScreenW( ScreenW );
    Game.Set_ScreenH( ScreenH );
    Game.InitDirectX( hWnd );

    // ������Ϣ����
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
            //��Ϸ����
            Game.GetInputDeviceData( lpRect );
            Game.GameProcess();
            //������Ļ
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

