/*-------------------------------------------------
|                   main.cpp
|                    ������
|            (c)  jk&ljq, 2010
---------------------------------------------------*/

#include <windows.h>
#include "resource.h"
#include "CApplication.h"
#include "CEdit.h"

CApplication    App;
CEdit           Edit;
HWND            hWnd = NULL;
BOOL            bFullScr = FALSE;
LONG            ScreenW = 800;
LONG            ScreenH = 600;

/******************************************
* ���ó������ԶԻ������Ϣ����
*******************************************/
BOOL CALLBACK NewSceneDlgProc( HWND hDlg,
                               UINT message,
                               WPARAM wParam,
                               LPARAM lParam);
/******************************************
* ���ڶԻ������Ϣ����
*******************************************/
BOOL CALLBACK AboutDlgProc( HWND hDlg,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);
/******************************************
* ��������Ϣ����
*******************************************/
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam )
{
    static HINSTANCE hIns;

    switch( uMsg )
    {
    case WM_CREATE:
        //��ȡ����ʵ�����
        hIns=((LPCREATESTRUCT)lParam)->hInstance;
        return 0;
        //�������ÿ����������ԶԻ���
    case WM_COMMAND:
        //>>>>> �˵������� >>>>>>
        switch (LOWORD (wParam))
        {       
        case ID_NEW_GRASSGROUND://�½�һ���ݵص������͵ĳ���
            Edit.Set_GRNKind(1);
            DialogBox( hIns,MAKEINTRESOURCE(IDD_SETSCENE),hWnd, (DLGPROC)NewSceneDlgProc);
            break;       
        case ID_NEW_MUDDYGROUND://�½�һ����ص������͵ĳ���
            Edit.Set_GRNKind(0);
            DialogBox( hIns,MAKEINTRESOURCE(IDD_DIALOG),hWnd, (DLGPROC)NewSceneDlgProc);
            break;       
        case ID_EDIT_WITCHGRASS1://�ݵر༭����
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(6);
            break;
		case ID_EDIT_WITCHGRASS2:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(7);
            break;
		case ID_EDIT_WITCHGRASS3:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(8);
            break;
		case ID_EDIT_WITCHGRASS4:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(9);
            break;
		case ID_EDIT_WITCHGRASS5:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(10);
            break;       
        case ID_EDIT_WITCHMUD1://����ر༭����
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(11);
            break;
		case ID_EDIT_WITCHMUD2:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(12);
            break;
        case ID_EDIT_WITCHMUD3:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(13);
            break;
		case ID_EDIT_WITCHMUD4:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(14);
            break;
		case ID_EDIT_WITCHMUD5:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(15);
            break;
		case ID_EDIT_GRASSTOMUD1://����
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(16);
            break;
		case ID_EDIT_GRASSTOMUD2:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(17);
            break;
		case ID_EDIT_GRASSTOMUD3:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(18);
            break;
		case ID_EDIT_GRASSTOMUD4:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(19);
            break;
		case ID_EDIT_GRASSTOMUD5:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(20);
            break;
		case ID_EDIT_GRASSTOMUD6:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(21);
            break;
		case ID_EDIT_GRASSTOMUD7:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(22);
            break;
		case ID_EDIT_GRASSTOMUD8:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(23);
            break;
		case ID_EDIT_GRASSTOMUD9:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(24);
            break;
		case ID_EDIT_GRASSTOMUD10:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(25);
            break;
		case ID_EDIT_GRASSTOMUD11:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(26);
            break;
		case ID_EDIT_GRASSTOMUD12:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(27);
            break;
		case ID_EDIT_GRASSTOMUD13:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(28);
            break;
		case ID_EDIT_GRASSTOMUD14:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(29);
            break;
		case ID_EDIT_GRASSTOMUD15:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(30);
            break;
		case ID_EDIT_GRASSTOMUD16:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(31);
            break;
		case ID_EDIT_GRASSTOMUD17:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(32);
            break;
		case ID_EDIT_GRASSTOMUD18:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(33);
            break;
		case ID_EDIT_GRASSTOMUD19:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(34);
            break;
		case ID_EDIT_GRASSTOMUD20:
            Edit.Set_lEditState(0);
            Edit.Set_GRNKind(35);
            break;
        case ID_EDIT_TREE1://�༭��ľ����
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(47);
            break;
		case ID_EDIT_TREE2:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(48);
            break;
		case ID_EDIT_TREE3:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(49);
            break;
		case ID_EDIT_TREE4:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(50);
            break;
		case ID_EDIT_TREE5:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(51);
            break;
		case ID_EDIT_TREE6:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(52);
            break;
		case ID_EDIT_TREE7:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(53);
            break;
		case ID_EDIT_TREE8:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(54);
            break;
		case ID_EDIT_TREE9:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(55);
            break;
		case ID_EDIT_TREE10:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(56);
            break;
		case ID_EDIT_TREE11:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(57);
            break;
		case ID_EDIT_TREE12:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(58);
            break;
		case ID_EDIT_TREE13:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(59);
            break;
		case ID_EDIT_TREE14:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(60);
            break;
		case ID_EDIT_TREE15:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(61);
            break;
		case ID_EDIT_TREE16:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(62);
            break;
		case ID_EDIT_TREE17:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(63);
            break;
		case ID_EDIT_TREE18:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(64);
            break;
		case ID_EDIT_TREE19:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(65);
            break;
		case ID_EDIT_TREE20:
            Edit.Set_lEditState(1);
			Edit.Set_GRNKind(66);
            break;
        case ID_SETMY_HUM://��������
            Edit.setMyRace(0);
            break;
		case ID_SETMY_ORC:
            Edit.setMyRace(1);
            break;
		case ID_SETAI_HUM:
            Edit.setAiRace(0);
            break;
		case ID_SETAI_ORC:
            Edit.setAiRace(1);
            break;
		case ID_SET_I://�༭�Լ�
            Edit.setIorAI(0);
            break;
		case ID_SET_AI://�༭AI
            Edit.setIorAI(1);
            break;
		case ID_SET_BUILDING1://�༭����
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(0);
            break;
		case ID_SET_BUILDING2://�༭����
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(1);
            break;
		case ID_SET_BUILDING3://�༭����
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(2);
            break;
		case ID_SET_BUILDING4://�༭����
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(3);
            break;
		case ID_SET_BUILDING5://�༭����
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(4);
            break;
		case ID_SET_PEOPLE1://�༭��
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(0);
            break;
		case ID_SET_PEOPLE2://�༭��
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(1);
            break;
		case ID_SET_PEOPLE3://�༭��
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(2);
            break;
		case ID_SET_PEOPLE4://�༭��
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(3);
            break;
		case ID_SET_PEOPLE5://�༭��
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(4);
            break;
		case ID_EDIT_GOLDMINE://�༭���
			Edit.Set_lEditState(4);
			Edit.setNeutral_Kind(0);
            break;
        case ID_OPEN://�򿪳����ļ�
            //��δ�����������޸�
            if( Edit.Get_lSaveState()==2 )
            {
                int yes = MessageBox( hWnd,
                    TEXT("�������޸ģ���δ����\n")
                    TEXT("������ٴ��ļ���\n"),
                    TEXT("Warning!"),MB_YESNO);
                if( yes==IDYES )//ѡ����"��"
                    Edit.SaveSceneData( FALSE );
            }
            //���볡���ļ�
             Edit.LoadSceneData();
            break;
        case ID_SAVE://�Ե�ǰ�ļ������泡������
            Edit.SaveSceneData( FALSE );
            break;
        case ID_SAVEAS://���û�ѡ���ļ�������
            Edit.SaveSceneData( TRUE );
            break;
        case ID_EXIT://�˳��༭��
            //��δ�����������޸�
            if( Edit.Get_lSaveState()==2 )
            {
                int yes = MessageBox( hWnd,
                    TEXT("�������޸ģ���δ����\n")
                    TEXT("�Ƿ񱣴���˳���\n"),
                    TEXT("Warning!"),MB_YESNO);
                if( yes==IDYES )//ѡ����"��"�򱣴�
                    Edit.SaveSceneData( FALSE );
            }
            SendMessage( hWnd,WM_CLOSE,0,0);
            break;
        case ID_ABOUT:
            //���������������öԻ���
            DialogBox( hIns,
                MAKEINTRESOURCE(IDD_ABOUT),
                hWnd, (DLGPROC)AboutDlgProc);
            break;
        }
        break;
    case WM_PAINT:
        if(!bFullScr)
            Edit.UpdateScreen(&App.Get_rectWin());
        break;
    }
    return App.MsgProc( hWnd,uMsg,wParam,lParam );
}
/******************************************
* WinMain�������
*******************************************/
int WINAPI WinMain(	HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow)
{
    //>>>>>>>>>> CApplication��ʹ�� >>>>>>>>>>
    App.isFullScreen( bFullScr );
    // �ͻ�������ɫ�Ǻ�ɫ
    App.SetClientBKColor( GRAY_BRUSH );
    // �����Զ���ͼ��
    App.Set_hIcon( hInstance, IDI_WAR );
    // �����Զ�����
    //App.Set_hCursor( hInstance, IDC_MYCURSOR );
    // ����˵�
    App.Set_hMenu( hInstance, IDR_MENU );
    // ��ֹ�ı䴰�ڳߴ�
    App.ChangeSizeAble( FALSE );
    // ���ô��ڴ�С
    App.SetScreenW( ScreenW );
    App.SetScreenH( ScreenH );

    // ��������
    App.CreateWin( WinProc,
                   hInstance,
                   "��ͼ�༭��",
                   WS_OVERLAPPEDWINDOW &
                   ~WS_MAXIMIZEBOX );
    hWnd = App.Get_hWnd();

    //>>>>>> ��ʼ����Ϸ�� >>>>>>>>>
    Edit.Set_hWnd( hWnd );
    Edit.Set_bFullScr( bFullScr );
    Edit.Set_ScreenW( ScreenW );
    Edit.Set_ScreenH( ScreenH );
    Edit.InitDirectX( hWnd );

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
            Edit.GetInputDeviceData( lpRect );
            if( Edit.EditProcess() )
            {
                //������Ļ
                Edit.UpdateScreen( lpRect );
            }
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
/******************************************
* ���ó������ԶԻ������Ϣ����
*******************************************/
BOOL CALLBACK NewSceneDlgProc(HWND hDlg,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG://��ʼ������������ֵ
        SetDlgItemInt(hDlg,IDC_EDITWIDTH,1920U,0);
        SetDlgItemInt(hDlg,IDC_EDITHIGH,1920U,0);
        SetDlgItemInt(hDlg,IDC_EDITSCENEID,0U,0);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case IDC_OK://������"OK"��ť
            LONG SceneW, SceneH;
            //���������ֵ
            SceneW = GetDlgItemInt(hDlg,
                IDC_EDITWIDTH,NULL,FALSE);
            SceneH = GetDlgItemInt(hDlg,
                IDC_EDITHIGH,NULL,FALSE);
            //���ó���������
            Edit.Set_SceneW( SceneW );
            Edit.Set_SceneH( SceneH );
            //�������õ�ֵ�½�һ������
            Edit.NewScene();
            //�رնԻ���
            EndDialog (hDlg, 0);
            return TRUE;
        case IDC_CANNCEL://������ȡ����ť
            //�ָ���ǰѡ��ĵ�������
            Edit.Restore_oldGRNKind();
            //�رնԻ���
            EndDialog (hDlg, 0);
            return TRUE;
        }
        break;
    }
    //û�д����κ���Ϣ���򷵻�FALSEֵ
    return FALSE;
}

/******************************************
* "����"�Ի������Ϣ����
*******************************************/
BOOL CALLBACK AboutDlgProc(HWND hDlg,
                           UINT message,
                           WPARAM wParam,
                           LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case IDC_OK://������"OK"��ť
            EndDialog (hDlg, 0);
            return TRUE;
        }
        break;
    }
    //û�д����κ���Ϣ���򷵻�FALSEֵ
    return FALSE;
}

