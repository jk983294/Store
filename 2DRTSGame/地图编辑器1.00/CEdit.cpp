/*-------------------------------------------
|              CEdit.cpp
|             编辑类代码
|             (c)  jk&ljq
---------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "CEdit.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define random(x) (rand()%x)
#define randomize() srand(GetTickCount())

inline RECT GetRscRect( LONG lIndex, LONG lWidth,
                           LONG lHeight, LONG lCount );

/************************************************************
*   函数名: CEdit(...)
*     功能: 构造函数
*************************************************************/
CEdit::CEdit( void )
{
    hWnd        = NULL;
    bFullScr    = FALSE;//默认为窗口模式
    ScreenW     = 640;//
    ScreenH     = 480;//默认分辨率640*480
    ColorBits   = 16;//默认颜色深度为16色
    MaxWObject  = 340;//对象矩形图片的最大宽度
    MaxHObject  = 300;//对象矩形图片的最大高度
    ScrPos.x    = 0;
    ScrPos.y    = 0;
    ZeroMemory( &map, sizeof(MAP) );
    lAppState   = 0;
    lUpdateScreen = 1;
    lSaveState  = 0;
    lEditState  = 0;//地面
    GRNKind     = 0;
    oldGRNKind  = GRNKind;

	ZeroMemory( sceFilePath, sizeof(sceFilePath) );
	ZeroMemory( GroundRC, sizeof(LONG)*100 );
	ZeroMemory( &map, sizeof(MAP) );
	delete []map.Grn;
	ZeroMemory( &player[0], sizeof(Player) );
	ZeroMemory( &player[1], sizeof(Player) );
	for(LONG i=0;i<2;i++){
		player[i].Building.clear();
		player[i].People.clear();
	}
	player[0].race = 0;//默认人族
	player[1].race = 1;//默认兽族
	IorAI=0;//默认自己
	kind_building =0;
	kind_people =0;
	UI=0;
    KeyKB           = NULL;
    KeyMouse        = NULL; 
    randomize();
	player[0].gold =1000;
	player[1].gold =1000;
	player[0].population =0;
	player[1].population =0;
	i=0;
	ifstream inf;							//读取地图表面方格定义
	inf.open("data\\Configure\\Map.Conf");
	LONG data;
	while (inf>>data)
	{
		GroundRC[i++]=data;
	}
	inf.close();
}

/************************************************************
*   函数名: ~CEdit(...)
*     功能: 析构函数
*       (c) jk&ljq, 2010
*************************************************************/
CEdit::~CEdit( void )
{
    DeleteSceneData(map);
}

/********************************************************
*  函数名:  GetRect(...)
*  功能:    获取一个矩形结构
*       (c) jk&ljq, 2010
*********************************************************/
RECT CEdit::GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}


/************************************************************
*   函数名: LoadImageSurf(...)
*     功能: 载入图像给表面
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::LoadImageSurf( void )
{
	//载入地面
    DDraw.CreateBMPSurface(lpDDSGroundRC,0,0,"data\\graphics\\terrain\\summer.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Row,0,0,"data\\graphics\\Select_Row.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Col,0,0,"data\\graphics\\Select_Col.bmp",DDSCAPS_SYSTEMMEMORY);
	//载入图标
	
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

	/*inf.open("data\\Configure\\Interface.Conf");//读取界面定义
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
	inf.close();*/

	/*inf.open("data\\Configure\\Missile.Conf");//读取导弹定义
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
	inf.close();*/
}
/************************************************************
*   函数名: InitDirectX(...)
*     功能: 初始化DirectX以及载入声音图像数据
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::InitDirectX( HWND _hWnd )
{
    hWnd = _hWnd;
    //>>>>>> 初始化CDInput >>>>>>>>>>>
    Input.Set_bFullScr( FALSE );
    Input.InitDInput( hWnd );
    KeyKB = Input.Get_KeyKB();
    KeyMouse = Input.Get_KeyMouse();

    //>>>>>> 初始化CDDraw >>>>>>>>>>>
    DDraw.InitDDraw(
        hWnd, 
        bFullScr,
        ScreenW, ScreenH,
        ColorBits );
    LoadImageSurf();//为表面载入图像数据

    //>>>>>> 初始化CText >>>>>>>>>>>
    Text.Set_lpDDSBack(DDraw.Get_lpDDSBack());
}
/***********************************************
*   函数名: NewScene(...)
*     功能: 根据指定的场景大小新建一个场景
************************************************/
void CEdit::NewScene( void )
{
    //>>>>>> 生成场景数据 >>>>>>>
    //地面方块的大小为32*32
    LONG hN = (LONG)(map.SceneH/32);
    LONG wN = (LONG)(map.SceneW/32);
    map.lGrnN = hN * wN;

    //为地面方块开辟空间
	map.Grn =new LONG[map.lGrnN];
	ZeroMemory(map.Grn,sizeof(LONG)*(hN*wN));
	LONG tmp;
    for( LONG n=0;n<map.lGrnN;n++ )
    {
		tmp=random(3);
		if(GRNKind==1)       //草地
			tmp = tmp+3;		
		map.Grn[n]=tmp;
    }
	if(GRNKind==1)       //草地
		GRNKind=tmp;
    map.lObjectN = 0;
    //地形方块的大小为16*16
    hN = (LONG)(map.SceneH/16);
    wN = (LONG)(map.SceneW/16);
    map.lSquareCount = hN * wN;
    map.Square = new WORD[hN * wN];
    ZeroMemory(map.Square,sizeof(WORD)*(hN*wN));

	//>>>>>> 初始化CGetPath >>>>>>>>>
	//设置地形方块大小为16*16
	GPa.Set_lSquareC(16);
	//设置场景大小
	GPa.Set_SceneW(map.SceneW);
	GPa.Set_SceneH(map.SceneH);
	//把地形数组传送给CGetPath对象
	GPa.Set_Square((VOID*)map.Square);

    lSaveState = 0;//当前场景数据还未存为文件
    //进入主进程
    lAppState = 1;
    lUpdateScreen = 1;
}

BOOL CEdit::GetSceneFileName( DWORD dwFlag )
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
/************************************************************
*   函数名: SaveGroundDesc(...)
*     功能: 保存指定场景的地面描述
*************************************************************/
void CEdit::SaveGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	fwrite( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   函数名: LoadGroundDesc(...)
*     功能: 载入指定场景的地面描述
*************************************************************/
void CEdit::LoadGroundDesc( FILE *file )
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
void CEdit::SaveObject( FILE *file )
{
    if( file==NULL )
        return;
	for(LONG i=0;i<2;i++){//player
		player[i].BuildingCount=player[i].Building.size();
		player[i].PeopleCount=player[i].People.size();
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
void CEdit::LoadObject( FILE *file )
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
void CEdit::SaveTerrainSquare( FILE *file )
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
void CEdit::LoadTerrainSquare( FILE *file )
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
void CEdit::SaveSceneData( BOOL bSaveAs )
{
    //是保存为或者是第一次保存则选择文件名
    if( bSaveAs || lSaveState==0 )
    {
        if( !GetSceneFileName( 1 ) )
            return;
        if( map.SceneH==0 || map.SceneW==0 )
        {
            MessageBox( hWnd,
                TEXT("还没有建立场景数据\n")
                TEXT("将不保存任何数据"),
                TEXT("Warning!"),MB_OK );
            return;
        }
    }
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
/************************************************
*   函数名: LoadSceneData(...)
*     功能: 载入指定场景的数据
*       (c) jk&ljq, 2010
*************************************************/
void CEdit::LoadSceneData( void )
{
    //获得打开文件的名称
    if( !GetSceneFileName( 0 ) ){
        return;
	}
    //释放旧场景的数据
    DeleteSceneData( map );
    //打开指定场景文件
    FILE *file = fopen(sceFilePath,"rb");
    //载入场景结构数据
    fread(&map,sizeof(MAP),1,file);
	fread(&UI,sizeof(LONG),1,file);
    //载入地面方块描述数据
    LoadGroundDesc( file );
    //载入对象数据
    LoadObject( file );
    //载入地形方块数据
    LoadTerrainSquare( file );

    //结束载入工作
    fclose(file);
    lSaveState = 1;//当前场景数据已经有对应的文件
    //进入主进程
    lAppState = 1;
    lUpdateScreen = 1;//更新后台缓冲及屏幕
}


/**************************************************
*   函数名: RestoreVariable(...)
*     功能: 恢复需要恢复的变量
*       (c) jk&ljq, 2010
***************************************************/
inline void CEdit::RestoreVariable( void )
{
    //恢复对象显示链表数组
}

/**************************************************
*   函数名: CheckScrPos(...)
*     功能: 检测屏幕位置
***************************************************/
inline LONG CEdit::CheckScrPos( void )
{
    //保存屏幕位置
    POINT oldScrPos = ScrPos;

    //鼠标在客户区边界上则移动屏幕
    if( KeyMouse[0].Pos.x<=0 )
        ScrPos.x -= 20;
    else if( KeyMouse[0].Pos.x>=ScreenW-8 )
        ScrPos.x += 20;
    if( KeyMouse[0].Pos.y<=-15 )
        ScrPos.y -= 20;
    else if( KeyMouse[0].Pos.y>=ScreenH-6 )
        ScrPos.y += 20;
    
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

/**************************************************
*   函数名: GroundEdit(...)
*     功能: 地面编辑
*       (c) jk&ljq, 2010
***************************************************/
LONG CEdit::GroundEdit( void )
{
    //还没有建立场景
    if( map.SceneH==0 || map.SceneH==0 )
        return 0;
    //选择的操作不是地面编辑
    if( (lEditState!=0)&&(lEditState!=1) )
        return 0;
    //没有按下并弹起鼠标左键
    if( !(KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0) )
        return 0;
    //修改了上次保存的数据
    if( lSaveState!=0 )
        lSaveState = 2;

    //>>>>>>>> 求鼠标单击的是哪个单元 >>>>>>>>>>
	LONG x = (LONG)( (MouseScePos.x)/32 );
	LONG y = (LONG)( (MouseScePos.y)/32 );
    LONG wN = map.SceneW/32;
    // 获得选择中方块的场景序号
    LONG grn_Index = y*wN+x;
	map.Grn[grn_Index]=GRNKind;
	if(lEditState==1)//跟新地形数组
	{
		wN *= 2;
		x  *= 2;
		y  *= 2;
		map.Square[y*wN+x]=1;
		map.Square[y*wN+x+1]=1;
		y++;
		map.Square[y*wN+x]=1;
		map.Square[y*wN+x+1]=1;
	}
    return 1;
}
/**************************************************
*   函数名: DeleteObject(...)
*     功能: 删除指定的对象
*       (c) jk&ljq, 2010
***************************************************/
void CEdit::DeleteObject( LONG uniform_temp )
{ 
	vector<Unit>::iterator itr;
	for(LONG i=0;i<2;i++){//player
		 itr= player[i].Building.begin();
		while (itr != player[i].Building.end())
		{
			if (itr->uniform_identifier == uniform_temp){
				//>>>>> 解除对应的障碍 >>>>>>>
				LONG w=Building_Common[player[i].race][itr->kind].eachWide;
				LONG h=Building_Common[player[i].race][itr->kind].eachHeight;
				SetRect(itr->Pos.x-w/2,itr->Pos.y-h/2,w,h,FALSE);
				player[i].Building.erase(itr);
				player[i].BuildingCount=player[i].Building.size();
				return ;
			}
			++itr;
		}
		itr = player[i].People.begin();
		while (itr != player[i].People.end())
		{
			if (itr->uniform_identifier == uniform_temp){
				//>>>>> 解除对应的障碍 >>>>>>>
				LONG w=People_Common[player[i].race][itr->kind].eachWide;
				LONG h=People_Common[player[i].race][itr->kind].eachHeight;
				SetRect(itr->Pos.x-w/2,itr->Pos.y-h/2,w,h,FALSE);
				player[i].People.erase(itr);
				player[i].PeopleCount=player[i].People.size();
				return ;
			}
			++itr;
		}		
	}
	itr = Neutral_Unit.begin();
	while (itr != Neutral_Unit.end())
	{
		if (itr->uniform_identifier == uniform_temp){
			//>>>>> 解除对应的障碍 >>>>>>>
			LONG w=Neutral_Common[itr->kind].eachWide;
			LONG h=Neutral_Common[itr->kind].eachHeight;
			SetRect(itr->Pos.x-w/2,itr->Pos.y-h/2,w,h,FALSE);
			Neutral_Unit.erase(itr);
			return ;
		}
		++itr;
	}
}
/**************************************************
*   函数名: DeleteSelect(...)
*     功能: 删除指定的对象
*       (c) jk&ljq, 2010
***************************************************/
void CEdit::DeleteSelect(LONG uniform_temp )
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
/**************************************************
*   函数名: NewObject(...)
*     功能: 新加对象
*       (c) jk&ljq, 2010
***************************************************/
void CEdit::NewObject( void )
{
	LONG w,h;
    Unit temp;
	Unit_Common temp1;
	ZeroMemory( &temp, sizeof(Unit) );
	ZeroMemory( &temp1, sizeof(Unit_Common) );
	temp.state =UnitActionNone;
	temp.Direct =0;
	temp.bDead=FALSE;
	temp.pic_Index=0;
	if(lEditState==2){
		LONG temp_race=player[IorAI].race;
		temp.kind = kind_building;
		temp1 = Building_Common[temp_race][kind_building];
		w = temp1.eachWide;
		h = temp1.eachHeight;
		temp.HP =temp1.Hp;				 
	}
	else if(lEditState==3){
		LONG temp_race=player[IorAI].race;
		temp.kind = kind_people;
		temp1 = People_Common[temp_race][kind_people];
		w = temp1.eachWide/2;
		h = temp1.eachHeight/2;	
		temp.HP =temp1.Hp;
	}
	else if(lEditState==4){
		temp1=Neutral_Common[kind_neutral];
		w = temp1.eachWide;
		h = temp1.eachHeight;
		temp.kind = kind_neutral;
	}
	temp.Pos.x = MouseScePos.x;
    temp.Pos.y = MouseScePos.y;

	BOOL flag= CheckRect(temp.Pos.x-w/2,temp.Pos.y-h/2,w,h,FALSE);
	if(lEditState==3){
		LONG aa = map.Square[GPa.GetSquareFromPos(MouseScePos)];
		if(aa==0) flag=TRUE;
		else flag = FALSE;
	}
	if(flag){
		temp.uniform_identifier=UI;
		UI++;
		if(lEditState==2){
			player[IorAI].Building.push_back(temp);
			player[IorAI].BuildingCount=player[IorAI].Building.size();
			//>>>>> 生成对应的障碍 >>>>>>>
			SetRect(temp.Pos.x-w/2,temp.Pos.y-h/2,w,h,TRUE);
		}
		else if(lEditState==3){
			player[IorAI].People.push_back(temp);
			player[IorAI].PeopleCount=player[IorAI].People.size();
			player[IorAI].population +=temp1.population_cost;
			//>>>>> 生成对应的障碍 >>>>>>>
			map.Square[GPa.GetSquareFromPos(MouseScePos)] = 1;
		}
		else if(lEditState==4){
			Neutral_Unit.push_back(temp);
			//>>>>> 生成对应的障碍 >>>>>>>
			SetRect(temp.Pos.x-w/2,temp.Pos.y-h/2,w,h,TRUE);
		}
		
	}	
}
/**************************************************
*   函数名: ObjectEdit(...)
*     功能: 编辑对象
*       (c) jk&ljq, 2010
***************************************************/
LONG CEdit::ObjectEdit( void )
{
    //还没有建立场景
    if( map.SceneH==0 || map.SceneH==0 )
        return 0;
    //选择的操作不是地面编辑
    if( lEditState!=2 && lEditState!=3 && lEditState!=4)
        return 0;

    //>>>>>>> 编辑对象 >>>>>>>>>>
    //右键选择对象
    if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON1 )
    {
		select.clear();
		SelectUnit temp1;
		Unit temp;
		LONG i,j;
		for(i=0;i<2;i++){//player
			for(j=0;j<player[i].Building.size();j++){//建筑
				temp=player[i].Building[j];
				LONG w = Building_Common[i][temp.kind].eachWide/2;
				LONG h = Building_Common[i][temp.kind].eachHeight/2;
				if(MouseScePos.x>(temp.Pos.x-w) &&
					MouseScePos.x<(temp.Pos.x+w) &&
					MouseScePos.y>(temp.Pos.y-h) &&
					MouseScePos.y<(temp.Pos.y+h))
				{
					temp1.player=i;
					temp1.Bui_Sol=0;
					temp1.number=j;
					temp1.uniform_identifier=temp.uniform_identifier;
					select.push_back(temp1);
					break;
				}
			}
			for(j=0;j<player[i].People.size();j++){//士兵
				temp=player[i].People[j];
				LONG w = People_Common[i][temp.kind].eachWide/4;
				LONG h = People_Common[i][temp.kind].eachHeight/4;
				if(MouseScePos.x>(temp.Pos.x-w) &&
					MouseScePos.x<(temp.Pos.x+w) &&
					MouseScePos.y>(temp.Pos.y-h) &&
					MouseScePos.y<(temp.Pos.y+h))
				{
					temp1.player=i;
					temp1.Bui_Sol=1;
					temp1.number=j;
					temp1.uniform_identifier=temp.uniform_identifier;
					select.push_back(temp1);
					break;
				}
			}
		}
		for(j=0;j<Neutral_Unit.size();j++){//中立
				temp=Neutral_Unit[j];
				LONG w = Neutral_Common[temp.kind].eachWide/2;
				LONG h = Neutral_Common[temp.kind].eachHeight/2;
				if(MouseScePos.x>(temp.Pos.x-w) &&
					MouseScePos.x<(temp.Pos.x+w) &&
					MouseScePos.y>(temp.Pos.y-h) &&
					MouseScePos.y<(temp.Pos.y+h))
				{
					temp1.player=2;//中立
					temp1.Bui_Sol=0;
					temp1.number=j;
					temp1.uniform_identifier=temp.uniform_identifier;
					select.push_back(temp1);
					break;
				}
			}
		return 1;
    }
    //左键生成对象
    else if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 )
    {
        NewObject();
        select.clear();
        //修改了上次保存的数据
        if( lSaveState!=0 )
            lSaveState = 2;
        return 1;
    }
    //"Delete"键删除所选对象
    else if( KeyKB[0].Key == DIK_DELETE )
    {
		if(select.size()>0){
			LONG uniform_temp=select[0].uniform_identifier;
			DeleteSelect(uniform_temp);
			DeleteObject(uniform_temp);
		return 1;
		}
    }
    return 0;
}
/**********************************************
*   函数名: DeleteSceneData(...)
*     功能: 删除指定场景
*       (c) jk&ljq, 2010
***********************************************/
void CEdit::DeleteSceneData( MAP map )
{
	ZeroMemory( &map, sizeof(MAP) );
	delete []map.Grn;
	ZeroMemory( &player[0], sizeof(Player) );
	ZeroMemory( &player[1], sizeof(Player) );
	for(LONG i=0;i<2;i++){
		player[i].Building.clear();
		player[i].People.clear();
	}

    //释放地形数组空间
    delete[] map.Square;
}
/************************************************************
*   函数名: MakeGround(...)
*     功能: 显示地面
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::MakeGround( void )
{
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
		rect = GetRscRect(GroundRC[map.Grn[n]],32,32,16);
		DDraw.BlitBack( x,y,lpDDSGroundRC,&rect,0);
    }
}
/************************************************
*   函数名: MakeEntity(...)
*     功能: 地面上的和文字提示的显示
*       (c) jk&ljq, 2010
*************************************************/
void CEdit::MakeEntity( void )
{
    RECT rect;
    LONG x,y,i,j,w,h;
	Unit temp;
	Unit_Common temp1;
	RECT rect_row=GetRect(0,0,500,3);
	RECT rect_col=GetRect(0,0,3,500);

	for(i=0;i<2;i++){//player
		for(j=0;j<player[i].Building.size();j++){//建筑
			temp=player[i].Building[j];
			temp1=Building_Common[i][temp.kind];
			if(temp.Pos.x >ScrPos.x-MaxWObject &&
				temp.Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				temp.Pos.y >ScrPos.y-MaxHObject &&
				temp.Pos.y <ScrPos.y+MaxHObject+ScreenH)
			{
				rect=GetRscRect(temp.pic_Index,temp1.eachWide,temp1.eachHeight,temp1.lSrcWCount);
				x=temp.Pos.x-Building_Common[i][temp.kind].eachWide/2-ScrPos.x;
                y=temp.Pos.y-Building_Common[i][temp.kind].eachHeight/2-ScrPos.y;
				DDraw.BlitBack(x,y,Building_Common[player[i].race][temp.kind].lpSurf,
                    &rect,DDBLTFAST_SRCCOLORKEY );
			}
		}

		for(j=0;j<player[i].People.size();j++){//士兵
			temp=player[i].People[j];
			temp1=People_Common[i][temp.kind];
			if(temp.Pos.x >ScrPos.x-MaxWObject &&
				temp.Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				temp.Pos.y >ScrPos.y-MaxHObject &&
				temp.Pos.y <ScrPos.y+MaxHObject+ScreenH)
			{
				rect=GetRscRect(temp.pic_Index,temp1.eachWide,temp1.eachHeight,temp1.lSrcWCount);
				x=temp.Pos.x-People_Common[i][temp.kind].eachWide/2-ScrPos.x;
                y=temp.Pos.y-People_Common[i][temp.kind].eachHeight/2-ScrPos.y;
				DDraw.BlitBack(x,y,People_Common[player[i].race][temp.kind].lpSurf,
                    &rect,DDBLTFAST_SRCCOLORKEY );
			}
		}
	}
	for(j=0;j<Neutral_Unit.size();j++){//中立
				temp=Neutral_Unit[j];
				temp1=Neutral_Common[temp.kind];
				if(temp.Pos.x >ScrPos.x-MaxWObject &&
				temp.Pos.x <ScrPos.x+MaxWObject+ScreenW&&
				temp.Pos.y >ScrPos.y-MaxHObject &&
				temp.Pos.y <ScrPos.y+MaxHObject+ScreenH)
				{
					rect=GetRscRect(temp.pic_Index,temp1.eachWide,temp1.eachHeight,temp1.lSrcWCount);
					x=temp.Pos.x-Neutral_Common[temp.kind].eachWide/2-ScrPos.x;
					y=temp.Pos.y-Neutral_Common[temp.kind].eachHeight/2-ScrPos.y;
					DDraw.BlitBack(x,y,Neutral[temp.kind],
						&rect,DDBLTFAST_SRCCOLORKEY );
				}
			}
	//select显示
	for(i=0;i<select.size();i++){
		if(select[i].player ==2){//中立
			temp=Neutral_Unit[select[i].number];
			temp1=Neutral_Common[temp.kind];
			w=temp1.eachWide;
			h=temp1.eachHeight;
		}
		else if(select[i].Bui_Sol==0){//建筑
			temp=player[select[i].player].Building[select[i].number];
			temp1=Building_Common[player[select[i].player].race][temp.kind];
			w=temp1.eachWide;
			h=temp1.eachHeight;
			}
		else{//士兵
			temp=player[select[i].player].People[select[i].number];
			temp1=People_Common[player[select[i].player].race][temp.kind];
			w=temp1.eachWide/2;
			h=temp1.eachHeight/2;
			}
		if(temp.uniform_identifier == select[i].uniform_identifier){
		
		x=temp.Pos.x-w/2-ScrPos.x;
        y=temp.Pos.y-h/2-ScrPos.y;
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
		else{
			LONG uniform_temp=select[i].uniform_identifier;
			DeleteSelect(uniform_temp);
		}
	}

    //>>>>> 文字提示的显示 >>>>>
    //屏幕左上角位置的显示
    Text.TextBackSurf(5,5,ScrPos.x,RGB(255,255,255),0);
    Text.TextBackSurf(5,25,ScrPos.y,RGB(255,255,255),0);
    //场景属性的显示
    if( map.SceneH!=0 ) {
        Text.TextBackSurf(ScreenW-150,5,"场景宽为:",RGB(128,128,255),0);
        Text.TextBackSurf(ScreenW-60,5,map.SceneW,RGB(255,128,255),0);
        Text.TextBackSurf(ScreenW-150,25,"场景高为:",RGB(128,128,255),0);
        Text.TextBackSurf(ScreenW-60,25,map.SceneH,RGB(255,128,255),0);
    }
    //选择了编辑地面操作的提示
    if( (lEditState==0) && map.SceneH!=0 )
        Text.TextBackSurf(ScreenW/2-100,5, "地面编辑",RGB(255,255,0),0);
    //选择了编辑大树操作的提示
    else if( (lEditState==1) && map.SceneH!=0 )
    {
        Text.TextBackSurf(ScreenW/2-100,5,"树木编辑",RGB(255,255,0),0);
        Text.TextBackSurf(ScreenW/2-20,5,map.lObjectN,RGB(255,128,255),0);
    }
    //选择了编辑小树操作的提示
    else if( (lEditState==4) && map.SceneH!=0 )
    {
        Text.TextBackSurf(ScreenW/2-100,5,"金矿编辑",RGB(255,255,0),0);
        Text.TextBackSurf(ScreenW/2-20,5,map.lObjectN,RGB(255,128,255),0);
    }
    //文件名显示
    Text.TextBackSurf(5,ScreenH-20,sceFilePath,RGB(255,255,0),0);
    //保存状态的显示
    if( (lSaveState==2||lSaveState==0) && map.SceneH!=0 )
        Text.TextBackSurf(5,ScreenH-40,"还未保存",RGB(255,255,0),0);
}

/********************************************
*   函数名: EditProcess(...)
*     功能: 编辑器进程
*       (c) jk&ljq, 2010
*********************************************/
BOOL CEdit::EditProcess( void )
{
    //更新时间
    Time = GetTickCount();
    //>>>>> 程序的主进程 >>>>>>>>
    if( lAppState == 1 ){
        //状态变量还原
        RestoreVariable();
        //检测屏幕位置
        lUpdateScreen += CheckScrPos();
        //编辑地面
        lUpdateScreen += GroundEdit();
        //编辑对象
        lUpdateScreen += ObjectEdit();
        //将地面显示到后台表面
        if( lUpdateScreen>0 )
        {
            MakeGround();
            //地面上的和文字提示的显示
            MakeEntity();
            Text.ReMain();
            lUpdateScreen = 0;
            return TRUE;
        }
        else {
            lUpdateScreen = 0;
            return FALSE;
        }
    }
    lUpdateScreen = 0;
    return FALSE;
}

/***************************************************
*   函数名: GetInputDeviceData(...)
*     功能: 获得输入设备的状态
*       (c) jk&ljq, 2010
****************************************************/
void CEdit::GetInputDeviceData( LPRECT lpClientRt )
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
    else if( KeyMouse[0].Pos.y < -15 )
        KeyMouse[0].Pos.y = -15;
    //在场景中的位置
    MouseScePos.x = KeyMouse[0].Pos.x+ScrPos.x;
    MouseScePos.y = KeyMouse[0].Pos.y+ScrPos.y;
}
/*******************************************************
* 函数名 : SetRect(...)
*   功能 : 设置矩形内的地形数组
*   参数 : left  地图位置
*          top  地图位置
*          Width 图片宽
*          Height  图片高
********************************************************/
void CEdit::SetRect( LONG left, LONG top,
                           LONG Width, LONG Height,WORD data )
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
	ofstream result("result.txt",ios::app);
	ofstream Delete1("result.txt",ios::trunc);
    Delete1.close();
	LONG count=xCount*xCount;
	for(i=0;i<count;i++){
		result<<map.Square[i];
		if((i+1)%xCount==0)
			result<<endl;
	}
	result.close();
}
/*******************************************************
* 函数名 : CheckRect(...)
*   功能 : 检查矩形内的地形数组
*   参数 : left  地图位置
*          top  地图位置
*          Width 图片宽
*          Height  图片高
********************************************************/
BOOL CEdit::CheckRect( LONG left, LONG top,
                           LONG Width, LONG Height,WORD data )
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
/************************************************************
*   函数名: UpdateScreen(...)
*     功能: 更新屏幕
*************************************************************/
void CEdit::UpdateScreen( LPRECT lpClientRect )
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
