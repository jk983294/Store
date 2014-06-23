/*-------------------------------------------
|              CEdit.h
|          编辑类成员声明
|            (c)  jk&ljq
---------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "CDDraw.h"
#include "CDInput.h"
#include "CText.h"
#include "StructDefine.h"
#include <vector>
using namespace std;

#define WM_CREATESETSWITCHTRAITDIALOG  WM_USER

class CEdit
{
private:
    CDDraw      DDraw;//DirectDraw
    CDInput     Input;//输入设备对象
	CGetPath    GPa;//路径对象
    CText       Text;//字符串对象
	//将来从文件读进来
	LONG GroundRC[100];
	//LPDIRECTDRAWSURFACE7 Building[2][10];//0:Hum 1:Orc
	//LPDIRECTDRAWSURFACE7 People[2][10];//0:Hum 1:Orc	
    LPDIRECTDRAWSURFACE7 lpDDSGroundRC;//地面源表面	
    LPDIRECTDRAWSURFACE7 lpDDSSelect_Row;//选择框源表面
	LPDIRECTDRAWSURFACE7 lpDDSSelect_Col;//选择框表面
	LPDIRECTDRAWSURFACE7 Neutral[10];//中立
	Unit_Common People_Common[2][10];//0:Hum 1:Orc
	Unit_Common Building_Common[2][10];//0:Hum 1:Orc
	Unit_Common  Neutral_Common[10];//中立
	Player player[2];//0:自己  1:AI
	vector<Unit> Neutral_Unit;//中立单位
	vector<SelectUnit> select;//鼠标选中单位	

    HWND        hWnd;//程序句柄
    BOOL        bFullScr;//屏幕模式
    LONG        ScreenW;//客户区的宽度
    LONG        ScreenH;//客户区的高度
    LONG        ColorBits;//图像颜色深度

    LONG        MaxWObject;//对象矩形图片的最大宽度
    LONG        MaxHObject;//对象矩形图片的最大高度

    MAP			map;//地图
    POINT       ScrPos;//屏幕的位置
    LONG        lAppState;//程序状态
    //是否需要更新后台及屏幕内容
    LONG        lUpdateScreen;
    LONG        lSaveState;//保存状态
    LONG        lEditState;//编辑状态
    DWORD       Time;//当前时间
    LONG        GRNKind;//选择的地面类型
    LONG        oldGRNKind;//旧地面类型
	//0:HUM 1:ORC
	LONG		IorAI;//0:I 1:AI
	LONG		kind_building;//建筑种类
	LONG		kind_people;//人种类
	LONG		kind_neutral;//中立种类
	LONG		UI;//统一标识符

    TCHAR       sceFilePath[MAX_PATH];//场景文件名

    PKEY        KeyKB;//键盘状态指针
    PKEY        KeyMouse;//鼠标状态指针(屏幕中的)
    POINT       MouseScePos;//鼠标转化到场景中的位置
	

public:
    CEdit(void);
    ~CEdit();
	
	//Player AI;

    //>>>>>>> 设置成员变量 >>>>>>>>>>>
    //设置应用程序句柄
    void Set_hWnd( HWND hwnd ) {hWnd = hwnd;}
    void Set_bFullScr( BOOL bFullScreen )
    {//设置屏幕模式
        bFullScr = bFullScreen;
    }
    //设置客户区宽度
    void Set_ScreenW( LONG ScrW ) {ScreenW = ScrW;}
    //设置客户区高度
    void Set_ScreenH( LONG ScrH ) {ScreenH = ScrH;}
    void Set_ColorBits( LONG ColorBitCount )
    {//设置颜色深度
        ColorBits = ColorBitCount;
    }
    
    void Set_MaxWObject( LONG MaxWideObject )
    {//设置对象图片的最大宽度
        MaxWObject = MaxWideObject;
    }
    void Set_MaxHObject( LONG MaxHeightObject )
    {//设置对象图片的最大高度
        MaxHObject = MaxHeightObject;
    }
    //设置场景宽度
    void Set_SceneW( LONG SceW )
    {
        //调整场景宽为80的整数倍
        SceW = (SceW/32)*32;
        map.SceneW = SceW;
    }
    //设置场景高度
    void Set_SceneH( LONG SceH )
    {
        //调整场景高为80的整数倍
        SceH = (SceH/32)*32;
        map.SceneH = SceH;
    }
    //设置选择的地面类型
    void Set_GRNKind( LONG grnKind )
    {
        oldGRNKind = GRNKind;
        GRNKind = grnKind;
    }
	void Restore_oldGRNKind()
	{
		GRNKind = oldGRNKind;
	}
    void setMyRace( LONG kind )
    {
        player[0].race= kind;
    }
	void setIorAI( LONG kind )
    {
        IorAI = kind;
    }
	void setAiRace( LONG kind )
    {
        player[1].race = kind;
    }
	void setBuildingKind( LONG a )
    {
        kind_building = a;
    }
	void setPeopleKind( LONG a )
    {
        kind_people = a;
    }
	void setNeutral_Kind( LONG a )
    {
        kind_neutral = a;
    }
    //设置选择的地面类型
    void Set_lEditState( LONG EditState )
    {
        lEditState = EditState;
        lUpdateScreen = 1;
    }

    //>>>>>>> 辅助成员函数 >>>>>>>>>>>
    //获取一个矩形
    RECT GetRect(LONG left,LONG top,
                 LONG right,LONG bottom);

    //>>>> 初始化DirectX成员函数 >>>>>>>
    //载入游戏的图像数据
    void LoadImageSurf( void );
    //初始化DirectX(外部通过调用它来初始化游戏)
    void InitDirectX( HWND _hWnd );

    void NewScene( void );
    void DeleteSceneData( MAP map );
    //>>>>>>> 文件输入输出相关成员 >>>>>>>
    BOOL GetSceneFileName( DWORD dwFlag );
    void SaveGroundDesc( FILE *file );
    void LoadGroundDesc( FILE *file );
    void SaveObject( FILE *file );
    void LoadObject( FILE *file);
    void SaveTerrainSquare( FILE *file);
    void LoadTerrainSquare( FILE *file);
    void SaveSceneData( BOOL bSaveAs );
    void LoadSceneData( void );

    //>>>>>> 恢复需要恢复的成员变量>>>>>
    void RestoreVariable( void );

    //根据鼠标位置改变屏幕的位置
    LONG CheckScrPos( void );
    //>>>>>> 地面地编辑相关成员 >>>>>>>>
    //根据指定地面描述获取对应地面方块的源位置
    //RECT GetSrcRect( GROUNDDESC *grn,BOOL bGetNew);
    //根据指定的地面方块序号获得对应地面方块指针
    //GROUNDDESC * GetGrnPointer( LONG lN );
    //地面地编辑
    LONG GroundEdit( void );
	//设置地形方块
	void SetRect( LONG left, LONG top,LONG Width, LONG Height,WORD data  );
	//检查地形方块
	BOOL CheckRect( LONG left, LONG top,LONG Width, LONG Height,WORD data );

    //>>>>>> 对象编辑相关成员 >>>>>>>
    //删除对象
    void DeleteObject( LONG uniform_temp );
	void DeleteSelect(LONG uniform_temp);
    //新加对象
    void NewObject( void );
    //编辑对象
    LONG ObjectEdit( void );

    //>>>>>> 生成地面 >>>>>>>>>>
    void MakeGround( void );
    //>>>>>>> 地面上的以及文字的显示 >>>>>>>
    void MakeEntity( void );

    //>>>>>> 编辑器的进程 >>>>>>>>
    BOOL EditProcess( void );
    //>>>> 获取输入设备状态函数 >>>>>
    void GetInputDeviceData( LPRECT lpClientRect );
    //>>>> 更新屏幕成员函数 >>>>>>>>
    void UpdateScreen( LPRECT lpClientRect );

    //>>>> 获得保存状态 >>>>>>>
    LONG Get_lSaveState( void ){return lSaveState;}

};