/*------------------------------------------------------
|               CApplication.cpp
|        CApplication��ĳ�Ա��Դ����
|            ��Ȩ����  jk&ljq, 2010
--------------------------------------------------------*/

#include "CApplication.h"
#include <windows.h>

/************************************************************
*   ������: CApplication(...) ����CApplication��ĳ�Ա
*     ����: ���캯��
* ����˵��:
*     bFullScr �Ƿ�ȫ��
*     ScrW     �ͻ����Ŀ��(ȫ��ʱ������Ļ�Ŀ��)
*     ScrH     �ͻ����ĸ߶�(ȫ��ʱ������Ļ�ĸ߶�)
*       ��Ȩ���� jk&ljq, 2010
*************************************************************/
CApplication::CApplication(int bFullScr, 
                           int ScrW, int ScrH)
{
    m_bFullScreen = bFullScr;

    //ȫ��ģʽ
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
    //����ģʽ
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
*      ���캯����������
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
*    ������: CreateWin(...) ����CApplication��ĳ�Ա
*      ����: ���岢�Ҵ���Ӧ�ó��򴰿�,��ʾ����
*  ����˵��: 
*   WinProc ���ڵ���Ϣ��������
*   hInstance ����ʵ�����
*   WindowName ���ڵ����ֺ�ע������
*   winStyle ���ڵ�����
*       ��Ȩ���� jk&ljq, 2010
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
        MessageBox( NULL, "ע�ᴰ�������!","Error!", MB_OK) ;
        return FALSE;
    }

    //ȫ��ģʽ
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
            MessageBox( NULL, "�������ڳ���!","Error!", MB_OK) ;
            return FALSE;
        }
    }
    else{//����ģʽ
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
            MessageBox( NULL, "�������ڳ���!","Error!", MB_OK) ;
            return FALSE;
        }

        //��ȡϵͳ��Ļ��С
        int	Win_W=GetSystemMetrics(SM_CXSCREEN);
        int Win_H=GetSystemMetrics(SM_CYSCREEN);
        //������ھ�����Ļ����ʱ�����Ͻ�λ��
        int x1=(Win_W-ScreenW)/2;
        int y1=(Win_H-ScreenH)/2;
        //���������ֵ,�������ڵ�λ��
        SetWindowPos( hWnd, HWND_NOTOPMOST,
                      x1, y1, ScreenW, ScreenH,
                      SWP_HIDEWINDOW );
        //��ȡ�ͻ������������Ļ���Ͻǵ�λ��
        GetClientRect( hWnd, &rectWin );
        ClientToScreen( hWnd, (LPPOINT)&rectWin );
        ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
    }

    ShowWindow( hWnd, SW_NORMAL );
    UpdateWindow( hWnd );

    return hWnd;
}

/************************************************************
*   ������: MsgProc(...) ����CApplication��ĳ�Ա
*     ����: ������������Ϣ
*   ����˵��:
*       hWnd ��ʾ��Ӧ���ڵľ����
*       uMsg ���͸���Ϣ����������Ϣ��
*       wParam��lParam ��ֵ���Ӧ����Ϣ�йء�
*           ��Ȩ���� jk&ljq, 2010
*************************************************************/
LRESULT CApplication::MsgProc(  HWND   hWnd, 
								UINT   uMsg,
								WPARAM wParam,
								LPARAM lParam )
{
    switch( uMsg )
    {
    //���ڼ���״̬��Ϣ�Ĵ���
	case WM_ACTIVATEAPP:
		 bActive = (BOOL)wParam;
		 break;
    //���ָ���ƶ���Ϣ�Ĵ���
	case WM_SETCURSOR:
		if( !Show_Mouse_Cursor ){
			SetCursor(NULL);
            return true;
		}
		break;
    //���ڴ�С�ı���Ϣ�Ĵ���
	case WM_SIZE:
		if( Change_Size_Able ){
			if( m_bFullScreen==0 ){
				//��ȡ���ڿͻ�������Ļ����ϵλ��
				GetClientRect( hWnd, &rectWin );
				ClientToScreen( hWnd, (LPPOINT)&rectWin );
				ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
			}
		}
		break;
    //����λ�øı���Ϣ�Ĵ���
	case WM_MOVE:
		if( m_bFullScreen==0 ){
			//��ȡ���ڿͻ�������Ļ����ϵλ��
			GetClientRect( hWnd, &rectWin );
			ClientToScreen( hWnd, (LPPOINT)&rectWin );
			ClientToScreen( hWnd, (LPPOINT)&rectWin + 1 );
		}
		break;
    //�������ߴ����С�ߴ���Ϣ�Ĵ���
	case WM_GETMINMAXINFO:
        //�����Ըı䴰�ڵĴ�С
		if( !Change_Size_Able ) {
            PMINMAXINFO pMM = ( PMINMAXINFO ) lParam;

            //��ȡ��������߿�Ŀ�Ⱥͱ������ĸ߶�
			int FrameW = GetSystemMetrics( SM_CXSIZEFRAME );
			int FrameH = GetSystemMetrics( SM_CYSIZEFRAME );
			int CapH   = GetSystemMetrics( SM_CYCAPTION );

            //�Ѵ����ܱ��ı䵽����С�ߴ��Ϊ��ǰ��ֵ
			pMM->ptMinTrackSize.x = ScreenW + FrameW*2;
			pMM->ptMinTrackSize.y = ScreenH + FrameH*2 + CapH;
            //�������ܱ��ı䵽�����ߴ���Ϊ����С�ߴ���ͬ��ֵ
			pMM->ptMaxTrackSize.x = pMM->ptMinTrackSize.x;
			pMM->ptMaxTrackSize.y = pMM->ptMinTrackSize.y;
		}
        else {
            PMINMAXINFO pMM = ( PMINMAXINFO ) lParam;

            //���ƴ����ܱ��ı䵽����С�ߴ�
            pMM->ptMinTrackSize.x = 100;
            pMM->ptMinTrackSize.y = 150;
        }
		break;
    //�رմ�����Ϣ�Ĵ���
	case WM_CLOSE:
	    DestroyWindow( hWnd );
		break;
    //���ٴ�����Ϣ�Ĵ���
    case WM_DESTROY:
	    PostQuitMessage( 0 );
        break;
    }

	//����ȱʡ��Ϣ�������
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*********************************************************
*	������: RunDefault(...) ����CApplication��ĳ�Ա
*	����: ��������Ϣѭ��
*       ��Ȩ���� jk&ljq, 2010
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
