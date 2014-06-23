#include <windows.h>
#include <stdio.h>
#include "CGame.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
inline RECT GetRscRect( LONG lIndex, LONG lWidth,LONG lHeight, LONG lCount );
inline RECT GetRect(LONG left,LONG top,LONG right,LONG bottom);
inline BOOL inRect(RECT a,POINT b);


#define Direction_Down         0    //下
#define Direction_LeftDown     1    //左下
#define Direction_Left         2    //左
#define Direction_LeftUp       3    //左上
#define Direction_Up           4    //上
#define Direction_RightUp      5    //右上
#define Direction_Right        6    //右
#define Direction_RightDown    7    //右下
#define blet_time 800
#define goldup_time 1000
#define gold_limit 100000
#define population_limit 30

LPCSTR Bui[2][5]={{"要塞","步兵兵营","骑兵兵营","魔法之塔","弓箭手营地"},
{"主城","步兵征集所","亡灵召唤地","食人魔居所","巨魔小屋"}};
LPCSTR Sol[2][5]={{"农民","人族步兵","骑兵","魔法师","弓箭手"},
		{"苦工","兽族步兵","死亡骑士","食人魔法师","巨魔掷斧者"}};
LPCSTR Defense_Kind[4]={"轻甲","中甲","重甲","加强型"};//0:轻甲 1:中甲 2:重甲 3:加强型城甲
LPCSTR Attack_Kind[4]={"普通","穿刺","魔法","混乱"};//0:普通 1:穿刺 2:魔法 3:混乱
LONG Strategy[3][5]={{10,8,2,8,8},{5,6,4,6,6},{2,4,6,8,8}};
RECT SelectRECT[9];


/************************************************************
*   函数名: CGame(...)
*     功能: 构造函数
*       (c) jk&ljq, 2010
*************************************************************/
CGame::CGame( void )
{
    hWnd        = NULL;
    bFullScr    = FALSE;//默认为窗口模式
    ScreenW     = 640;//
    ScreenH     = 480;//默认分辨率640*480
    ColorBits   = 16;//默认颜色深度为16色
    MaxWObject  = 340;//对象矩形图片的最大宽度
    MaxHObject  = 300;//对象矩形图片的最大高度

    lAppState   = 0;//开始状态

	ScrPos.x    = 0;
    ScrPos.y    = 0;
    ZeroMemory( &map, sizeof(MAP) );
    lUpdateScreen = 1;
    lSaveState  = 0;
	game_start_time = 0;
	oldTime = 0;

	ZeroMemory( GroundRC, sizeof(LONG)*100 );
	ZeroMemory( &map, sizeof(MAP) );
	ZeroMemory( &circle_draw, sizeof(Circle_Draw) );
	delete []map.Grn;
	ZeroMemory( &player[0], sizeof(Player) );
	ZeroMemory( &player[1], sizeof(Player) );
	for(LONG i=0;i<2;i++){
		player[i].Building.clear();
		player[i].People.clear();
	}
	player[0].race = 0;//默认人族
	player[1].race = 1;//默认兽族
	UI=0;
    KeyKB           = NULL;
    KeyMouse        = NULL; 

	i=0;
	ifstream inf;							//读取地图表面方格定义
	inf.open("data\\Configure\\Map.Conf");
	LONG data;
	while (inf>>data)
	{
		GroundRC[i++]=data;
	}
	inf.close();
	MissileQueue.clear();
	DeadQueue.clear();
	select_1.clear();
	select_2.clear();
	select_3.clear();

	SelectRECT[0] = GetRect(600,10,646,48);
	SelectRECT[1] = GetRect(646,10,692,48);
	SelectRECT[2] = GetRect(692,10,738,48);
	SelectRECT[3] = GetRect(738,10,784,48);
	SelectRECT[4] = GetRect(600,52,646,90);
	SelectRECT[5] = GetRect(646,52,692,90);
	SelectRECT[6] = GetRect(692,52,738,90);
	SelectRECT[7] = GetRect(738,52,784,90);
	SelectRECT[8] = GetRect(600,130,646,168);
}

/************************************************************
*   函数名: ~CGame(...)
*     功能: 析构函数
*       (c) jk&ljq, 2010
*************************************************************/
CGame::~CGame( void )
{
    DeleteSceneData(map);
}

/********************************************************
*  函数名:  GetRect(...)
*  功能:    获取一个矩形结构
*       (c) jk&ljq, 2010
*********************************************************/
RECT GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}
/********************************************************
*  函数名:  GetRect(...)
*  功能:    获取一个矩形结构
*       (c) jk&ljq, 2010
*********************************************************/
BOOL inRect(RECT a,POINT b){
	if(b.x>=a.left && b.x <= a.right && b.y>= a.top && b.y <= a.bottom)
		return TRUE;
	return FALSE;
}
/************************************************************
*   函数名: DoFor(...)
*     功能: 循环变化一组等差整数数列,差值是8
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG CGame::DoFor( LONG lIndex,
                          LONG lEndCount,
                          LONG lStartNum,LONG Direct )
{
    // 不大于倒数第二个数，且不小于第一个数
	if( lIndex<= lEndCount&& lIndex>=lStartNum )
        lIndex += 8;
    else
        lIndex = lStartNum;
	if(lIndex >lEndCount)
		lIndex = Direct+lStartNum;
	return	lIndex;
}
/************************************************************
*   函数名: GetDirection(...)
*     功能: 获得指定起点到指定终点的方向
*       (c) jk&ljq, 2010
*************************************************************/
INT CGame::GetDirection( POINT p1, POINT p2 )
{
    FLOAT   dx,dy,k;
    INT     Direct;
    dx = (FLOAT)(p2.x-p1.x);
    dy = (FLOAT)(p2.y-p1.y);
    k = dy/dx;//斜率
    //横轴正向
    if( dx>0 ) {
        if( dy>0 )
        {//(+,+)第一象限,k>0
            if( k<=0.4142 )//tan(22.5)==0.4142
                Direct = Direction_Right;
            else if(k<=2.4142)//tan(67.5)==2.4142
                Direct = Direction_RightDown;
            else
                Direct = Direction_Down;
        }
        else if( dy<0 )
        {//(+,-)第四象限,k<0
            if( k>-0.4142 )//tan(-22.5)
                Direct = Direction_Right;
            else if( k>-2.4142 )//tan(-67.5)
                Direct = Direction_RightUp;
            else
                Direct = Direction_Up;
        }
        else//在横轴正向上
            Direct = Direction_Right;
    }
    //横轴负向
    else if( dx<0 ) {
        if( dy>0 ) 
        {//(-,+)第二象限,k<0
            if( k>-0.4142 )//tan(-22.5)
                Direct = Direction_Left;
            else if( k>-2.4142 )//tan(-67.5)
                Direct = Direction_LeftDown;
            else
                Direct = Direction_Down;
        }
        else if( dy<0 )
        {//(-,-)第三象限,k>0
            if( k<=0.4142 )//tan(22.5)
                Direct = Direction_Left;
            else if( k<=2.4142 )//tan(67.5)
                Direct = Direction_LeftUp;
            else
                Direct = Direction_Up;
        }
        else//在横轴负向上
            Direct = Direction_Left;
	}
    //在纵轴上
    else{
		if( dy>0 )//在纵轴正向上
            Direct = Direction_Down;
        else//在纵轴负向上
            Direct = Direction_Up;
    }
    return	Direct;
}
/************************************************************
*   函数名: LoadImageSurf(...)
*     功能: 载入图像给表面
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::LoadImageSurf( void )
{
    //载入开头画面
    DDraw.CreateBMPSurface(lpDDSHeader,0,0,"data\\graphics\\ui\\Menu_background_without_title.bmp",DDSCAPS_SYSTEMMEMORY);
	//载入人族胜利画面
    DDraw.CreateBMPSurface(Win_Lose[0][0],0,0,"data\\graphics\\ui\\human\\victory.bmp",DDSCAPS_SYSTEMMEMORY);
	//载入人族失败画面
    DDraw.CreateBMPSurface(Win_Lose[0][1],0,0,"data\\graphics\\ui\\human\\defeat.bmp",DDSCAPS_SYSTEMMEMORY);
    //载入兽族胜利画面
    DDraw.CreateBMPSurface(Win_Lose[1][0],0,0,"data\\graphics\\ui\\orc\\victory.bmp",DDSCAPS_SYSTEMMEMORY);
	//载入兽族失败画面
    DDraw.CreateBMPSurface(Win_Lose[1][1],0,0,"data\\graphics\\ui\\orc\\defeat.bmp",DDSCAPS_SYSTEMMEMORY);
    //载入按钮
    if( DDraw.CreateBMPSurface(lpDDSButtonUp,0,0,"data\\graphics\\ButtonUP.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( lpDDSButtonUp,CLR_INVALID );
    if( DDraw.CreateBMPSurface(lpDDSButtonDown,0,0,"data\\graphics\\ButtonDown.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( lpDDSButtonDown,CLR_INVALID );
	if( DDraw.CreateBMPSurface(circle_draw.lpCircle[0],0,0,"data\\graphics\\Circlegreen.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( circle_draw.lpCircle[0],CLR_INVALID );
	if( DDraw.CreateBMPSurface(circle_draw.lpCircle[1],0,0,"data\\graphics\\Circlered.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( circle_draw.lpCircle[1],CLR_INVALID );

	//载入地面
    DDraw.CreateBMPSurface(lpDDSGroundRC,0,0,"data\\graphics\\terrain\\summer.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Row,0,0,"data\\graphics\\Select_Row.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Col,0,0,"data\\graphics\\Select_Col.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSColor[0],0,0,"data\\graphics\\green.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSColor[1],0,0,"data\\graphics\\red.bmp",DDSCAPS_SYSTEMMEMORY);
	//载入图标
	DDraw.CreateBMPSurface(lpDDSIcons,0,0,"data\\graphics\\icons.bmp",DDSCAPS_SYSTEMMEMORY);
	
	ifstream inf;								
	int n,i=0;
	string a;

	inf.open("data\\Configure\\Human_Building.Conf");//读取人族建筑定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &Building_Common[0][i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(Building_Common[0][i].lpSurf,0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( Building_Common[0][i].lpSurf, CLR_INVALID );
		inf>>Building_Common[0][i].lSrcWCount;
		inf>>Building_Common[0][i].lSrcHCount;
		inf>>Building_Common[0][i].Wide;
		inf>>Building_Common[0][i].Height;
		inf>>Building_Common[0][i].sound[0];
        inf>>Building_Common[0][i].Hp;
		inf>>Building_Common[0][i].defense;
		inf>>Building_Common[0][i].train_time;
		inf>>Building_Common[0][i].defense_kind;
		Building_Common[0][i].abletoproduct = i;
		Building_Common[0][i].count = 0;
		Building_Common[0][i].eachWide=(Building_Common[0][i].Wide)/(Building_Common[0][i].lSrcWCount);
		Building_Common[0][i].eachHeight=(Building_Common[0][i].Height)/(Building_Common[0][i].lSrcHCount);
		getline(inf,a,' ');
		source_path=a.c_str();
		source=source_path+1;
		DDraw.CreateBMPSurface(Building_Common[0][i].lpIcon,0,0,source,DDSCAPS_SYSTEMMEMORY);
	}
	inf.close();

	inf.open("data\\Configure\\Orc_Building.Conf");//读取兽族建筑定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &Building_Common[1][i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(Building_Common[1][i].lpSurf,0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( Building_Common[1][i].lpSurf, CLR_INVALID );
		inf>>Building_Common[1][i].lSrcWCount;
		inf>>Building_Common[1][i].lSrcHCount;
		inf>>Building_Common[1][i].Wide;
		inf>>Building_Common[1][i].Height;
		inf>>Building_Common[1][i].sound[0];
		inf>>Building_Common[1][i].Hp;
		inf>>Building_Common[1][i].defense;
		inf>>Building_Common[1][i].train_time;
		inf>>Building_Common[1][i].defense_kind;
		Building_Common[1][i].abletoproduct = i;
		Building_Common[1][i].count=0;
		Building_Common[1][i].eachWide=(Building_Common[1][i].Wide)/(Building_Common[1][i].lSrcWCount);
		Building_Common[1][i].eachHeight=(Building_Common[1][i].Height)/(Building_Common[1][i].lSrcHCount);
		getline(inf,a,' ');
		source_path=a.c_str();
		source=source_path+1;
		DDraw.CreateBMPSurface(Building_Common[1][i].lpIcon,0,0,source,DDSCAPS_SYSTEMMEMORY);
	}
	inf.close();

	inf.open("data\\Configure\\Human_Solider.Conf");//读取人族士兵定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &People_Common[0][i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(People_Common[0][i].lpSurf,0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( People_Common[0][i].lpSurf, CLR_INVALID );
		inf>>People_Common[0][i].lSrcWCount;
		inf>>People_Common[0][i].lSrcHCount;
		inf>>People_Common[0][i].Wide;
		inf>>People_Common[0][i].Height;
		People_Common[0][i].eachWide=(People_Common[0][i].Wide)/(People_Common[0][i].lSrcWCount);
		People_Common[0][i].eachHeight=(People_Common[0][i].Height)/(People_Common[0][i].lSrcHCount);
		inf>>People_Common[0][i].start_run;
		inf>>People_Common[0][i].end_run;
		inf>>People_Common[0][i].start_attack;
		inf>>People_Common[0][i].end_attack;
		inf>>People_Common[0][i].start_die;
		inf>>People_Common[0][i].end_die;
		inf>>People_Common[0][i].sound[0];
        inf>>People_Common[0][i].sound[1];
	    inf>>People_Common[0][i].sound[2];
        inf>>People_Common[0][i].sound[3];
		inf>>People_Common[0][i].sound[4];
		inf>>People_Common[0][i].movespeed;
		inf>>People_Common[0][i].attackspeed;
		inf>>People_Common[0][i].range;
		inf>>People_Common[0][i].Hp;
		inf>>People_Common[0][i].missile_kind;
		inf>>People_Common[0][i].attackpower;
		inf>>People_Common[0][i].sight;
		inf>>People_Common[0][i].defense;
		inf>>People_Common[0][i].population_cost;
		inf>>People_Common[0][i].gold_cost;
		inf>>People_Common[0][i].attack_kind;
		inf>>People_Common[0][i].defense_kind;
		People_Common[0][i].abletoproduct = i;
		People_Common[0][i].count=0;
		getline(inf,a,' ');
		source_path=a.c_str();
		source=source_path+1;
		DDraw.CreateBMPSurface(People_Common[0][i].lpIcon,0,0,source,DDSCAPS_SYSTEMMEMORY);
	}
	inf.close();

	inf.open("data\\Configure\\Orc_Solider.Conf");//读取兽族士兵定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &People_Common[1][i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(People_Common[1][i].lpSurf,0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( People_Common[1][i].lpSurf, CLR_INVALID );
		inf>>People_Common[1][i].lSrcWCount;
		inf>>People_Common[1][i].lSrcHCount;
		inf>>People_Common[1][i].Wide;
		inf>>People_Common[1][i].Height;
		People_Common[1][i].eachWide=(People_Common[1][i].Wide)/(People_Common[1][i].lSrcWCount);
		People_Common[1][i].eachHeight=(People_Common[1][i].Height)/(People_Common[1][i].lSrcHCount);
		inf>>People_Common[1][i].start_run;
		inf>>People_Common[1][i].end_run;
		inf>>People_Common[1][i].start_attack;
		inf>>People_Common[1][i].end_attack;
		inf>>People_Common[1][i].start_die;
		inf>>People_Common[1][i].end_die;
		inf>>People_Common[1][i].sound[0];
        inf>>People_Common[1][i].sound[1];
		inf>>People_Common[1][i].sound[2];
        inf>>People_Common[1][i].sound[3];
		inf>>People_Common[1][i].sound[4];
		inf>>People_Common[1][i].movespeed;
		inf>>People_Common[1][i].attackspeed;
		inf>>People_Common[1][i].range;
		inf>>People_Common[1][i].Hp;
		inf>>People_Common[1][i].missile_kind;
		inf>>People_Common[1][i].attackpower;
		inf>>People_Common[1][i].sight;
		inf>>People_Common[1][i].defense;
		inf>>People_Common[1][i].population_cost;
		inf>>People_Common[1][i].gold_cost;
		inf>>People_Common[1][i].attack_kind;
		inf>>People_Common[1][i].defense_kind;
		People_Common[1][i].abletoproduct = i;
		People_Common[1][i].count=0;
		getline(inf,a,' ');
		source_path=a.c_str();
		source=source_path+1;
		DDraw.CreateBMPSurface(People_Common[1][i].lpIcon,0,0,source,DDSCAPS_SYSTEMMEMORY);
	}
	inf.close();

	inf.open("data\\Configure\\Neutral.Conf");//读取中立单位定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &Neutral_Common[i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(Neutral[i],0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( Neutral[i], CLR_INVALID );
		inf>>Neutral_Common[i].lSrcWCount;
		inf>>Neutral_Common[i].lSrcHCount;
		inf>>Neutral_Common[i].Wide;
		inf>>Neutral_Common[i].Height;
		inf>>Neutral_Common[i].sound[0];
		inf>>Neutral_Common[i].Hp;	
		Neutral_Common[i].eachWide=(Neutral_Common[i].Wide)/(Neutral_Common[i].lSrcWCount);
		Neutral_Common[i].eachHeight=(Neutral_Common[i].Height)/(Neutral_Common[i].lSrcHCount);
		Neutral_Common[i].count=0;
	}
	inf.close();

	inf.open("data\\Configure\\Interface.Conf");//读取界面定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &Interface_Common[i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(Interface[i],0,0,source,DDSCAPS_SYSTEMMEMORY);
		inf>>Interface_Common[i].lSrcWCount;
		inf>>Interface_Common[i].lSrcHCount;
		inf>>Interface_Common[i].Wide;
		inf>>Interface_Common[i].Height;
		inf>>Interface_Common[i].pos.x;
		inf>>Interface_Common[i].pos.y;
		Interface_Common[i].eachWide=(Interface_Common[i].Wide)/(Interface_Common[i].lSrcWCount);
		Interface_Common[i].eachHeight=(Interface_Common[i].Height)/(Interface_Common[i].lSrcHCount);
	}
	inf.close();

	inf.open("data\\Configure\\Missile.Conf");//读取导弹定义
	inf>>n;
	for(i=0;i<n;i++)
	{
		ZeroMemory( &Missile_Common[i], sizeof(Unit_Common) );
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		DDraw.CreateBMPSurface(MissileSource[i],0,0,source,DDSCAPS_SYSTEMMEMORY);
		DDraw.DDSetColorKey( MissileSource[i], CLR_INVALID );
		inf>>Missile_Common[i].lSrcWCount;
		inf>>Missile_Common[i].lSrcHCount;
		inf>>Missile_Common[i].Wide;
		inf>>Missile_Common[i].Height;
		Missile_Common[i].eachWide=(Missile_Common[i].Wide)/(Missile_Common[i].lSrcWCount);
		Missile_Common[i].eachHeight=(Missile_Common[i].Height)/(Missile_Common[i].lSrcHCount);
		inf>>Missile_Common[i].start_attack;
		inf>>Missile_Common[i].end_attack;
	}
	inf.close();
}

/************************************************************
*   函数名: InitDirectX(...)
*     功能: 初始化DirectX以及载入声音图像数据
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::InitDirectX( HWND _hWnd )
{
    hWnd = _hWnd;
    //>>>>>> 初始化CDInput >>>>>>>>>>>
    Input.Set_bFullScr( FALSE );
    Input.InitDInput( hWnd );
    KeyKB = Input.Get_KeyKB();
    KeyMouse = Input.Get_KeyMouse();

    //>>>>>> 初始化CDSound >>>>>>>>>>>
    Sound.Set_bSetPrimaryFormat( TRUE );
    //设置次级静态缓冲区个数n个
    ifstream inf;							
	int n,k=0;
	string a;
    inf.open("data\\Configure\\Sound.Conf");//读取声音定义
	inf>>n;
    Sound.Set_dwNumStatics( n );
    //设置次级流式缓冲区个数为4个
	Sound.Set_dwNumStreams( 4 );
    Sound.Set_dwMaxBytesStream( 1024000 );
	if( Sound.InitDSound( hWnd ) )
    {   
        Sound.CreateStreamBuffer( 0, "data\\Sound\\start.wav" );
		Sound.CreateStreamBuffer( 1, "data\\Sound\\back1.wav" );
		Sound.CreateStreamBuffer( 2, "data\\Sound\\victory.wav" );
		Sound.CreateStreamBuffer( 3, "data\\Sound\\defeat.wav" );
		Sound.SubStreamVolume(1,1000);
	for(k=0;k<n;k++)
	{
		getline(inf,a,' ');
		LPCSTR source_path=a.c_str();
		LPCSTR source=source_path+1;
		Sound.CreateStaticBuffer(k,source);
	}
	inf.close();
        // 设置主缓冲区的声音格式
        if( Sound.Get_bSetPrimaryFormat() )
            Sound.SetPrimaryBufferFormat( 2, 44100, 16 );
    }

    //>>>>>> 初始化CDDraw >>>>>>>>>>>
    DDraw.InitDDraw(
        hWnd, 
        bFullScr,
        ScreenW, ScreenH,
        ColorBits );
    LoadImageSurf();//为表面载入图像数据

    //>>>>>> 初始化CText >>>>>>>>>>>
    Text.Set_lpDDSBack(DDraw.Get_lpDDSBack());
	//>>>>>> 初始化计数器 >>>>>>>>>>>
	Initial_count();
}
/************************************************************
*   函数名: SaveGroundDesc(...)
*     功能: 保存指定场景的地面描述
*************************************************************/
void CGame::SaveGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	fwrite( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   函数名: LoadGroundDesc(...)
*     功能: 载入指定场景的地面描述
*************************************************************/
void CGame::LoadGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	//为地面方块载入数据
	if( map.lSquareCount==0 )
        return;
    map.Grn = new LONG[map.lGrnN];
    fread( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   函数名: SaveObject(...)
*     功能: 保存指定场景的对象
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::SaveObject( FILE *file )
{
    if( file==NULL )
        return;
	for(LONG i=0;i<2;i++){//player
		LONG temp=player[i].race;
		fwrite( &temp,sizeof(LONG),1,file);
		temp= player[i].gold;
		fwrite( &temp,sizeof(LONG),1,file);
		temp= player[i].population;
		fwrite( &temp,sizeof(LONG),1,file);
		temp=player[i].BuildingCount;
		fwrite( &temp,sizeof(LONG),1,file);
		temp=player[i].PeopleCount;
		fwrite( &temp,sizeof(LONG),1,file);
		player[i].BuildingCount=player[i].Building.size();
		player[i].PeopleCount=player[i].People.size();
		LONG length =player[i].PeopleCount;
		for(LONG j=0;j<length;j++)
		{
			Unit unitTemp=player[i].People[j];
			fwrite( &unitTemp,sizeof(Unit),1,file);
		}
		 length =player[i].BuildingCount;
		for( j=0;j<length;j++)
		{
			Unit unitTemp=player[i].Building[j];
			fwrite( &unitTemp,sizeof(Unit),1,file);
		}				
	}
	//中立
	LONG length =Neutral_Unit.size();
	fwrite( &length,sizeof(LONG),1,file);	
	for( LONG j=0;j<length;j++)
	{
		Unit unitTemp=Neutral_Unit[j];
		fwrite( &unitTemp,sizeof(Unit),1,file);
	}
}
/************************************************************
*   函数名: LoadObject(...)
*     功能: 载入指定场景的对象
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::LoadObject( FILE *file )
{
    if( file==NULL )
        return;
    for(LONG i=0;i<2;i++){//player
		LONG temp;
		fread( &temp,sizeof(LONG),1,file);
		player[i].race = temp;
		fread( &temp,sizeof(LONG),1,file);
		player[i].gold = temp;
		fread( &temp,sizeof(LONG),1,file);
		player[i].population = temp;
		fread( &temp,sizeof(LONG),1,file);
		player[i].BuildingCount = temp;
		fread( &temp,sizeof(LONG),1,file);
		player[i].PeopleCount = temp;
		player[i].Building.clear();		
		player[i].People.clear();		
		LONG length =player[i].PeopleCount;
		for(LONG j=0;j<length;j++)
		{
			Unit unitTemp;
			fread( &unitTemp,sizeof(Unit),1,file);
			player[i].People.push_back(unitTemp);
			player[i].count[unitTemp.kind]++;
		}
		 length =player[i].BuildingCount;
		 
		for(j=0;j<length;j++)
		{
			Unit unitTemp;
			fread( &unitTemp,sizeof(Unit),1,file);
			player[i].Building.push_back(unitTemp);
		}				
	}
	LONG length;
	fread( &length,sizeof(LONG),1,file);	
	for( LONG j=0;j<length;j++)
	{
		Unit unitTemp;
		fread( &unitTemp,sizeof(Unit),1,file);
		Neutral_Unit.push_back(unitTemp);
	}
}
/************************************************************
*   函数名: SaveTerrainSquare(...)
*     功能: 保存指定场景的地形数组
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::SaveTerrainSquare( FILE *file )
{
    if( file==NULL )
        return;
    fwrite( map.Square, sizeof(WORD)*map.lSquareCount,1,file);
}
/************************************************************
*   函数名: LoadTerrainSquare(...)
*     功能: 载入指定场景的地形数组
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::LoadTerrainSquare( FILE *file )
{
    if( file==NULL )
        return;
    if( map.lSquareCount==0 )
        return;
    map.Square = new WORD[map.lSquareCount];
    fread( map.Square, sizeof(WORD)*map.lSquareCount,1,file);
}
/************************************************
*   函数名: SaveSceneData(...)
*     功能: 保存场景数据
*       (c) jk&ljq, 2010
*************************************************/
void CGame::SaveSceneData( BOOL bSaveAs )
{
    //是保存为或者是第一次保存则选择文件名
    if( !GetSceneFileName( 1 ) )
            return;
    //建立指定场景文件
    FILE *file = fopen(sceFilePath,"wb");

    //保存场景结构数据
    fwrite(&map,sizeof(MAP),1,file);
	fwrite(&UI,sizeof(LONG),1,file);
    //保存地面方块描述数据
    SaveGroundDesc( file );
    //保存对象数据
    SaveObject( file );
    //保存地形方块数据
    SaveTerrainSquare( file);

    //结束保存工作
    fclose(file);
    lSaveState = 1;//当前场景数据已经存为文件
}
BOOL CGame::GetSceneFileName( DWORD dwFlag )
{
    // 设置OPENFILENAME结构
    OPENFILENAME ofn ;//
    ZeroMemory(&ofn, sizeof(ofn));
	
    static const TCHAR szFilter[] = TEXT("MAP Files (.map)\0*.map;\0")TEXT("All Files (*.*)\0*.*;\0");
    ofn.lStructSize         = sizeof(OPENFILENAME);
    ofn.hwndOwner           = hWnd;
    ofn.hInstance           = NULL;
    ofn.lpstrFilter         = szFilter;
    ofn.nFilterIndex        = 1;
    ofn.lpstrCustomFilter   = NULL;
    ofn.nMaxCustFilter      = 0;
    ofn.lpstrFile           = sceFilePath;
    ofn.nMaxFile            = MAX_PATH;
    ofn.lpstrFileTitle      = NULL;
    ofn.nMaxFileTitle       = 0;
    ofn.lpstrInitialDir     = NULL;
    if( dwFlag == 0 )//打开文件编辑
        ofn.lpstrTitle      = TEXT("选择一个场景文件...");
    else//保存文件
        ofn.lpstrTitle      = TEXT("保存为指定的文件...");
    ofn.Flags               = OFN_HIDEREADONLY;
    ofn.nFileOffset         = 0;
    ofn.nFileExtension      = 0;
    ofn.lpstrDefExt         = TEXT("map");
    ofn.lCustData           = 0L;
    ofn.lpfnHook            = NULL;
    ofn.lpTemplateName      = NULL;
	
    if( dwFlag == 0 )//打开文件编辑
        return GetOpenFileName( &ofn );
    else //保存文件
        return GetSaveFileName( &ofn );
}
/************************************************
*   函数名: LoadSceneData(...)
*     功能: 载入指定场景的数据
*************************************************/
void CGame::LoadSceneData( void )
{
	FILE *file;
	//获得打开文件的名称
	char OldDir[256];
	if(lAppState==2){
		  GetCurrentDirectory(256, OldDir);
		if( !GetSceneFileName( 0 ) ){
			lAppState=0;
			return;}
	}
    //释放旧场景的数据
	DeleteSceneData( map );
    //打开指定场景文件
	if(lAppState==2){
		file = fopen(sceFilePath,"rb");
		SetCurrentDirectory(OldDir);
	}
	else if(lAppState == 1){
		file = fopen("data\\maps\\jk.map","rb");
	}
    //载入场景结构数据
	if(file!=NULL){
    fread(&map,sizeof(MAP),1,file);
	fread(&UI,sizeof(LONG),1,file);
	}
    //载入地面方块描述数据
    LoadGroundDesc( file );
    //载入对象数据
    LoadObject( file );
    //载入地形方块数据
    LoadTerrainSquare( file );

    //结束载入工作
   fclose(file);
}
/***********************************************
*   函数名: MakeScene(...)
*     功能: 生成当前场景(lSceneID指定)
************************************************/
void CGame::MakeScene( void )
{
    if( lAppState==1 || lAppState==2 )
		LoadSceneData();
	//>>>>>> 初始化CGetPath >>>>>>>>>
	//设置地形方块大小为16*16
	GPa.Set_lSquareC(16);
	//设置场景大小
	GPa.Set_SceneW(map.SceneW);
	GPa.Set_SceneH(map.SceneH);
	//把地形数组传送给CGetPath对象
	GPa.Set_Square((VOID*)map.Square);
}

/************************************************************
*   函数名: DrawButton(...)
*     功能: 画按钮
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::DrawButton( LPDIRECTDRAWSURFACE7 Surf,
                        WORD state, LPRECT lpRect )
{
    //整个表面
    RECT rect =GetRect(0,0,131,54);
    if( state == 0 )
        DDraw.BlitBack( lpRect->left,lpRect->top,lpDDSButtonDown,&rect,DDBLTFAST_SRCCOLORKEY);
    else
        DDraw.BlitBack( lpRect->left,lpRect->top,lpDDSButtonUp,&rect,DDBLTFAST_SRCCOLORKEY);
}

/*************************************************
*   函数名: Header(...)
*     功能: 游戏开头画面
*       (c) jk&ljq, 2010
**************************************************/
void CGame::Header( void )
{
	Sound.PlayStream(0,1);
    //显示背景画面
    DDraw.BlitBack(0,0,lpDDSHeader,NULL,0);
	Text.TextBackSurf(350,300,"New Game",RGB(255,0,0),0,0);
	Text.TextBackSurf(350,380,"Load Game",RGB(255,0,0),0,0);
	Text.TextBackSurf(350,460,"Exit",RGB(255,0,0),0,0);
	Text.ReMain();

    RECT rectNEW,rectLOAD,rectEXIT;
    FLOAT x=(FLOAT)ScreenW/800;
    FLOAT y=(FLOAT)ScreenH/600;
    rectNEW = GetRect((LONG)(320*x),(LONG)(270*y),
                      (LONG)(450*x),(LONG)(324*y));
    rectLOAD= GetRect((LONG)(320*x),(LONG)(330*y),
                      (LONG)(450*x),(LONG)(384*y));
    rectEXIT= GetRect((LONG)(320*x),(LONG)(420*y),
                      (LONG)(450*x),(LONG)(474*y));
    //按钮检测
    if( KeyMouse[0].Pos.x>(320*x) &&
        KeyMouse[0].Pos.x<(450*x) &&
        KeyMouse[0].Pos.y>=270*y &&
        KeyMouse[0].Pos.y<324*y )
    {//鼠标在"NEW GAME"模式上
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//鼠标左键弹起
            lAppState = 1;//新模式
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectNEW );
        }
        else if( KeyMouse[0].Key==DIMOFS_BUTTON0 )
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectNEW );
        else
            DrawButton( DDraw.Get_lpDDSBack(),1, &rectNEW );
    }
    else if(KeyMouse[0].Pos.x>(320*x) &&
        KeyMouse[0].Pos.x<(450*x) &&
        KeyMouse[0].Pos.y>=330*y &&
        KeyMouse[0].Pos.y<384*y )
    {//鼠标在"LOAD GAME"模式上
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//鼠标左键弹起
            lAppState = 2;//载入模式
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectLOAD );
        }
        else if( KeyMouse[0].Key==DIMOFS_BUTTON0 )
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectLOAD);
        else
            DrawButton( DDraw.Get_lpDDSBack(),1, &rectLOAD);
    }
    else if(KeyMouse[0].Pos.x>320*x &&
        KeyMouse[0].Pos.x<450*x &&
        KeyMouse[0].Pos.y>=420*y &&
        KeyMouse[0].Pos.y<475*y )
    {//鼠标在EXIT模式上
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//鼠标左键弹起
            lAppState = 5;
		    Sound.ReleaseDSound();
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectEXIT);
        }
        else if( KeyMouse[0].Key==DIMOFS_BUTTON0 )
            DrawButton( DDraw.Get_lpDDSBack(),0, &rectEXIT);
        else
            DrawButton( DDraw.Get_lpDDSBack(),1, &rectEXIT);
    }
}
/**************************************************
*   函数名: RestoreVariable(...)
*     功能: 恢复需要恢复的变量
*       (c) jk&ljq, 2010
***************************************************/
inline void CGame::RestoreVariable( void )
{
	
    LONG flag=0;
	//调整select
	AdjustSelect();
	
	//删除MissileQueue中死亡的单位
	vector<Missile>::iterator itr1 = MissileQueue.begin();
	for( itr1 = MissileQueue.begin(); itr1!=MissileQueue.end(); ++itr1 )
	{
		if(!CheckUI(itr1->select))//统一标志不对
		{
			itr1->select=AdjustUI(itr1->select);//调整
			if(itr1->select.uniform_identifier == -1) //调整后发现该单位已不存在
			{
				MissileQueue.erase(itr1);
				itr1--;
				continue;
			}
		}
		Unit *temp = returnUnit(itr1->select);
		if(temp->bDead == TRUE || temp == NULL){
			MissileQueue.erase(itr1);
			itr1--;
			continue;
		}
	}
	//删除死亡单位
	vector<Unit>::iterator itr = Neutral_Unit.begin();
	for( itr = Neutral_Unit.begin(); itr!=Neutral_Unit.end(); ++itr )
	{
		if( itr->bDead == TRUE )
		{
			Neutral_Unit.erase(itr);
			itr--;
			continue;
		}
	}
	for( LONG i=0;i<2;i++){//player
		//建筑
		itr = player[i].Building.begin();
		for( itr = player[i].Building.begin(); itr!=player[i].Building.end(); ++itr )
		{
			if( itr->bDead == TRUE )
			{
				//>>>>> 解除对应的障碍 >>>>>>>
				LONG dis=soundDis(ScrPos,itr->Pos);
				Sound.SetStaticVolume(61+player[i].race,0-dis);
				Sound.PlayStatic(61+player[i].race,0,1);				
				player[i].Building.erase(itr);
				itr--;
			}
		}		
		//士兵
		itr = player[i].People.begin();
		for( itr = player[i].People.begin(); itr!=player[i].People.end(); ++itr )
		{
			if( itr->bDead == TRUE )
			{
				//>>>>> 解除对应的障碍 >>>>>>>
                LONG dis=soundDis(ScrPos,itr->Pos);
				Sound.SetStaticVolume(People_Common[player[i].race][itr->kind].sound[4],0-dis);
				Sound.PlayStatic(People_Common[player[i].race][itr->kind].sound[4],0,1);
				player[i].count[itr->kind]--;
				//player[i].population -= People_Common[player[i].race][itr->kind].population_cost;
				player[i].People.erase(itr);
				itr--;
				continue;
			}
			else if(itr->state == UnitActionFollow || itr->state == UnitActionAttackFollow ||
				itr->state == UnitActionAttack){
				if(!CheckUI(itr->Target_Select))//统一标志不对
					itr->Target_Select=AdjustUI(itr->Target_Select);//调整
				if(itr->Target_Select.uniform_identifier == -1) //调整后发现该单位已不存在
				{
					itr->state =UnitActionNone;
				    itr->pic_Index = itr->Direct;
					continue;
				}
				Unit *tempUnit = returnUnit(itr->Target_Select);
				if(tempUnit->bDead == TRUE|| tempUnit == NULL) //攻击对象死亡
				{
					itr->state =UnitActionNone;
					itr->pic_Index = itr->Direct;
				}
			}
		}
	}
	//同步
	for( i=0;i<2;i++){//player
		//建筑
		/*itr = player[i].Building.begin();
		for( itr = player[i].Building.begin(); itr!=player[i].Building.end(); ++itr )
		{
			
		}*/		
		//士兵
		itr = player[i].People.begin();
		for( itr = player[i].People.begin(); itr!=player[i].People.end(); ++itr )
		{			
			if(itr->state == UnitActionFollow || itr->state == UnitActionAttackFollow ||
				itr->state == UnitActionAttack){
				if(!CheckUI(itr->Target_Select))//统一标志不对
					itr->Target_Select=AdjustUI(itr->Target_Select);//调整
				if(itr->Target_Select.uniform_identifier == -1) //调整后发现该单位已不存在
				{
					itr->state =UnitActionNone;
					itr->pic_Index = itr->Direct;
					continue;
				}
			}
		}
	}
	//同步编队信息
	AdjustAfterDelete();

	//死亡队列中的信息处理
	itr = DeadQueue.begin();
	for( itr = DeadQueue.begin(); itr!=DeadQueue.end(); ++itr )
	{
		
		if( itr->state == UnitActionDie )
		{
			if(itr->lEndIndex-itr->pic_Index<8){//腐烂结束
				DeadQueue.erase( itr );
				itr--;
			}				
		}
	}
}
/**********************************************
*   函数名: DeleteSceneData(...)
*     功能: 删除指定场景
*       (c) jk&ljq, 2010
***********************************************/
void CGame::DeleteSceneData( MAP map )
{
	for(LONG i=0;i<2;i++){
		player[i].Building.clear();
		player[i].People.clear();
	}
    ZeroMemory( &map, sizeof(MAP) );
	delete []map.Grn;
	ZeroMemory( &player[0], sizeof(Player) );
	ZeroMemory( &player[1], sizeof(Player) );
    //释放地形数组空间
	select.clear();
	select_frame.clear();
	Neutral_Unit.clear();
	MissileQueue.clear();
	DeadQueue.clear();
    delete[] map.Square;
	select_1.clear();
	select_2.clear();
	select_3.clear();
}
/**************************************************
*   函数名: CheckScrPos(...)
*     功能: 检测屏幕位置
***************************************************/
inline LONG CGame::CheckScrPos( void )
{
    //保存屏幕位置
    POINT oldScrPos = ScrPos;

    //鼠标在客户区边界上则移动屏幕
    if( KeyMouse[0].Pos.x<=0 )
        ScrPos.x -= 10;
    else if( KeyMouse[0].Pos.x>=ScreenW-8 )
        ScrPos.x += 10;
    if( KeyMouse[0].Pos.y<=0 )
        ScrPos.y -= 10;
    else if( KeyMouse[0].Pos.y>=ScreenH-6 )
        ScrPos.y += 10;
    
    //屏幕位置的边界限制
    if( ScrPos.x<0 )
        ScrPos.x = 0;
    else if( ScrPos.x>(map.SceneW-ScreenW) )
        ScrPos.x = map.SceneW-ScreenW;
    if( ScrPos.y<0 )
        ScrPos.y = 0;
    else if( ScrPos.y>(map.SceneH-ScreenH) )
        ScrPos.y = map.SceneH-ScreenH;

    //改变了屏幕位置则更新屏幕
    if( oldScrPos.x!=ScrPos.x ||
        oldScrPos.y!=ScrPos.y )
        return 1;
    else//没有改变屏幕则无需更新屏幕
        return 0;
}
/************************************************************
*   函数名: MakeGround(...)
*     功能: 显示地面
*************************************************************/
void CGame::MakeGround( void )
{
    //清空后台表面
    DDBLTFX ddbltfx; 
    ddbltfx.dwSize = sizeof(ddbltfx); 
    ddbltfx.dwFillColor = 0;//黑色
    DDraw.Get_lpDDSBack()->Blt(NULL,NULL,NULL,
                   DDBLT_COLORFILL,&ddbltfx);

    //>>>>>>> 地面的显示 >>>>>>>>>>>
	LONG x,y,wN;
    RECT rect;
    //求场景宽度上有多少个地面方块
    wN = map.SceneW/32;
	
    for( WORD n=0;n<map.lGrnN;n++ )
    {
        y = (LONG)(n/wN);//求行号
        x = n - y*wN;//求列号
        y = y*32-ScrPos.y;//求转换到屏幕中的位置
        x = x*32-ScrPos.x;//求转换到屏幕中的位置
		if( x >-32 &&
			x <ScreenW+32&&
			y >-32 &&
			y <ScreenH+32)
		{
			rect = GetRscRect(GroundRC[map.Grn[n]],32,32,16);
			DDraw.BlitBack( x,y,lpDDSGroundRC,&rect,0);
		}
    }    
}
/************************************************
*   函数名: playsound(...)
*     功能: 播放选中对应单位的声音，加入了频率变化
*    (c) jk&ljq, 2010
*************************************************/
void CGame::playsound(SelectUnit ui,LONG index)
{   LONG dis;
	Unit_Common *temp=returnUnitCommon(ui);
	Unit        *temp1=returnUnit(ui);
	if(temp!=NULL&&temp1!=NULL)
	{
		DWORD freq;
		ofstream result("result.txt",ios::app);
		dis=soundDis(ScrPos,temp1->Pos);
		freq=Sound.GetStaticFreq(temp->sound[index]);
		Sound.SetStaticVolume(temp->sound[index],0-dis);	
	    if(!soundIn(temp1->Pos))
		{
			if(freq==22050){
				Sound.SetStaticFreq(temp->sound[index],0.86*22050);
			}
		}
		else
		{
		if(freq!=11025)
		  Sound.SetStaticFreq(temp->sound[index],22050);
		}
		freq=Sound.GetStaticFreq(temp->sound[index]);
		result<<soundIn(temp1->Pos)<<"/"<<Sound.GetStaticVolume(temp->sound[index])<<","<<freq<<"||";
		Sound.PlayStatic(temp->sound[index],0,1);
	}
}
/************************************************
*   函数名: soundDis(...)
*     功能: 根据距离对声音做出调整
*    (c) jk&ljq, 2010
*************************************************/
LONG CGame::soundDis(POINT a,POINT b)
{
	POINT mid;	
	mid.x=a.x+400;
	mid.y=a.y+320;
	LONG dis;
	dis=sqrt(pow((b.x-a.x),2)+pow((b.y-a.y),2));
	return dis;
}
/************************************************
*   函数名: soundIn(...)
*     功能: 判断发生单位是否在屏幕之内
*    (c) jk&ljq, 2010
*************************************************/
bool CGame::soundIn(POINT a)
{
	POINT an;	
	an.x=ScrPos.x+800;
	an.y=ScrPos.y+640;
	if(ScrPos.x<a.x&&an.x>a.x&&ScrPos.y<a.y&&an.y>a.y)
		return true;
	else
		return false;
	   
}
/************************************************
*   函数名: MakeEntity(...)
*     功能: 角色、怪物、地面对象、怪物选择圈和
*           文字的显示
*       (c) jk&ljq, 2010
*************************************************/
void CGame::MakeEntity( void )
{
    RECT rect;
    LONG x,y,i,j,w,h;
	Unit *temp;
	Unit_Common *temp1;
	vector<Unit>::iterator itr0 = DeadQueue.begin();
	//死亡显示
	for( itr0 = DeadQueue.begin(); itr0!=DeadQueue.end(); ++itr0 )
	{
		temp1 = returnUnitCommon(itr0->Target_Select);
		if(temp1==NULL)
			continue;
		rect=GetRscRect(itr0->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
		x=itr0->Pos.x-temp1->eachWide/2-ScrPos.x;
		y=itr0->Pos.y-temp1->eachHeight/2-ScrPos.y;
		i= player[itr0->Target_Select.player].race;
		j= itr0->Target_Select.kind;
		DDraw.BlitBack(x,y,People_Common[i][j].lpSurf,&rect,DDBLTFAST_SRCCOLORKEY );
	}   
	//select显示
	for(i=0;i<select.size();i++){
		temp = returnUnit(select[i]);
		if(temp->bDead == TRUE|| temp == NULL) //攻击对象死亡
		{
			continue;
		}
		temp1 = returnUnitCommon(select[i]);
		if(temp1==NULL)
			continue;
		w=temp1->eachWide;
		h=temp1->eachHeight;
		if(select[i].player!=2 &&select[i].Bui_Sol==1){//士兵画圈
			w /=2;
			h /=2;
			if(temp->uniform_identifier == select[i].uniform_identifier){
				rect = GetRect(0,0,40,30);
				DDraw.BlitBack(temp->Pos.x-ScrPos.x-w/2,temp->Pos.y-ScrPos.y-10,circle_draw.lpCircle[select[i].player],&rect,DDBLTFAST_SRCCOLORKEY );
			}
		}		
		else 
		if(temp->uniform_identifier == select[i].uniform_identifier){
			POINT a,b;
			a.x= temp->Pos.x-w/2;
			a.y= temp->Pos.y-h/2;
			b.x= temp->Pos.x+w/2;
			b.y= temp->Pos.y+h/2;
			RectangleFrame(a,b);//画矩形框
		}
	}

	for(i=0;i<2;i++){//player
		//建筑
		itr0= player[i].Building.begin();
		for(itr0 = player[i].Building.begin(); itr0!=player[i].Building.end(); ++itr0){
			temp1=&Building_Common[player[i].race][itr0->kind];
			if(itr0->Pos.x >ScrPos.x-MaxWObject &&
				itr0->Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				itr0->Pos.y >ScrPos.y-MaxHObject &&
				itr0->Pos.y <ScrPos.y+MaxHObject+ScreenH)
			{
				rect=GetRscRect(itr0->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
				x=itr0->Pos.x-temp1->eachWide/2-ScrPos.x;
                y=itr0->Pos.y-temp1->eachHeight/2-ScrPos.y;
				DDraw.BlitBack(x,y,Building_Common[player[i].race][itr0->kind].lpSurf,&rect,DDBLTFAST_SRCCOLORKEY );
			}
		}
		//士兵
		itr0= player[i].People.begin();
		for(itr0 = player[i].People.begin(); itr0!=player[i].People.end(); ++itr0){
			temp1=&People_Common[player[i].race][itr0->kind];
			if(itr0->Pos.x >ScrPos.x-MaxWObject &&
				itr0->Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				itr0->Pos.y >ScrPos.y-MaxHObject &&
				itr0->Pos.y <ScrPos.y+MaxHObject+ScreenH)
			{
				rect=GetRscRect(itr0->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
				x=itr0->Pos.x-temp1->eachWide/2-ScrPos.x;
                y=itr0->Pos.y-temp1->eachHeight/2-ScrPos.y;
				DDraw.BlitBack(x,y,People_Common[player[i].race][itr0->kind].lpSurf,&rect,DDBLTFAST_SRCCOLORKEY );
			}
		}
	}
	//中立
	itr0= Neutral_Unit.begin();
	for(itr0 = Neutral_Unit.begin(); itr0!=Neutral_Unit.end(); ++itr0){
				temp=&Neutral_Unit[0];
				temp1=&Neutral_Common[temp->kind];
				if(itr0->Pos.x >ScrPos.x-MaxWObject &&
				itr0->Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				itr0->Pos.y >ScrPos.y-MaxHObject &&
				itr0->Pos.y <ScrPos.y+MaxHObject+ScreenH)
				{
					rect=GetRscRect(itr0->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
					x=itr0->Pos.x-temp1->eachWide/2-ScrPos.x;
					y=itr0->Pos.y-temp1->eachHeight/2-ScrPos.y;
					DDraw.BlitBack(x,y,Neutral[itr0->kind],&rect,DDBLTFAST_SRCCOLORKEY );
				}
	}
	//导弹
	vector<Missile>::iterator itr = MissileQueue.begin();
	for( itr = MissileQueue.begin(); itr!=MissileQueue.end(); ++itr )
	{
		temp1 = &Missile_Common[itr->kind];
		rect=GetRscRect(itr->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
		x=itr->pos.x-temp1->eachWide/2-ScrPos.x;
		y=itr->pos.y-temp1->eachHeight/2-ScrPos.y;
		DDraw.BlitBack(x,y,MissileSource[itr->kind],&rect,DDBLTFAST_SRCCOLORKEY );
	}   
	//界面
	for(j=0;j<2;j++){
		temp1=&Interface_Common[j];
		rect=GetRscRect(0,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
		DDraw.BlitBack(temp1->pos.x,temp1->pos.y,Interface[j],&rect,0 );
		if(j==0)
			Text.TextBackSurf(temp1->pos.x+30,temp1->pos.y,player[0].gold,RGB(128,128,255),0,0);
		else if(j==1){
			Text.TextBackSurf(temp1->pos.x+30,temp1->pos.y,player[0].population,RGB(128,128,255),0,0);
			Text.TextBackSurf(temp1->pos.x+30,temp1->pos.y+15,player[1].population,RGB(128,128,255),0,0);
			Text.TextBackSurf(temp1->pos.x+45,temp1->pos.y,"/",RGB(255,128,255),0,0);
			Text.TextBackSurf(temp1->pos.x+60,temp1->pos.y,(LONG)population_limit,RGB(255,128,255),0,0);			
		}
	}

	//select框
	if(select_draw){
		RectangleFrame(start_select,end_select);//画矩形框
	}

    //>>>>> 文字提示的显示 >>>>>
	for(i=0;i<2;i++){//player各兵种人口显示
		for (j=0;j<5;j++)
		{
			Text.TextBackSurf(5+15*j,5+15*i,player[i].count[j],RGB(128,128,255),0,0);
		}
	}
	for (j=0;j<6;j++)//AI策略显示
	{
		Text.TextBackSurf(5+15*j,35,player[1].strategy[j],RGB(128,128,255),0,0);
	}
	//select显示
	rect=GetRect(0,0,46,38);
	if(select.size() == 1){		
		temp = returnUnit(select[0]);
		if(temp->bDead == TRUE|| temp == NULL) //对象死亡
		{
		}
		else {
			temp1 = returnUnitCommon(select[0]);			
			if(select[0].Bui_Sol==0 && select[0].player!=2){//建筑
				Text.TextBackSurf(ScreenW-120,10,Bui[player[select[0].player].race][select[0].kind],RGB(255,128,255),0,0);
				DDraw.BlitBack(SelectRECT[0].left,SelectRECT[0].top,temp1->lpIcon,&rect,0 );
				if(temp->state == UnitActionNone){
					Text.TextBackSurf(ScreenW-190,50,"HP:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-160,50,temp->HP,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-115,50,"/",RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-90,50,temp1->Hp,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-190,65,"Armor:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-120,65,temp1->defense,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-190,80,"Armor_Kind:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-100,80,Defense_Kind[temp1->defense_kind],RGB(255,128,255),0,0);
					if(select[0].player==0){
						DDraw.BlitBack(SelectRECT[8].left,SelectRECT[8].top,People_Common[player[0].race][temp1->abletoproduct].lpIcon,&rect,0 );
						Text.TextBackSurf(ScreenW-140,130,"Gold_Cost:",RGB(128,128,255),0,0);
					    Text.TextBackSurf(ScreenW-60,130,People_Common[player[0].race][temp1->abletoproduct].gold_cost,RGB(128,128,255),0,0);
						Text.TextBackSurf(ScreenW-140,150,"Pop_Cost:",RGB(128,128,255),0,0);
					    Text.TextBackSurf(ScreenW-60,150,People_Common[player[0].race][temp1->abletoproduct].population_cost,RGB(128,128,255),0,0);
					}
				}
				else if(temp->state == UnitActionTrain){
					if(select[0].player==0){
					FLOAT rate = (FLOAT)(Time-temp->newTime)/temp1->train_time;
					DrawBloodBar(600,60,150,20,rate);
					LONG _rate = rate*100;
					Text.TextBackSurf(675,60,_rate,RGB(128,128,255),0,0);
					Text.TextBackSurf(700,60,"%",RGB(128,128,255),0,0);
				}
					else{
					Text.TextBackSurf(ScreenW-190,50,"HP:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-160,50,temp->HP,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-115,50,"/",RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-90,50,temp1->Hp,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-190,65,"Armor:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-120,65,temp1->defense,RGB(255,128,255),0,0);
					Text.TextBackSurf(ScreenW-190,80,"Armor_Kind:",RGB(128,128,255),0,0);
					Text.TextBackSurf(ScreenW-100,80,Defense_Kind[temp1->defense_kind],RGB(255,128,255),0,0);
					}
				}
			}
			else if(select[0].Bui_Sol==1 && select[0].player!=2)//士兵
			{
				DDraw.BlitBack(SelectRECT[0].left,SelectRECT[0].top,temp1->lpIcon,&rect,0 );
				Text.TextBackSurf(ScreenW-190,50,"HP:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-160,50,temp->HP,RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-115,50,"/",RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-90,50,temp1->Hp,RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-190,65,"Armor:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-120,65,temp1->defense,RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-190,80,"Armor_Kind:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-100,80,Defense_Kind[temp1->defense_kind],RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-190,95,"Attack:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-120,95,temp1->attackpower,RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-190,110,"Attack_Kind:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-90,110,Attack_Kind[temp1->attack_kind],RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-190,125,"Sight:",RGB(128,128,255),0,0);
				Text.TextBackSurf(ScreenW-120,125,temp1->sight,RGB(255,128,255),0,0);
				Text.TextBackSurf(ScreenW-120,10,Sol[player[select[0].player].race][select[0].kind],RGB(255,128,255),0,0);	
			}
			else//金矿
			{
				Text.TextBackSurf(ScreenW-120,10,"金矿",RGB(255,128,255),0,0);
			}
		}
	}
	else{
		for(i=0;i<select.size();i++){
			temp = returnUnit(select[i]);
			if(temp->bDead == TRUE|| temp == NULL) //攻击对象死亡
			{
				continue;
			}
			temp1 = returnUnitCommon(select[i]);
			if(temp1==NULL)
				continue;
			w=temp1->eachWide;
			h=temp1->eachHeight;			
			if(select[i].player!=2 &&select[i].Bui_Sol==1){
				w /=2;
				h /=2;
			}		
			if(temp->uniform_identifier == select[i].uniform_identifier){
				POINT a,b;
				a.x= temp->Pos.x-w/2;
				a.y= temp->Pos.y-h/2;
				b.x= temp->Pos.x+w/2;
				b.y= temp->Pos.y+h/2;
				//RectangleFrame(a,b);//画矩形框
			}
			DDraw.BlitBack(SelectRECT[i].left,SelectRECT[i].top,temp1->lpIcon,&rect,0 );
			LONG length = SelectRECT[i].right-SelectRECT[i].left;
			FLOAT rate = (FLOAT)temp->HP/temp1->Hp;
			DrawBloodBar(SelectRECT[i].left,SelectRECT[i].bottom,length,3,rate);
		}
	}    
}
/***********************************************
*   函数名: CheckMessage(...)
*     功能: 检测角色消息
*       (c) jk&ljq, 2010
************************************************/
void CGame::CheckMessage( void )
{
	if( KeyKB[1].Key==DIK_ESCAPE &&
		KeyKB[0].Key==NULL )
	{
		//回到开始画面
		lAppState = 0;
		//删除旧的和当前的场景
		DeleteSceneData(map);
		Sound.StopStream(1);
	}	
	if( KeyKB[1].Key==DIK_S &&
		KeyKB[0].Key==NULL )//错误，建议去掉
	{
		//保存
		SaveSceneData( TRUE );
		KeyKB[1].Key=NULL;
		return;
	}
	if( KeyKB[1].Key==DIK_1 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 1编队
		vector<SelectUnit> temp_vector(select);
		select_1.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_1 &&
		KeyKB[0].Key==NULL )
	{
		//选取 1编队
		//AdjustSelect(&select_1);
		vector<SelectUnit> temp_vector(select_1);
		select.swap(temp_vector);		
		return;
	}
	if( KeyKB[1].Key==DIK_2 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 2编队
		vector<SelectUnit> temp_vector(select);
		select_2.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_2 &&
		KeyKB[0].Key==NULL )
	{
		//选取2编队
		//AdjustSelect(&select_2);
		vector<SelectUnit> temp_vector(select_2);
		select.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_3 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 3编队
		vector<SelectUnit> temp_vector(select);
		select_3.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_3 &&
		KeyKB[0].Key==NULL )
	{
		//选取 3编队
		//AdjustSelect(&select_3);
		vector<SelectUnit> temp_vector(select_3);
		select.swap(temp_vector);
		return;
	}
	//>>>>>> 检测鼠标指针是否超出场景范围>>>>
    BOOL bMouseOutSce=FALSE;
    if( MouseScePos.x<0   ||
        MouseScePos.x >map.SceneW ||
        MouseScePos.y<0   ||
        MouseScePos.y >map.SceneH )
        bMouseOutSce = TRUE;
	
    //左击键选择对象
	SelectUnit temp1;
	Unit *temp;
	LONG i,w,h;
	Unit_Common *temp2;
    if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 )
    {
		i = CheckIcon(KeyMouse[1].Pos);		
		//检测建筑生产消息
		if(i==8 && select.size()==1 && select[0].Bui_Sol==0 && select[0].player==0 ){
			temp = returnUnit(select[0]);
			temp2 = returnUnitCommon(select[0]);
			Unit_Common *temp3 = &People_Common[player[0].race][temp2->abletoproduct];
			if(temp != NULL && temp2 != NULL && temp->state == UnitActionNone){
				if(player[0].population+temp3->population_cost <=population_limit
					&& player[0].gold >= temp3->gold_cost){
					temp->state = UnitActionTrain;
					temp->newTime = Time;
					player[0].gold -= temp3->gold_cost;
					player[0].population += temp3->population_cost;
					Count.playerSolProduce[temp->kind]++;
				}
				else if(player[0].gold < temp3->gold_cost){
				Sound.PlayStatic(63+player[0].race,0,1);
				}
				else if(player[0].population+temp3->population_cost >population_limit){
				Sound.PlayStatic(65+player[0].race,0,1);
				}
			}
		}
		//检测选中select
		else if(i!=-1){
			if(i<select.size()){
				temp1 = select[i];
				select.clear();
				select.push_back(temp1);
				return;
			}
		}
		//检测点击非图标
		else {
			select.clear();
			temp1 = returnSelect(MouseScePos);
			if(temp1.uniform_identifier!=-1){
				select.push_back(temp1);
				playsound(temp1,0);
				return;
			}
		}
    }	
	//>>>>>>>>>按下鼠标右键行走攻击>>>>>>>>
    else if( KeyMouse[0].Key == DIMOFS_BUTTON1 &&
             !bMouseOutSce )
    {
		temp1 = returnSelect(MouseScePos);
		//select
		for(i=0;i<select.size();i++){
			if(!CheckUI(select[i]))//统一标志不对
				select[i]=AdjustUI(select[i]);//调整
			if(select[i].uniform_identifier == -1) //调整后发现该单位已不存在
				continue;
			if(select[i].player ==2){//中立
				
			}
			else if(select[i].Bui_Sol==0){//建筑
				
			}
			else{//士兵
				if((select[i].number < player[select[i].player].People.size())&& select[i].player==0 ){//控制自己的单位
					temp=&player[select[i].player].People[select[i].number];
					temp2=&People_Common[player[select[i].player].race][temp->kind];
					w=temp2->eachWide/2;
					h=temp2->eachHeight/2;
					if(temp1.uniform_identifier == -1){//点击的是地面
						temp->state = UnitActionMove;
						temp->Target=MouseScePos;						
						temp->lStartIndex = temp2->start_run;
						temp->lEndIndex = temp2->end_run;	
						playsound(select[i],1);
					}
					else {
						if(temp1.player==1){//选中敌人
							temp->state = UnitActionAttack;
							temp->lStartIndex = temp2->start_attack;
							temp->lEndIndex = temp2->end_attack;	
							//Unit temp3 = returnUnit(select[i]);
							Unit *temp4 = returnUnit(temp1);
							if(temp4->bDead==TRUE || temp4 == NULL){
								continue;
							}
							playsound(select[i],2);
							LONG distance = GPa.Distance(temp->Pos,temp4->Pos);
							if(distance>=temp2->range){ //距离太远 无法攻击
								temp->state = UnitActionAttackFollow;
								temp->lStartIndex = temp2->start_run;
								temp->lEndIndex = temp2->end_run;	
								playsound(select[i],2);
							}
						}
						if(temp1.player==0){//选中自己单位
							temp->state = UnitActionFollow;
							temp->lStartIndex = temp2->start_run;
							temp->lEndIndex = temp2->end_run;
						    playsound(select[i],1);
						}
						temp->Target_Select = temp1;
					}
					temp->Direct = GetDirection( temp->Pos,MouseScePos);
					temp->oldDirect = temp->Direct;
					temp->startTime =Time;
					temp->pic_Index = temp->Direct+temp->lStartIndex;
				}
				else if(select[i].player==1){//别人的单位
					select.clear();
				}
			}
		}   
	}

	//鼠标拖出选择框
	if( KeyMouse[0].Key==DIMOFS_BUTTON0 &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 && !select_draw)
    {
		start_select = MouseScePos;
		select_draw = TRUE;
		end_select = MouseScePos;
    }
	else if( KeyMouse[0].Key==DIMOFS_BUTTON0 &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 && select_draw)
    {
		end_select = MouseScePos;
		return;
    }
	else if(KeyMouse[0].Key==NULL && select_draw)
    {
		end_select = MouseScePos;
		select_draw = FALSE;
		selectInFrame(start_select,end_select);
    }
}
/***********************************************
*   函数名: CheckAIMessage(...)
*     功能: 检测人工智能消息
计算攻击优先度  
对于士兵：距离×0.7+血量×0.3 越小优先攻击
对于建筑：距离×0.9+血量×0.1 越小优先攻击
************************************************/
void CGame::CheckAIMessage( void ){
	LONG threattemp,race,distance,count;
	LONG threat= 9999;
	Unit_Common *temp1;
	SelectUnit  temp2;
	//检查自己的人工智能：进入攻击范围自动攻击
	vector<Unit>::iterator itr0 = player[0].People.begin();
	vector<Unit>::iterator itr1;
	race = player[0].race;
	while (itr0 != player[0].People.end())
	{
		threat= 9999;
		if(itr0->bDead == FALSE && itr0->state == UnitActionNone){
			itr1 = player[1].People.begin();
			count=0;
			while (itr1 != player[1].People.end()){
				if(itr1->bDead == FALSE){
					temp1 = &People_Common[race][itr0->kind];
					distance = GPa.Distance(itr0->Pos,itr1->Pos);
					//进入攻击范围
					if(temp1->sight >= distance){ 
						threattemp = distance*0.7+ itr1->HP*0.3;
						//优先度高
						if(threattemp< threat){
							temp2.player = 1;
							temp2.number = count;
							temp2.Bui_Sol = 1;
							temp2.uniform_identifier = itr1->uniform_identifier;
							temp2.kind = itr1->kind;
							itr0->Target_Select = temp2;
							itr0->state = UnitActionAttackFollow;
							threat = threattemp;
						}
					}
				}
				count++;
				itr1++;
			}
			//没有人可以攻击时检测建筑
			if(itr0->state == UnitActionNone)
			{
				itr1 = player[1].Building.begin();
				count=0;
				while (itr1 != player[1].Building.end()){
					if(itr1->bDead == FALSE){
						temp1 = &People_Common[race][itr0->kind];
						distance = GPa.Distance(itr0->Pos,itr1->Pos);
						//进入攻击范围
						if(temp1->sight >= distance){ 
							threattemp = distance*0.9+ itr1->HP*0.1;
							//优先度高
							if(threattemp< threat){
								temp2.player = 1;
								temp2.number = count;
								temp2.Bui_Sol = 0;
								temp2.uniform_identifier = itr1->uniform_identifier;
								temp2.kind = itr1->kind;
								itr0->Target_Select = temp2;
								itr0->state = UnitActionAttackFollow;
								threat = threattemp;
							}
						}
					}
					count++;
					itr1++;
				}
				
			}
		}
		itr0++;
	}

	//检查电脑的人工智能
	//状态监测
	if(player[1].state == 1  && player[1].population<=8){
		player[1].state = 0;//停止进攻  开始发展
	}
	if(player[1].state == 0  && player[1].population-player[0].population>=8){
		player[1].state = 1;//全部进攻
		player[1].ArmyPos = CalArmyBuildingCenterPos(0);
	}
	DWORD dif_time = Time - game_start_time;
	if(player[1].strategy[5]!=2 && dif_time > 360000){//载入策略2
		setStrategy(1,2);
	}
	else if (player[1].strategy[5]!=2 &&
		player[1].strategy[5]!=1 && dif_time>180000)//载入策略1
	{
		setStrategy(1,1);
	}
	//动态修改策略
	if (player[0].count[0]>2 )
	{
		//巨魔克农民
		player[1].strategy[4] = player[0].count[0];
	}
	if (player[0].count[1]>5)
	{
		//骑士克步兵
		player[1].strategy[2] = player[0].count[1];
	}
	if (player[0].strategy[2]>5)
	{
		//魔法师克骑士
		player[1].strategy[3] = player[0].count[2];
	}
	if (player[0].count[3]>5 || player[0].count[4]>5)
	{
		//步兵克魔法师，巨魔,弓箭手
		player[1].strategy[1] = player[0].count[3]+player[0].count[4];
	}
	//电脑士兵AI：进入攻击范围自动攻击
	race = player[1].race;
	itr1 = player[1].People.begin();
	BOOL flag = FALSE;
	while (itr1 != player[1].People.end())
	{
		threat= 9999;
		if(itr1->state == UnitActionAttackFollow || itr1->state == UnitActionAttack){//攻击目标为建筑
			if( itr1->Target_Select.Bui_Sol==0 && itr1->Target_Select.player!=2)
				flag = TRUE;
		}
		if(itr1->bDead == FALSE && 
			((itr1->state != UnitActionAttackFollow && itr1->state!=UnitActionAttack) || flag)){
			itr0 = player[0].People.begin();
			count=0;
			while (itr0 != player[0].People.end()){
				if(itr0->bDead == FALSE){
					temp1 = &People_Common[race][itr1->kind];
					distance = GPa.Distance(itr0->Pos,itr1->Pos);
					//进入攻击范围
					if(temp1->sight >= distance){ 
						threattemp = distance*0.7+ itr0->HP*0.3;
						//优先度高
						if(threattemp< threat){
							temp2.player = 0;
							temp2.number = count;
							temp2.Bui_Sol = 1;
							temp2.uniform_identifier = itr0->uniform_identifier;
							temp2.kind = itr0->kind;
							itr1->Target_Select = temp2;
							itr1->state = UnitActionAttackFollow;
							threat = threattemp;
						}
					}
				}
				count++;
				itr0++;
			}
			//没有人可以攻击时检测建筑
			if(itr1->state == UnitActionNone)
			{
				itr0 = player[0].Building.begin();
				count=0;
				while (itr0 != player[0].Building.end()){
					if(itr0->bDead == FALSE){
						temp1 = &People_Common[race][itr1->kind];
						distance = GPa.Distance(itr0->Pos,itr1->Pos);
						//进入攻击范围
						if(temp1->sight >= distance){ 
							threattemp = distance*0.9+ itr0->HP*0.1;
							//优先度高
							if(threattemp< threat){
								temp2.player = 0;
								temp2.number = count;
								temp2.Bui_Sol = 0;
								temp2.uniform_identifier = itr0->uniform_identifier;
								temp2.kind = itr1->kind;
								itr1->Target_Select = temp2;
								itr1->state = UnitActionAttackFollow;
								threat = threattemp;
							}
						}
					}
					count++;
					itr0++;
				}
			}
		}
		if(itr1->state == UnitActionNone && player[1].state ==1){
			itr1->state = UnitActionMove;
			itr1->Target = player[1].ArmyPos;
		}
		itr1++;
	}
	//电脑建筑AI
	Unit_Common *temp3=NULL;
	itr1 = player[1].Building.begin();
	while (itr1 != player[1].Building.end())
	{
		if(itr1->bDead == FALSE && itr1->state == UnitActionNone ){
			temp1 = &Building_Common[race][itr1->kind];
			temp3 = &People_Common[race][temp1->abletoproduct];
			if(player[1].population+temp3->population_cost<=population_limit &&
				player[1].gold >= temp3->gold_cost
				&& player[1].count[temp1->abletoproduct]<player[1].strategy[temp1->abletoproduct]){
				itr1->state = UnitActionTrain;
				itr1->newTime = Time;
				player[1].population += temp3->population_cost;
				player[1].gold -=temp3->gold_cost;
				Count.AISolProduce[temp1->abletoproduct]++;
			}
		}
		itr1++;
	}
}
/************************************************************
*   函数名: MessageProcess(...)
*     功能: 处理角色消息
*************************************************************/
void CGame::MessageProcess( void )
{
	if(Time-oldTime > goldup_time){
		if(player[0].gold < gold_limit)
			player[0].gold += 10;
		if(player[1].gold < gold_limit)
			player[1].gold += 10;
		oldTime = Time;
	}
	LONG i,w,h,dis;
	Unit_Common *temp2;
	vector<Unit>::iterator itr0 ;
	for(i=0;i<2;i++){//player
		//建筑
		itr0 = player[i].Building.begin();
		for(itr0 = player[i].Building.begin(); itr0!=player[i].Building.end(); ++itr0){
			temp2 = &Building_Common[player[i].race][itr0->kind];
			w = temp2->eachWide/2;
			h = temp2->eachHeight/2;
			if(itr0->state == UnitActionTrain && Time-itr0->newTime >= temp2->train_time){
				Unit temp;
				ZeroMemory( &temp, sizeof(Unit) );
				temp.state =UnitActionMove;
				temp.Direct =0;
				temp.bDead=FALSE;
				temp.pic_Index=0;
				temp.kind = temp2->abletoproduct;
				Unit_Common *temp3 = &People_Common[player[i].race][temp2->abletoproduct];
				player[i].count[temp2->abletoproduct]++;
				temp.HP = temp3->Hp;
				temp.Pos.x = itr0->Pos.x;
				temp.Pos.y = itr0->Pos.y+h+16;
				temp.Target.x = itr0->Pos.x;
				temp.Target.y = itr0->Pos.y +h+64;
				temp.uniform_identifier = UI;
				UI++;
				player[i].People.push_back(temp);
				//player[i].population += temp3->population_cost;
				itr0->state = UnitActionNone;
			}				
		}
		//士兵
		itr0 = player[i].People.begin();
		for(itr0 = player[i].People.begin(); itr0!=player[i].People.end(); ++itr0){
			temp2 = &People_Common[player[i].race][itr0->kind];
			w = temp2->eachWide/2;
			h = temp2->eachHeight/2;
			dis=0;
			//移动或追踪
			if(((itr0->state == UnitActionMove)||(itr0->state==UnitActionAttackFollow)||(itr0->state==UnitActionFollow))
				&& (Time-itr0->startTime >= temp2->movespeed)){
				if(itr0->state==UnitActionAttackFollow || itr0->state==UnitActionFollow){//跟踪  检测距离
					Unit *temp3 = returnUnit(itr0->Target_Select);
					if(temp3->bDead==TRUE || temp3 == NULL){
						itr0->state = UnitActionNone;
						continue;
					}
					LONG distance = GPa.Distance(temp3->Pos,itr0->Pos);
					if(itr0->Target_Select.player==2 ){//若攻击的为中立  距离加长
						Unit_Common *temp4 = &Neutral_Common[temp3->kind];
						dis = (temp4->eachHeight+temp4->eachWide)/4;
					}
					else if(itr0->Target_Select.Bui_Sol==0){//若攻击的为建筑  距离加长
						Unit_Common *temp4 = &Building_Common[itr0->Target_Select.player][itr0->Target_Select.number];
						dis = (temp4->eachHeight+temp4->eachWide)/4;
					}
					itr0->Target = temp3->Pos;
					if(distance<temp2->range+dis){ //距离足够近 开始攻击或停下												
						itr0->Direct = GetDirection( itr0->Pos,temp3->Pos);
						itr0->oldDirect = itr0->Direct;						
						if(itr0->state==UnitActionFollow){
							itr0->state = UnitActionNone;
							itr0->lStartIndex = temp2->start_run;
							itr0->lEndIndex = temp2->end_run;	
						}
						else{
							itr0->state = UnitActionAttack;
							itr0->lStartIndex = temp2->start_attack;
							itr0->lEndIndex = temp2->end_attack;
							itr0->startTime =Time;
						}
						itr0->pic_Index = itr0->Direct+itr0->lStartIndex;
						continue;
					}
				}
				//end 跟踪  检测距离
				GPa.Set_Square((VOID*)map.Square);
				itr0->oldDirect = itr0->Direct;
				itr0->Direct = GetDirection( itr0->Pos,itr0->Target);				
				map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 0;//将所处的方块恢复为非障碍
				itr0->lpPath = GPa.GetTheBestPath(itr0->Pos, itr0->Target, 1);
				if(itr0->lpPath == NULL ){
					itr0->state = UnitActionNone;
					itr0->pic_Index = itr0->oldDirect;
					map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 1;//将所处的方块设为障碍
					//player[i].People[j] = temp;
					continue;
				}
				if(itr0->oldDirect != itr0->Direct){  //更换图片类型
					itr0->lStartIndex = temp2->start_run;
					itr0->lEndIndex = temp2->end_run;
					itr0->pic_Index = itr0->Direct+itr0->lStartIndex;
				}
				itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);
				itr0->Pos =  GPa.AdjustPos(itr0->lpPath->Pos);
				itr0->startTime = Time;
				map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 1;//将所处的方块设为障碍				
				GPa.DeletePath( itr0->lpPath );
				//player[i].People[j] = temp;
			}
			//攻击
			else if(itr0->state == UnitActionAttack && (Time-itr0->startTime >= temp2->attackspeed)){	
				itr0->oldDirect = itr0->Direct;
				Unit *temp3 = returnUnit(itr0->Target_Select);
				if(temp3 == NULL || temp3->bDead == TRUE){
					itr0->state = UnitActionNone;
					continue;
				}
				LONG distance = GPa.Distance(temp3->Pos,itr0->Pos);
				if(itr0->Target_Select.player==2 ){//若攻击的为中立
					Unit_Common *temp4 = &Neutral_Common[temp3->kind];
					distance =0;
				}
				else if(itr0->Target_Select.Bui_Sol==0){//若攻击的为建筑
					distance =0;
				}
				if(distance>temp2->range){
					itr0->state = UnitActionAttackFollow;
					continue;
				}
				itr0->Direct = GetDirection( itr0->Pos,temp3->Pos);
				if(itr0->oldDirect != itr0->Direct){  //更换图片类型
					itr0->lStartIndex = temp2->start_attack;
					itr0->lEndIndex = temp2->end_attack;
					itr0->pic_Index = itr0->Direct+itr0->lStartIndex;
				}
				itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);
				if(itr0->lEndIndex-itr0->pic_Index<8){//伤害
					LONG dis=soundDis(ScrPos,itr0->Pos);
					Sound.SetStaticVolume(People_Common[player[i].race][itr0->kind].sound[3],0-dis);
					Sound.PlayStatic(People_Common[player[i].race][itr0->kind].sound[3],0,1);
					//近程单位
					if(temp2->range<100){
						if(setDamage(itr0->Target_Select,temp2->attackpower,temp2->attack_kind)){
							itr0->state = UnitActionNone;
							itr0->pic_Index = itr0->Direct;
						}
					}
					//远程单位
					else {
						Missile tempmissile;
						tempmissile.kind  = temp2->missile_kind;
						tempmissile.select = itr0->Target_Select;
						tempmissile.pos = itr0->Pos;
						tempmissile.startTime = Time;
						Unit *a= returnUnit(itr0->Target_Select);
						if(a == NULL)
							continue;
						tempmissile.target_pos = a->Pos;
						tempmissile.attack_kind = temp2->attack_kind;
						tempmissile.attackpower = temp2->attackpower;
						MissileQueue.push_back(tempmissile);
					}
				}
				itr0->startTime = Time;
			}				
		}
	}
	//导弹
	vector<Missile>::iterator itr = MissileQueue.begin();
	for( itr = MissileQueue.begin(); itr!=MissileQueue.end(); ++itr )
	{
		if(Time-itr->startTime >=50){
			Unit *temp = returnUnit(itr->select);
			if(temp == NULL || temp->bDead == TRUE){
				MissileQueue.erase(itr);
				itr--;
				continue;
			}	
			//未击中
			if(GPa.Distance(itr->pos,temp->Pos) >16){ 
				LONG Direct = GetDirection( itr->pos,temp->Pos);
				if((itr->kind ==0 || itr->kind == 1)
					&& Direct != itr->oldDirect){
					itr->pic_Index = Direct;
					itr->oldDirect = Direct;					
				}
				else if(itr->kind ==2 || itr->kind == 3){
					Unit_Common missilecom = Missile_Common[itr->kind];
					itr->pic_Index = DoFor(itr->pic_Index,missilecom.end_attack,missilecom.start_attack,Direct);
				}
				itr->pos = OneStep(itr->pos,Direct);
			}
			//击中目标
			else{
				setDamage(itr->select,itr->attackpower,itr->attack_kind);
				MissileQueue.erase(itr);
				itr--;
				continue;
			}
			itr->startTime = Time;
		}
	} 
	itr0 = DeadQueue.begin();
	for( itr0 = DeadQueue.begin(); itr0!=DeadQueue.end(); ++itr0 )
	{
		if(Time-itr0->startTime >= blet_time){//死亡腐烂
			itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);	
			itr0->startTime = Time;
		}
	}   
}
/*********************************************************
*   函数名: CheckWin(...) 
*     功能: 检测胜利
**********************************************************/
void CGame::CheckWin(){
	if(player[1].Building.size()==0)
		lAppState = 4;
	if(player[0].Building.size()==0)
		lAppState = 4;
}
/************************************************************
*   函数名: DeadProcess(...)
*     功能: 死后处理
*************************************************************/
void CGame::DeadProcess( void )
{
    //按下“ESC”键重新开始
   //按下“ESC”键重新开始
    if( KeyKB[1].Key==DIK_ESCAPE &&
        KeyKB[0].Key==NULL )
    {
        //回到开始画面
        lAppState = 0;
        //删除旧的和当前的场景
        DeleteSceneData(map);
		Sound.StopStream(2);
        Sound.StopStream(3);
    }
	else {
		//胜利
	   LONG AllplayerSolProduce=0;
	   LONG AllplayerSolLost=0;
       LONG AllplayerBuiLost=0;
	   LONG AllAISolProduce=0;
	   LONG AllAISolLost=0;
	   LONG AllAIBuiLost=0;
	   LONG AllPlayerScore=0;
	   LONG AllAIScore=0;
	   LONG ScorePlayer[2][5]={{75,135,345,215,205},{385,160,120,140,80}};
	   LONG ScoreAI[2][5]={{75,135,205,245,155},{385,160,120,140,80}};
	   for(int j=0;j<5;j++){
		   AllplayerSolProduce+=Count.playerSolProduce[j];
		   AllplayerSolLost+=Count.playerSolLost[j];
		   AllplayerBuiLost+=Count.playerBuiLost[j];
	       AllAISolProduce+=Count.AISolProduce[j];
	       AllAISolLost+=Count.AISolLost[j];
	       AllAIBuiLost+=Count.AIBuiLost[j];
		   AllPlayerScore+=Count.playerSolProduce[j]*ScorePlayer[0][j];
		   AllPlayerScore-=Count.playerSolLost[j]*ScorePlayer[0][j]/2;
		   AllPlayerScore-=Count.playerBuiLost[j]*ScorePlayer[1][j];
		   AllAIScore+=Count.AISolProduce[j]*ScoreAI[0][j];
		   AllAIScore-=Count.AISolLost[j]*ScoreAI[0][j]/2;
		   AllAIScore-=Count.AIBuiLost[j]*ScoreAI[1][j];
	   }
		if(player[1].Building.size()==0){
		DDraw.BlitBack(0,0,Win_Lose[player[0].race][0],NULL,0);
	    Text.TextBackSurf(150,50,"You Win!!!",RGB(255,0,0),0,50);
		Sound.StopStream(1);
		Sound.PlayStream(2,1);
	}
	//失败
	if(player[0].Building.size()==0){
		DDraw.BlitBack(0,0,Win_Lose[player[0].race][1],NULL,0);
        Text.TextBackSurf(150,100,"You Lose!!!",RGB(255,0,0),0,50);
		Sound.StopStream(1);
	    Sound.PlayStream(3,1);
	}	
	    RECT rect;
		for(int i=2;i<7;i++){
	    rect=GetRscRect(i-2,Interface_Common[i].eachWide,Interface_Common[i].eachHeight,Interface_Common[i].lSrcWCount);
		DDraw.BlitBack(Interface_Common[i].pos.x,Interface_Common[i].pos.y,Interface[i],&rect,0 );}
		rect=GetRscRect(0,Interface_Common[7].eachWide,Interface_Common[7].eachHeight,Interface_Common[7].lSrcWCount);
		DDraw.BlitBack(Interface_Common[7].pos.x,Interface_Common[7].pos.y,Interface[7],&rect,0 );
	    Text.TextBackSurf(150,200,"士兵生产",RGB(255,0,0),0,0);
		Text.TextBackSurf(250,200,"士兵死亡",RGB(0,0,139),0,0);
		Text.TextBackSurf(350,200,"建筑丢失",RGB(230,230,250),0,0);
		Text.TextBackSurf(450,200,"士兵击杀",RGB(119,136,153),0,0);
		Text.TextBackSurf(550,200,"建筑摧毁",RGB(255,69,0),0,0);
		Text.TextBackSurf(650,200,"总得分",RGB(64,224,208),0,0);
		Text.TextBackSurf(50,300,"Player",RGB(64,224,208),0,0);
		Text.TextBackSurf(150,300,AllplayerSolProduce,RGB(255,0,0),0,0);
		Text.TextBackSurf(250,300,AllplayerSolLost,RGB(0,0,139),0,0);
		Text.TextBackSurf(350,300,AllplayerBuiLost,RGB(230,230,250),0,0);
		Text.TextBackSurf(450,300,AllAISolLost,RGB(119,136,153),0,0);
		Text.TextBackSurf(550,300,AllAIBuiLost,RGB(255,69,0),0,0);
		Text.TextBackSurf(650,300,AllPlayerScore,RGB(64,224,208),0,0);
	    Text.TextBackSurf(50,400,"AI",RGB(64,224,208),0,0);
		Text.TextBackSurf(150,400,AllAISolProduce,RGB(255,0,0),0,0);
		Text.TextBackSurf(250,400,AllAISolLost,RGB(0,0,139),0,0);
		Text.TextBackSurf(350,400,AllAIBuiLost,RGB(230,230,250),0,0);
		Text.TextBackSurf(450,400,AllplayerSolLost,RGB(119,136,153),0,0);
		Text.TextBackSurf(550,400,AllplayerBuiLost,RGB(255,69,0),0,0);
		Text.TextBackSurf(650,400,AllAIScore,RGB(64,224,208),0,0);
		Text.TextBackSurf(100,510,"Press ESC to Restart",RGB(255,0,0),0,30);
	} 
}

/********************************************
*   函数名: GameProcess(...)
*     功能: 游戏进程
*       (c) jk&ljq, 2010
*********************************************/
void CGame::GameProcess( void )
{
    //更新时间
    Time = GetTickCount();
    //>>>>>> 程序的开始进程 >>>>>>
    if( lAppState==0 )
    {
        //程序的开头
        Header();
        if( lAppState==1 || lAppState==2 )
        {//进入游戏
			Sound.StopStream(0);
            MakeScene();
            if(lAppState!=0){
			lAppState = 3;
			MakeGround();
			MakeEntity();
			setStrategy(1,0);
			game_start_time = Time;
			Text.ReMain();
			}
        }
    }
    //>>>>> 程序的主进程 >>>>>>>>
    else if( lAppState == 3 ){
		Sound.PlayStream(1,1);
		//检测鼠标位置
		CheckScrPos();
		//检测鼠标响应消息
		CheckMessage();
		//检测AI消息
		CheckAIMessage();
		//决策
        MessageProcess();
		//个单位状态检测
        RestoreVariable();
        //将地面显示到后台表面
		MakeGround();
        //显示单位
		MakeEntity();
        //文字的显示
		Text.ReMain();
		//检测胜利消息
		CheckWin();
		if( KeyKB[1].Key==DIK_ESCAPE &&
			KeyKB[0].Key==NULL )
		{
			//回到开始画面
			lAppState = 0;
			//删除旧的和当前的场景
			DeleteSceneData(map);
			Sound.StopStream(1);
		}		

    }
    //>>>>> 角色死亡进程 >>>>>>>>
    else if( lAppState==4 )
    {
        DeadProcess();
        //文字的显示
        Text.ReMain();
    }
    //>>>>>> 退出程序 >>>>>>>
    else//lAppState==5
        DestroyWindow( hWnd );
}

/***************************************************
*   函数名: GetInputDeviceData(...)
*     功能: 获得输入设备的状态
*       (c) jk&ljq, 2010
****************************************************/
void CGame::GetInputDeviceData( LPRECT lpClientRt )
{
    //>>>> 获得键盘和鼠标输入状态 >>>>
    Input.GetDInputData();

    //>>>> 调整鼠标位置的值 >>>>>>>
    if( !bFullScr )//是窗口模式则调整鼠标位置
    {
        KeyMouse[0].Pos.x -= lpClientRt->left;
        KeyMouse[0].Pos.y -= lpClientRt->top;
    }
    //横向边界限制
    if( KeyMouse[0].Pos.x > ScreenW-8 )
        KeyMouse[0].Pos.x = ScreenW-8;
    else if( KeyMouse[0].Pos.x < 0 )
        KeyMouse[0].Pos.x = 0;
    //纵向边界限制
    if( KeyMouse[0].Pos.y > ScreenH-6 )
        KeyMouse[0].Pos.y = ScreenH-6;
    else if( KeyMouse[0].Pos.y < 0 )
        KeyMouse[0].Pos.y = 0;
    //在场景中的位置
    MouseScePos.x = KeyMouse[0].Pos.x+ScrPos.x;
    MouseScePos.y = KeyMouse[0].Pos.y+ScrPos.y;
}

/************************************************************
*   函数名: UpdateScreen(...)
*     功能: 更新屏幕
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::UpdateScreen( LPRECT lpClientRect )
{
    DDraw.Flip( lpClientRect );
}
/*******************************************************
* 函数名 : GetRscRect(...)
*   功能 : 根据动作编号取得动作在源图片表面的矩形位置
*   参数 : lIndex  要求的编号
*          lWidth  每个动作图片的宽
*          lHeight 每个动作图片的高
*          lCount  每行有多少个图片
********************************************************/
inline RECT GetRscRect( LONG lIndex, LONG lWidth,
                           LONG lHeight, LONG lCount )
{
    RECT rect;  LONG x,y;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //求出在第几行(0,1,2,3,...从上到下)
    y = (LONG)( lIndex/lCount );

    //求出在行的第几个(0,1,2,3,...从左到右)
    x = lIndex - y*lCount;

    rect.left	= x*lWidth;
    rect.top	= y*lHeight;

    rect.right	= rect.left + lWidth;
    rect.bottom	= rect.top + lHeight;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    return rect;
}
/**************************************************
*   函数名: DeleteSelect(...)
*     功能: 删除指定的对象
*       (c) jk&ljq, 2010
***************************************************/
void CGame::DeleteSelect(LONG uniform_temp )
{
    vector<SelectUnit>::iterator itr = select.begin();
	while (itr != select.end())
	{
		if (itr->uniform_identifier == uniform_temp){
			select.erase(itr);
			break;
		}
		++itr;
	}
}
/*******************************************************
* 函数名 : SetRect(...)
*   功能 : 设置矩形内的地形数组
*   参数 : left  地图位置
*          top  地图位置
*          Width 图片宽
*          Height  图片高
********************************************************/
void CGame::SetRect( LONG left, LONG top,
                           LONG Width, LONG Height,BOOL data )
{
    LONG x,y,bottom,right,x1,y1,xCount,i,j;
	if((left+Width)<map.SceneW) 
		right = left+Width;
	else right=map.SceneW-1;
	if((top+Height)<map.SceneH) 
		bottom = top+Height;
	else bottom=map.SceneH-1;
	if(left<0) left=0;
	if(top<0) top=0;
    x = (LONG)( left/16 );
    y = (LONG)( top/16 );
	x1 = (LONG)( right/16 );
	if((x1*16)==(right+1))//整除
		x1++;
    y1 = (LONG)( bottom/16 );
	if((y1*16)==(bottom+1))//整除
		y1++;
	xCount=(LONG)(map.SceneW/16);
	for(i=x;i<=x1;i++)
		for(j=y;j<=y1;j++)
			map.Square[j*xCount+i]=data;
	/*ofstream result("result.txt",ios::app);
	ofstream Delete1("result.txt",ios::trunc);
    Delete1.close();
	LONG count=xCount*xCount;
	for(i=0;i<count;i++){
		result<<map.Square[i];
		if((i+1)%xCount==0)
			result<<endl;
	}
	result.close();*/
}
/*******************************************************
* 函数名 : CheckRect(...)
*   功能 : 检查矩形内的地形数组
*   参数 : left  地图位置
*          top  地图位置
*          Width 图片宽
*          Height  图片高
********************************************************/
BOOL CGame::CheckRect( LONG left, LONG top,
                           LONG Width, LONG Height,BOOL data )
{
    LONG x,y,bottom,right,x1,y1,xCount,i,j;
	BOOL flag=FALSE;
	if((left+Width)<map.SceneW) 
		right = left+Width;
	else right=map.SceneW-1;
	if((top+Height)<map.SceneH) 
		bottom = top+Height;
	else bottom=map.SceneH-1;
	if(left<0) left=0;
	if(top<0) top=0;
    x = (LONG)( left/16 );
    y = (LONG)( top/16 );
	x1 = (LONG)( right/16 );
	if((x1*16)==(right+1))//整除
		x1++;
    y1 = (LONG)( bottom/16 );
	if((y1*16)==(bottom+1))//整除
		y1++;
	xCount=(LONG)(map.SceneW/16);
	for(i=x;i<=x1;i++){
		for(j=y;j<=y1;j++){
			if(map.Square[j*xCount+i]!=data){//break只跳出一层循环
				flag =TRUE;
			}
		}
		if(flag) break;
	}
	if(j==(y1+1)){
		if(i==(x1+1)){
			return TRUE;
		}
	}
	return FALSE;
}
/*******************************************************
* 函数名 : returnSelect(...)
*   功能 : 检查矩形内的单位 返回选中
*   参数 : pos  地图位置
********************************************************/
SelectUnit CGame::returnSelect(POINT pos){
	LONG i,j;
	SelectUnit temp1;
	Unit *temp;
	for( i=0;i<2;i++){//player
			for(j=0;j<player[i].Building.size();j++){//建筑
				temp=&player[i].Building[j];
				LONG w = Building_Common[player[i].race][temp->kind].eachWide/2;
				LONG h = Building_Common[player[i].race][temp->kind].eachHeight/2;
				if(pos.x>(temp->Pos.x-w) &&
					pos.x<(temp->Pos.x+w) &&
					pos.y>(temp->Pos.y-h) &&
					pos.y<(temp->Pos.y+h))
				{
					temp1.player=i;
					temp1.Bui_Sol=0;
					temp1.number=j;
					temp1.uniform_identifier=temp->uniform_identifier;
					temp1.kind=temp->kind;
					return temp1;
				}
			}
			for(j=0;j<player[i].People.size();j++){//士兵
				temp=&player[i].People[j];
				LONG w = People_Common[player[i].race][temp->kind].eachWide/4;
				LONG h = People_Common[player[i].race][temp->kind].eachHeight/4;
				if(pos.x>(temp->Pos.x-w) &&
					pos.x<(temp->Pos.x+w) &&
					pos.y>(temp->Pos.y-h) &&
					pos.y<(temp->Pos.y+h))
				{
					temp1.player=i;
					temp1.Bui_Sol=1;
					temp1.number=j;
					temp1.uniform_identifier=temp->uniform_identifier;
					temp1.kind=temp->kind;
					return temp1;
				}
			}
		}
		for(j=0;j<Neutral_Unit.size();j++){//中立
				temp=&Neutral_Unit[j];
				LONG w = Neutral_Common[temp->kind].eachWide/2;
				LONG h = Neutral_Common[temp->kind].eachHeight/2;
				if(pos.x>(temp->Pos.x-w) &&
					pos.x<(temp->Pos.x+w) &&
					pos.y>(temp->Pos.y-h) &&
					pos.y<(temp->Pos.y+h))
				{
					temp1.player=2;//中立
					temp1.Bui_Sol=0;
					temp1.number=j;
					temp1.uniform_identifier=temp->uniform_identifier;
					temp1.kind=temp->kind;
					return temp1;
				}
		}
		temp1.uniform_identifier = -1;
		return temp1;
}
/*******************************************************
* 函数名 : AdjustUI(...)
*   功能 : 调整统一标志值
*   参数 : ui  选择单位
********************************************************/
SelectUnit CGame::AdjustUI(SelectUnit ui){
	LONG i;
	if(ui.player ==2){//中立
		vector<Unit>::iterator itr = Neutral_Unit.begin();
		i=0;
		while (itr != Neutral_Unit.end())
		{
			if(itr->uniform_identifier == ui.uniform_identifier){
				ui.number = i;
				return ui;
			}
			++itr;
			++i;
		}				
	}
	else if(ui.Bui_Sol == 0){ //建筑
		vector<Unit>::iterator itr = player[ui.player].Building.begin();
		i=0;
		while (itr != player[ui.player].Building.end())
		{
			if(itr->uniform_identifier == ui.uniform_identifier){
				ui.number = i;
				return ui;
			}
			++itr;
			++i;
		}
	}
	else if(ui.Bui_Sol == 1){ //士兵
		vector<Unit>::iterator itr = player[ui.player].People.begin();
		i=0;
		while (itr != player[ui.player].People.end())
		{
			if(itr->uniform_identifier == ui.uniform_identifier){
				ui.number = i;
				return ui;
			}
			++itr;
			++i;
		}
	}
	ui.uniform_identifier = -1;
	return ui;	
}
/*******************************************************
* 函数名 : CheckUI(...)
*   功能 : 检查选择单位
*   参数 : ui  选择单位
********************************************************/
BOOL CGame::CheckUI(SelectUnit ui){
	if(ui.player ==2 && ui.number<Neutral_Unit.size() && 
		Neutral_Unit[ui.number].uniform_identifier==ui.uniform_identifier){//中立
		return TRUE;						
	}
	else if(ui.Bui_Sol == 0){ //建筑
		if(ui.number < player[ui.player].Building.size() && 
			player[ui.player].Building[ui.number].uniform_identifier==ui.uniform_identifier)
			return TRUE;
	}
	else if(ui.Bui_Sol == 1){ //士兵
		if(ui.number < player[ui.player].People.size() &&
			player[ui.player].People[ui.number].uniform_identifier==ui.uniform_identifier)
			return TRUE;
	}
	return FALSE;
}
/*******************************************************
* 函数名 : returnUnit(...)
*   功能 : 返回选择单位
*   参数 : ui  选择单位
********************************************************/
Unit *CGame::returnUnit(SelectUnit ui){
	Unit *temp=NULL;
	if(!CheckUI(ui))//统一标志不对
	{
		ui=AdjustUI(ui);//调整
	}
	if(ui.uniform_identifier == -1) //调整后发现该单位已不存在
	{
		return temp;
	}
	if(ui.player ==2 && ui.number<Neutral_Unit.size() && 
		Neutral_Unit[ui.number].uniform_identifier==ui.uniform_identifier){//中立
		temp = &Neutral_Unit[ui.number];
		return temp;						
	}
	else if(ui.Bui_Sol == 0){ //建筑
		if(ui.number < player[ui.player].Building.size() &&
			player[ui.player].Building[ui.number].uniform_identifier==ui.uniform_identifier)
			temp = &player[ui.player].Building[ui.number];
			return temp;
	}
	else if(ui.Bui_Sol == 1){ //士兵
		if(ui.number < player[ui.player].People.size() &&
			player[ui.player].People[ui.number].uniform_identifier==ui.uniform_identifier)
			temp = &player[ui.player].People[ui.number];
			return temp;
	}
	return temp;	
}
/*******************************************************
* 函数名 : returnUnitCommon(...)
*   功能 : 返回选择单位的公共部分
*   参数 : ui  选择单位
********************************************************/
Unit_Common *CGame::returnUnitCommon(SelectUnit ui){
	//Unit temp = returnUnit(ui);
	Unit_Common *temp1=NULL;
	if(ui.player ==2 ){//中立
		if (ui.kind>=0 && ui.kind<1)
		{
			temp1 = &Neutral_Common[ui.kind];
			return temp1;
		}
								
	}
	else if(ui.Bui_Sol == 0){ //建筑
		if (ui.kind>=0 && ui.kind<5)
		{
			temp1 = &Building_Common[player[ui.player].race][ui.kind];
			return temp1;
		}
	}
	else if(ui.Bui_Sol == 1){ //士兵
		if (ui.kind>=0 && ui.kind<5)
		{
			temp1 = &People_Common[player[ui.player].race][ui.kind];
			return temp1;
		}
	}
	return temp1;
}
/*******************************************************
* 函数名 : RectangleFrame(...)
*   功能 : 画矩形框
********************************************************/
void CGame::RectangleFrame(POINT a,POINT b){
	if(a.x==b.x && a.y == b.y)
		return;
	LONG dis;
	if(a.x>b.x){
		dis=a.x;
		a.x=b.x;
		b.x=dis;
	}
	if(a.y>b.y){
		dis=a.y;
		a.y=b.y;
		b.y=dis;
	}
	RECT rect_row = GetRect(0,0,500,3);
	RECT rect_col = GetRect(0,0,3,500);
	LONG x=a.x-ScrPos.x;
    LONG y=a.y-ScrPos.y;
	LONG w = b.x-a.x;
	LONG h = b.y-a.y;
	rect_row.right=w;
	DDraw.BlitBack(x,y,lpDDSSelect_Row,&rect_row,0 );
	y=y+h;
	DDraw.BlitBack(x,y,lpDDSSelect_Row,&rect_row,0 );
	y=y-h;
	rect_col.bottom = h+3;
	DDraw.BlitBack(x,y,lpDDSSelect_Col,&rect_col,0 );
	x=x+w;
	DDraw.BlitBack(x,y,lpDDSSelect_Col,&rect_col,0 );
}
/*******************************************************
* 函数名 : setDamage(...)
*   功能 : 设置选择单位的伤害
*   参数 : ui  选择单位
*	返回值：TRUE 伤害单位死亡 FALSE 伤害单位还存活
********************************************************/
BOOL CGame::setDamage(SelectUnit ui,LONG damage,LONG _attack_kind){
	Unit *temp = returnUnit(ui);
	if(temp->bDead == TRUE|| temp == NULL)//对方已不存在
			return TRUE;
	Unit_Common *temp2 = returnUnitCommon(ui);
	if(temp2==NULL)
		return TRUE;
	Unit *temp3;
	if(ui.player ==2 ){//中立
		temp3= &Neutral_Unit[ui.number];
		damage = damage*(10-temp2->defense)*0.1;
		temp3->HP -= damage;
		if(temp3->HP<=0){
			temp3->HP=0;
			temp3->bDead = TRUE;
			temp3->state = UnitActionDie;
			//将所处的方块恢复为非障碍
			LONG w=temp2->eachWide;
			LONG h=temp2->eachHeight;
			SetRect(temp3->Pos.x-w/2,temp3->Pos.y-h/2,w,h,FALSE);
			return TRUE;
		}
	}
	else if(ui.Bui_Sol == 0){ //建筑
		temp3= &player[ui.player].Building[ui.number];
		damage = returnDamage(damage,temp2->defense,_attack_kind,temp2->defense_kind);
		temp3->HP -= damage;
		if(temp3->HP<=0){
			temp3->HP=0;
			temp3->bDead = TRUE;
			temp3->state = UnitActionDie;
			if(ui.player==0)
			{
				Count.playerBuiLost[temp3->kind]++;
			}
			if(ui.player==1)
			{
				Count.AIBuiLost[temp3->kind]++;
			}
			//将所处的方块恢复为非障碍
			LONG w=temp2->eachWide;
			LONG h=temp2->eachHeight;
			SetRect(temp3->Pos.x-w/2,temp3->Pos.y-h/2,w,h,FALSE);
			return TRUE;
		}		
	}
	else if(ui.Bui_Sol == 1){ //士兵
		temp3= &player[ui.player].People[ui.number];
		damage = returnDamage(damage,temp2->defense,_attack_kind,temp2->defense_kind);
		temp3->HP -= damage;
		if(temp3->HP<=0){
			temp3->HP=0;
			player[ui.player].population -= temp2->population_cost;
			map.Square[GPa.GetSquareFromPos(temp3->Pos)] = 0;//将所处的方块恢复为非障碍
			temp3->bDead = TRUE;
			temp3->state = UnitActionDie;
			if(ui.player==0)
			{
				Count.playerSolLost[temp3->kind]++;
			}
			if(ui.player==1)
			{
				Count.AISolLost[temp3->kind]++;
			}
			temp3->lStartIndex = temp2->start_die;
			temp3->lEndIndex = temp2->end_die;
			temp3->pic_Index = temp->Direct+temp->lStartIndex;
			temp3->startTime = Time;
			temp3->Target_Select = ui;
			DeadQueue.push_back(*temp3);
			return TRUE;
		}

	}
	return  FALSE;
}
/*******************************************************
* 函数名 : returnDamage(...)
*   功能 : 设置选择单位的伤害
*   参数 : ui  选择单位

0:轻甲 1:中甲 2:重甲 3:加强型城甲
0:普通 1:穿刺 2:魔法 3:混乱

-----中型 重型 轻型 建筑  
普通 150% 100% 100% 70%   
穿刺 75%  100% 200% 35%   
魔法 75%  200% 100% 35%   
混乱 100% 100% 100% 100%  
 
所受伤害之减少 =((装甲)*0.06)/)(1+0.06*(装甲)
********************************************************/
LONG CGame::returnDamage(LONG damage,LONG defense,LONG src_attack_kind,LONG des_defense_kind){
	switch (src_attack_kind)
	{
	case 0://普通攻击
		switch (des_defense_kind)
		{
		case 1: damage = (FLOAT)damage * 1.5;
			break;
		case 3: damage = (FLOAT)damage * 0.5;
			break;
		}
		break;
	case 1://穿刺攻击
			switch (des_defense_kind)
			{
			case 0: damage = (FLOAT)damage * 2;
				break;
			case 1: damage = (FLOAT)damage * 0.75;
				break;
			case 3: damage = (FLOAT)damage * 0.35;
				break;
			}
		break;
	case 2://魔法攻击
				switch (des_defense_kind)
				{
				case 1: damage = (FLOAT)damage * 1;
					break;
				case 2: damage = (FLOAT)damage * 2;
					break;
				case 3: damage = (FLOAT)damage * 0.35;
					break;
				}
		break;
	case 3://混乱攻击
		switch (des_defense_kind)
		{
		case 1: damage = (FLOAT)damage * 1;
			break;
		}
		break;
	}
	FLOAT rate = 1 - (defense*0.06)/(1+defense*0.06);
	damage *= rate;
	return damage;
}
/*******************************************************
* 函数名 : selectInFrame(...)
*   功能 : 选择在选择框中的单位
*   参数 : a 起点 b 终点
********************************************************/
void CGame::selectInFrame(POINT a,POINT b){
	SelectUnit temp;
	if(a.x==b.x && a.y == b.y)
		return;
	LONG dis;
	if(a.x>b.x){
		dis=a.x;
		a.x=b.x;
		b.x=dis;
	}
	if(a.y>b.y){
		dis=a.y;
		a.y=b.y;
		b.y=dis;
	}
	select_frame.clear();
	//士兵
	vector<Unit>::iterator itr = player[0].People.begin();
	dis=0;
	while (itr != player[0].People.end())
	{
		if(itr->bDead == FALSE){
			if(itr->Pos.x >= a.x && itr->Pos.x <= b.x && itr->Pos.y >= a.y && itr->Pos.y <= b.y){
				temp.player = 0;
				temp.Bui_Sol = 1;
				temp.number = dis;
				temp.uniform_identifier = itr->uniform_identifier;
				temp.kind = itr->kind;
				select_frame.push_back(temp);
			}
		}
		dis++;
		itr++;
		if(select_frame.size()>=8)//最多一队选8个人
			break;
	}
	if(select_frame.size()){
		swap(select,select_frame);
		playsound(temp,0);
	}
}
/*********************************************************
*   函数名: OneStep(...) 
*     功能: 在指定方向上移动一步
* 参数说明: pos：起点, direct ：方向
**********************************************************/
POINT CGame::OneStep( POINT pos,LONG direct ){
	pos = GPa.AdjustPos(pos);
	switch(direct){
	case Direction_Down:{	//下
		pos.y +=16;
		break;
		   }
	case Direction_LeftDown:{	//左下
		pos.x -=16;
		pos.y +=16;
		break;
		   }
	case Direction_Left:{	//左
		pos.x -=16;
		break;
		   }
	case Direction_LeftUp:{	//左上
		pos.x -=16;
		pos.y -=16;
		break;
		   }
	case Direction_Up:{	//上
		pos.y -=16;
		break;
		   }
	case Direction_RightUp:{	//右上
		pos.x +=16;
		pos.y -=16;
		break;
		   }
	case Direction_Right:{	//右
		pos.x +=16;
		break;
		   }
	case Direction_RightDown:{	//右下
		pos.x +=16;
		pos.y +=16;
		break;
		}
	}
	return pos;
}
/*********************************************************
*   函数名: DrawIcon(...) 
*     功能: 在指定位置画图标
* 参数说明: pos：位置, i ：图标源表面的计数
**********************************************************/
void CGame::DrawIcon(POINT pos,LONG i){
	RECT rect = GetRscRect(i,46,38,5);
	DDraw.BlitBack(pos.x,pos.y,lpDDSIcons,&rect,0 );
}
/*********************************************************
*   函数名: CheckIcon(...) 
*     功能: 检测交互  是否有图标被击中
**********************************************************/
LONG CGame::CheckIcon(POINT a){
	if(select.size()>1){
		for(LONG i=0;i<select.size();i++){
			if(inRect(SelectRECT[i],a))
				return i;
		}
	}
	if(inRect(SelectRECT[8],a))
		return 8;
	return -1;
}
/*********************************************************
*   函数名: DrawBloodBar(...) 
*     功能: 在指定位置画血条
* 参数说明: startx 起点x starty 起点y length 长 height 高 rate 血量比
**********************************************************/
void CGame::DrawBloodBar(LONG startx,LONG starty,LONG length,LONG height,FLOAT rate){	
	LONG x=length*rate;
	RECT rect = GetRect(0,0,x,height);
	DDraw.BlitBack(startx,starty,lpDDSColor[0],&rect,0 );
	rect.right = length - x;
	DDraw.BlitBack(startx+x,starty,lpDDSColor[1],&rect,0 );
}
/*********************************************************
*   函数名: CalArmyBuildingCenterPos(...) 
*     功能: 计算玩家建筑的中心位置
* 参数说明: i player[i]
**********************************************************/
POINT CGame::CalArmyBuildingCenterPos(LONG i){
	POINT temp;
	LONG dis;
	if(player[i].Building.size() <=0){
		temp.x = -1;
		return temp;
	}
	else {
		temp = player[i].Building[0].Pos;
		vector<Unit>::iterator itr = player[i].Building.begin();
		while (itr != player[0].Building.end())//错误player[i]
		{
			dis = GPa.Distance(temp,itr->Pos);
			if(dis>400)//两个建筑距离过长
			{
				itr++;
				continue;
			}
			temp.x = (temp.x+itr->Pos.x)/2;
			temp.y = (temp.y+itr->Pos.y)/2;
			itr++;
		}
	}
	return temp;
}
/*********************************************************
*   函数名: setStrategy(...) 
*     功能: 计算玩家建筑的中心位置
* 参数说明: i player[i]选择j策略
**********************************************************/
void CGame::setStrategy(LONG i,LONG j){
	for(LONG m=0;m<5;m++){
		player[i].strategy[m]=Strategy[j][m];
	}
	player[i].strategy[5]=j;
}
/*********************************************************
*   函数名: AdjustSelect(...) 
*     功能: 根据统一标准符调整select中的内容
* 参数说明: 
**********************************************************/
void CGame::AdjustSelect(){
	vector<SelectUnit>::iterator itr0 = select.begin();
	for( itr0 = select.begin(); itr0!=select.end(); ++itr0 )
	{		
		Unit *temp = returnUnit(*itr0);
		if( temp->bDead == TRUE || temp == NULL)
		{
			select.erase(itr0);
			itr0--;
		}
	}

	itr0 = select_1.begin();
	for( itr0 = select_1.begin(); itr0!=select_1.end(); ++itr0 )
	{		
		Unit *temp = returnUnit(*itr0);
		if( temp->bDead == TRUE || temp == NULL)
		{
			select_1.erase(itr0);
			itr0--;
		}
	}

	itr0 = select_1.begin();
	for( itr0 = select_2.begin(); itr0!=select_2.end(); ++itr0 )
	{		
		Unit *temp = returnUnit(*itr0);
		if( temp->bDead == TRUE || temp == NULL)
		{
			select_2.erase(itr0);
			itr0--;
		}
	}

	itr0 = select_1.begin();
	for( itr0 = select_3.begin(); itr0!=select_3.end(); ++itr0 )
	{		
		Unit *temp = returnUnit(*itr0);
		if( temp->bDead == TRUE || temp == NULL)
		{
			select_3.erase(itr0);
			itr0--;
		}
	}
}
/*********************************************************
*   函数名: AdjustSelect(...) 
*     功能: 删除后根据统一标准符调整select中的内容
* 参数说明: 
**********************************************************/
void CGame::AdjustAfterDelete(){
	vector<SelectUnit>::iterator itr0 = select.begin();
	for( itr0 = select.begin(); itr0!=select.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//统一标志不对
			*itr0=AdjustUI(*itr0);//调整
		/*if(ui.uniform_identifier == -1) //调整后发现该单位已不存在
		{
			return temp;
		}*/
	}

	itr0 = select_1.begin();
	for( itr0 = select_1.begin(); itr0!=select_1.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//统一标志不对
			*itr0=AdjustUI(*itr0);//调整
	}

	itr0 = select_2.begin();
	for( itr0 = select_2.begin(); itr0!=select_2.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//统一标志不对
			*itr0=AdjustUI(*itr0);//调整
	}

	itr0 = select_3.begin();
	for( itr0 = select_3.begin(); itr0!=select_3.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//统一标志不对
			*itr0=AdjustUI(*itr0);//调整
	}
}
