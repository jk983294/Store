/*-------------------------------------------------
|                   main.cpp
|                    主程序
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
* 设置场景属性对话框的消息处理
*******************************************/
BOOL CALLBACK NewSceneDlgProc( HWND hDlg,
                               UINT message,
                               WPARAM wParam,
                               LPARAM lParam);
/******************************************
* 关于对话框的消息处理
*******************************************/
BOOL CALLBACK AboutDlgProc( HWND hDlg,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);
/******************************************
* 主窗口消息处理
*******************************************/
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam )
{
    static HINSTANCE hIns;

    switch( uMsg )
    {
    case WM_CREATE:
        //获取程序实例句柄
        hIns=((LPCREATESTRUCT)lParam)->hInstance;
        return 0;
        //创建设置开关区域属性对话框
    case WM_COMMAND:
        //>>>>> 菜单的命令 >>>>>>
        switch (LOWORD (wParam))
        {       
        case ID_NEW_GRASSGROUND://新建一个草地地面类型的场景
            Edit.Set_GRNKind(1);
            DialogBox( hIns,MAKEINTRESOURCE(IDD_SETSCENE),hWnd, (DLGPROC)NewSceneDlgProc);
            break;       
        case ID_NEW_MUDDYGROUND://新建一个泥地地面类型的场景
            Edit.Set_GRNKind(0);
            DialogBox( hIns,MAKEINTRESOURCE(IDD_DIALOG),hWnd, (DLGPROC)NewSceneDlgProc);
            break;       
        case ID_EDIT_WITCHGRASS1://草地编辑地面
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
        case ID_EDIT_WITCHMUD1://以泥地编辑地面
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
		case ID_EDIT_GRASSTOMUD1://过度
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
        case ID_EDIT_TREE1://编辑树木操作
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
        case ID_SETMY_HUM://设置种族
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
		case ID_SET_I://编辑自己
            Edit.setIorAI(0);
            break;
		case ID_SET_AI://编辑AI
            Edit.setIorAI(1);
            break;
		case ID_SET_BUILDING1://编辑建筑
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(0);
            break;
		case ID_SET_BUILDING2://编辑建筑
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(1);
            break;
		case ID_SET_BUILDING3://编辑建筑
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(2);
            break;
		case ID_SET_BUILDING4://编辑建筑
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(3);
            break;
		case ID_SET_BUILDING5://编辑建筑
			Edit.Set_lEditState(2);
			Edit.setBuildingKind(4);
            break;
		case ID_SET_PEOPLE1://编辑人
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(0);
            break;
		case ID_SET_PEOPLE2://编辑人
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(1);
            break;
		case ID_SET_PEOPLE3://编辑人
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(2);
            break;
		case ID_SET_PEOPLE4://编辑人
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(3);
            break;
		case ID_SET_PEOPLE5://编辑人
			Edit.Set_lEditState(3);
			Edit.setPeopleKind(4);
            break;
		case ID_EDIT_GOLDMINE://编辑金矿
			Edit.Set_lEditState(4);
			Edit.setNeutral_Kind(0);
            break;
        case ID_OPEN://打开场景文件
            //还未保存所做的修改
            if( Edit.Get_lSaveState()==2 )
            {
                int yes = MessageBox( hWnd,
                    TEXT("所做的修改，还未保存\n")
                    TEXT("保存后再打开文件？\n"),
                    TEXT("Warning!"),MB_YESNO);
                if( yes==IDYES )//选择了"是"
                    Edit.SaveSceneData( FALSE );
            }
            //载入场景文件
             Edit.LoadSceneData();
            break;
        case ID_SAVE://以当前文件名保存场景数据
            Edit.SaveSceneData( FALSE );
            break;
        case ID_SAVEAS://由用户选择文件名保存
            Edit.SaveSceneData( TRUE );
            break;
        case ID_EXIT://退出编辑器
            //还未保存所做的修改
            if( Edit.Get_lSaveState()==2 )
            {
                int yes = MessageBox( hWnd,
                    TEXT("所做的修改，还未保存\n")
                    TEXT("是否保存后退出？\n"),
                    TEXT("Warning!"),MB_YESNO);
                if( yes==IDYES )//选择了"是"则保存
                    Edit.SaveSceneData( FALSE );
            }
            SendMessage( hWnd,WM_CLOSE,0,0);
            break;
        case ID_ABOUT:
            //创建场景属性设置对话框
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
* WinMain程序入口
*******************************************/
int WINAPI WinMain(	HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow)
{
    //>>>>>>>>>> CApplication的使用 >>>>>>>>>>
    App.isFullScreen( bFullScr );
    // 客户区背景色是黑色
    App.SetClientBKColor( GRAY_BRUSH );
    // 载入自定义图标
    App.Set_hIcon( hInstance, IDI_WAR );
    // 载入自定义光标
    //App.Set_hCursor( hInstance, IDC_MYCURSOR );
    // 载入菜单
    App.Set_hMenu( hInstance, IDR_MENU );
    // 禁止改变窗口尺寸
    App.ChangeSizeAble( FALSE );
    // 设置窗口大小
    App.SetScreenW( ScreenW );
    App.SetScreenH( ScreenH );

    // 创建窗口
    App.CreateWin( WinProc,
                   hInstance,
                   "地图编辑器",
                   WS_OVERLAPPEDWINDOW &
                   ~WS_MAXIMIZEBOX );
    hWnd = App.Get_hWnd();

    //>>>>>> 初始化游戏类 >>>>>>>>>
    Edit.Set_hWnd( hWnd );
    Edit.Set_bFullScr( bFullScr );
    Edit.Set_ScreenW( ScreenW );
    Edit.Set_ScreenH( ScreenH );
    Edit.InitDirectX( hWnd );

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
            Edit.GetInputDeviceData( lpRect );
            if( Edit.EditProcess() )
            {
                //更新屏幕
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
* 设置场景属性对话框的消息处理
*******************************************/
BOOL CALLBACK NewSceneDlgProc(HWND hDlg,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG://初始化三个输入框的值
        SetDlgItemInt(hDlg,IDC_EDITWIDTH,1920U,0);
        SetDlgItemInt(hDlg,IDC_EDITHIGH,1920U,0);
        SetDlgItemInt(hDlg,IDC_EDITSCENEID,0U,0);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case IDC_OK://单击了"OK"按钮
            LONG SceneW, SceneH;
            //获得输入框的值
            SceneW = GetDlgItemInt(hDlg,
                IDC_EDITWIDTH,NULL,FALSE);
            SceneH = GetDlgItemInt(hDlg,
                IDC_EDITHIGH,NULL,FALSE);
            //设置场景的属性
            Edit.Set_SceneW( SceneW );
            Edit.Set_SceneH( SceneH );
            //根据设置的值新建一个场景
            Edit.NewScene();
            //关闭对话框
            EndDialog (hDlg, 0);
            return TRUE;
        case IDC_CANNCEL://单击了取消按钮
            //恢复以前选择的地面类型
            Edit.Restore_oldGRNKind();
            //关闭对话框
            EndDialog (hDlg, 0);
            return TRUE;
        }
        break;
    }
    //没有处理任何消息，则返回FALSE值
    return FALSE;
}

/******************************************
* "关于"对话框的消息处理
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
        case IDC_OK://单击了"OK"按钮
            EndDialog (hDlg, 0);
            return TRUE;
        }
        break;
    }
    //没有处理任何消息，则返回FALSE值
    return FALSE;
}

