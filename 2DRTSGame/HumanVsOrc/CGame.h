/*-------------------------------------------
|              CGame.h
|          游戏类成员声明
|            (c)  jk&ljq
---------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "CDDraw.h"
#include "CDInput.h"
#include "CDSound.h"
#include "CText.h"
#include "StructDefine.h"

class CGame
{
private:
    CDDraw      DDraw;//DirectDraw
    CDInput     Input;//输入设备对象
    CDSound     Sound;//声音对象
    CGetPath    GPa;//路径对象
    CText       Text;//字符串对象
	//将来从文件读进来
	LONG GroundRC[100];
	//LPDIRECTDRAWSURFACE7 Building[2][5];//0:Hum 1:Orc	
	//LPDIRECTDRAWSURFACE7 People[2][5];//0:Hum 1:Orc
	LPDIRECTDRAWSURFACE7 Win_Lose[2][2];//0:Hum 1:Orc
    LPDIRECTDRAWSURFACE7 lpDDSGroundRC;//地面源表面	
	LPDIRECTDRAWSURFACE7 lpDDSIcons;//图标源表面	
    LPDIRECTDRAWSURFACE7 lpDDSSelect_Row;//选择框源表面
	LPDIRECTDRAWSURFACE7 lpDDSSelect_Col;//选择框表面
	LPDIRECTDRAWSURFACE7 Neutral[2];//中立
    LPDIRECTDRAWSURFACE7 Interface[8];//界面
	LPDIRECTDRAWSURFACE7 MissileSource[4];//导弹
	Unit_Common Building_Common[2][5];//0:Hum 1:Orc
	Unit_Common People_Common[2][5];//0:Hum 1:Orc
	Unit_Common  Neutral_Common[2];//中立
	Unit_Common  Interface_Common[8];//界面
	Unit_Common  Missile_Common[4];//导弹
    countNum Count;//定义计数器
	Player player[2];//0:自己  1:AI
	vector<SelectUnit> select;//鼠标选中单位
	vector<SelectUnit> select_frame;//鼠标拖框选中单位
	vector<SelectUnit> select_1;//鼠标选中单位
	vector<SelectUnit> select_2;//鼠标选中单位
	vector<SelectUnit> select_3;//鼠标选中单位
	vector<Unit> Neutral_Unit;//中立单位
	vector<Missile> MissileQueue;//导弹
	vector<Unit> DeadQueue;//死亡单位

    LPDIRECTDRAWSURFACE7 lpDDSHeader;//开头画面
    LPDIRECTDRAWSURFACE7 lpDDSButtonUp;//按钮凸起
    LPDIRECTDRAWSURFACE7 lpDDSButtonDown;//按钮凹下
	LPDIRECTDRAWSURFACE7 lpDDSColor[2];//颜色 0:绿 1:红

    HWND        hWnd;//程序句柄
    BOOL        bFullScr;//屏幕模式
    LONG        ScreenW;//客户区的宽度
    LONG        ScreenH;//客户区的高度
    LONG        ColorBits;//图像颜色深度

    LONG        MaxWObject;//对象矩形图片的最大宽度
    LONG        MaxHObject;//对象矩形图片的最大高度

	MAP			map;//地图
    POINT       ScrPos;//屏幕的位置
    LONG        lAppState;//应用程序状态
    DWORD       Time;//当前时间
	DWORD       oldTime;//金币时间
	DWORD       game_start_time;//游戏开始时间
	//是否需要更新后台及屏幕内容
    LONG        lUpdateScreen;
    LONG        lSaveState;//保存状态
    
    LONG        GRNKind;//选择的地面类型
    LONG        oldGRNKind;//旧地面类型

	//0:HUM 1:ORC
	LONG		IorAI;//0:I 1:AI
	LONG		kind_building;//建筑种类
	LONG		kind_people;//人种类
	LONG		kind_neutral;//中立种类
	LONG		UI;//统一标识符
	LONG		flag;

	TCHAR       sceFilePath[MAX_PATH];//场景文件名

    PKEY        KeyKB;//键盘状态指针
    PKEY        KeyMouse;//鼠标状态指针(屏幕中的)
    POINT       MouseScePos;//鼠标转化到场景中的位置

	POINT		start_select;
	POINT		end_select;
	BOOL		select_draw;
	Circle_Draw circle_draw;


    
public:
    CGame(void);
    ~CGame();

    //>>>>>>> 设置成员变量 >>>>>>>>>>>

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
	void Initial_count(void)
	{//初始化计数器
		for(int i=0;i<5;i++){
		Count.playerSolProduce[i]=0;
		Count.playerSolLost[i]=0;
		Count.playerBuiLost[i]=0;
		Count.AISolProduce[i]=0;
		Count.AISolLost[i]=0;
		Count.AIBuiLost[i]=0;
		}
	
	}
    void Set_MaxHObject( LONG MaxHeightObject )
    {//设置对象图片的最大高度
        MaxHObject = MaxHeightObject;
    }

    //>>>>>>> 辅助成员函数 >>>>>>>>>>>
    //获取一个矩形
    LONG DoFor( LONG lIndex,
                LONG lEndNum,
                LONG lStartNum,LONG Direct);
    INT  GetDirection( POINT p1, POINT p2 );
    RECT GetActionRect( LONG lIndex,
                        LONG lWidth,
                        LONG lHeight,
                        LONG lCount);

    //>>>> 初始化DirectX成员函数 >>>>>>>
    //载入游戏的图像数据
    void LoadImageSurf( void );
    //初始化DirectX(外部通过调用它来初始化游戏)
    void InitDirectX( HWND _hWnd );

    //>>>>> 游戏开头画面相关处理函数 >>>>>>
    //画按钮
    void DrawButton( LPDIRECTDRAWSURFACE7 Surf,
                     WORD state, LPRECT lpRect );
    //游戏开头画面
    void Header( void );

	//>>>>>> 对象编辑相关成员 >>>>>>>
    //删除对象
    //void DeleteObject( LONG uniform_temp );
	void DeleteSelect(LONG uniform_temp);
    //新加对象
    void NewObject( void );
    //编辑对象
    LONG ObjectEdit( void );

    void MakeScene( void );
    //删除指定场景的数据
    void DeleteSceneData( MAP map );
	//>>>>>>> 文件输入输出相关成员 >>>>>>>
	void SaveGroundDesc( FILE *file );
    void LoadGroundDesc( FILE *file );
    void SaveObject( FILE *file );
    void LoadObject( FILE *file);
    void SaveTerrainSquare( FILE *file);
    void LoadTerrainSquare( FILE *file);
    void SaveSceneData( BOOL bSaveAs );
    void LoadSceneData( void );
	BOOL GetSceneFileName( DWORD dwFlag );
    //>>>>>> 恢复需要恢复的成员变量>>>>>
    void RestoreVariable( void );
	//根据鼠标位置改变屏幕的位置
    LONG CheckScrPos( void );



    //>>>>>> 生成地面 >>>>>>>>>>
    void MakeGround( void );
    //>>>>>> 对象显示链表的更新 >>>>>>>>>
    //>>> 角色、怪物、地面对象和怪物选择圈的显示>>>
    void MakeEntity( void );
    //>>> 角色死后的处理 >>>>>>>
    void DeadProcess( void );
	void CheckMessage( void );
	void CheckAIMessage( void );
	void MessageProcess( void );

    //>>>> 游戏的进程(外部通过调用它来运行游戏)>>>
    void GameProcess( void );
    //>>>> 获取输入设备状态函数 >>>>>
    void GetInputDeviceData( LPRECT lpClientRect );
    //>>>> 更新屏幕成员函数 >>>>>>>>
    void UpdateScreen( LPRECT lpClientRect );
	//设置地形方块
	void SetRect( LONG left, LONG top,LONG Width, LONG Height,BOOL data  );
	//检查地形方块
	BOOL CheckRect( LONG left, LONG top,LONG Width, LONG Height,BOOL data );

	SelectUnit returnSelect(POINT pos);
	SelectUnit returnSelect1(POINT pos);
	SelectUnit AdjustUI(SelectUnit ui);
	void AdjustSelect();
	void AdjustAfterDelete();
	Unit_Common *returnUnitCommon(SelectUnit ui);
	BOOL CheckUI(SelectUnit ui);
	Unit *returnUnit(SelectUnit ui);
	void RectangleFrame(POINT a,POINT b);
	BOOL setDamage(SelectUnit ui,LONG damage,LONG _attack_kind);
	LONG returnDamage(LONG damage,LONG defense,LONG src_attack_kind,LONG des_defense_kind);
	void selectInFrame(POINT a,POINT b);
	void playsound(SelectUnit ui,LONG index);
	LONG  soundDis(POINT a,POINT b);
	bool  soundIn(POINT a);
	POINT OneStep( POINT pos,LONG direct );
	void CheckWin();
	void DrawIcon(POINT pos,LONG i);
	LONG CheckIcon(POINT a);
	void DrawBloodBar(LONG startx,LONG starty,LONG length,LONG height,FLOAT rate);
	POINT CalArmyBuildingCenterPos(LONG i);
	void setStrategy(LONG i,LONG j);
};