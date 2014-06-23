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


#define Direction_Down         0    //��
#define Direction_LeftDown     1    //����
#define Direction_Left         2    //��
#define Direction_LeftUp       3    //����
#define Direction_Up           4    //��
#define Direction_RightUp      5    //����
#define Direction_Right        6    //��
#define Direction_RightDown    7    //����
#define blet_time 800
#define goldup_time 1000
#define gold_limit 100000
#define population_limit 30

LPCSTR Bui[2][5]={{"Ҫ��","������Ӫ","�����Ӫ","ħ��֮��","������Ӫ��"},
{"����","����������","�����ٻ���","ʳ��ħ����","��ħС��"}};
LPCSTR Sol[2][5]={{"ũ��","���岽��","���","ħ��ʦ","������"},
		{"�๤","���岽��","������ʿ","ʳ��ħ��ʦ","��ħ������"}};
LPCSTR Defense_Kind[4]={"���","�м�","�ؼ�","��ǿ��"};//0:��� 1:�м� 2:�ؼ� 3:��ǿ�ͳǼ�
LPCSTR Attack_Kind[4]={"��ͨ","����","ħ��","����"};//0:��ͨ 1:���� 2:ħ�� 3:����
LONG Strategy[3][5]={{10,8,2,8,8},{5,6,4,6,6},{2,4,6,8,8}};
RECT SelectRECT[9];


/************************************************************
*   ������: CGame(...)
*     ����: ���캯��
*       (c) jk&ljq, 2010
*************************************************************/
CGame::CGame( void )
{
    hWnd        = NULL;
    bFullScr    = FALSE;//Ĭ��Ϊ����ģʽ
    ScreenW     = 640;//
    ScreenH     = 480;//Ĭ�Ϸֱ���640*480
    ColorBits   = 16;//Ĭ����ɫ���Ϊ16ɫ
    MaxWObject  = 340;//�������ͼƬ�������
    MaxHObject  = 300;//�������ͼƬ�����߶�

    lAppState   = 0;//��ʼ״̬

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
	player[0].race = 0;//Ĭ������
	player[1].race = 1;//Ĭ������
	UI=0;
    KeyKB           = NULL;
    KeyMouse        = NULL; 

	i=0;
	ifstream inf;							//��ȡ��ͼ���淽����
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
*   ������: ~CGame(...)
*     ����: ��������
*       (c) jk&ljq, 2010
*************************************************************/
CGame::~CGame( void )
{
    DeleteSceneData(map);
}

/********************************************************
*  ������:  GetRect(...)
*  ����:    ��ȡһ�����νṹ
*       (c) jk&ljq, 2010
*********************************************************/
RECT GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}
/********************************************************
*  ������:  GetRect(...)
*  ����:    ��ȡһ�����νṹ
*       (c) jk&ljq, 2010
*********************************************************/
BOOL inRect(RECT a,POINT b){
	if(b.x>=a.left && b.x <= a.right && b.y>= a.top && b.y <= a.bottom)
		return TRUE;
	return FALSE;
}
/************************************************************
*   ������: DoFor(...)
*     ����: ѭ���仯һ��Ȳ���������,��ֵ��8
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG CGame::DoFor( LONG lIndex,
                          LONG lEndCount,
                          LONG lStartNum,LONG Direct )
{
    // �����ڵ����ڶ��������Ҳ�С�ڵ�һ����
	if( lIndex<= lEndCount&& lIndex>=lStartNum )
        lIndex += 8;
    else
        lIndex = lStartNum;
	if(lIndex >lEndCount)
		lIndex = Direct+lStartNum;
	return	lIndex;
}
/************************************************************
*   ������: GetDirection(...)
*     ����: ���ָ����㵽ָ���յ�ķ���
*       (c) jk&ljq, 2010
*************************************************************/
INT CGame::GetDirection( POINT p1, POINT p2 )
{
    FLOAT   dx,dy,k;
    INT     Direct;
    dx = (FLOAT)(p2.x-p1.x);
    dy = (FLOAT)(p2.y-p1.y);
    k = dy/dx;//б��
    //��������
    if( dx>0 ) {
        if( dy>0 )
        {//(+,+)��һ����,k>0
            if( k<=0.4142 )//tan(22.5)==0.4142
                Direct = Direction_Right;
            else if(k<=2.4142)//tan(67.5)==2.4142
                Direct = Direction_RightDown;
            else
                Direct = Direction_Down;
        }
        else if( dy<0 )
        {//(+,-)��������,k<0
            if( k>-0.4142 )//tan(-22.5)
                Direct = Direction_Right;
            else if( k>-2.4142 )//tan(-67.5)
                Direct = Direction_RightUp;
            else
                Direct = Direction_Up;
        }
        else//�ں���������
            Direct = Direction_Right;
    }
    //���Ḻ��
    else if( dx<0 ) {
        if( dy>0 ) 
        {//(-,+)�ڶ�����,k<0
            if( k>-0.4142 )//tan(-22.5)
                Direct = Direction_Left;
            else if( k>-2.4142 )//tan(-67.5)
                Direct = Direction_LeftDown;
            else
                Direct = Direction_Down;
        }
        else if( dy<0 )
        {//(-,-)��������,k>0
            if( k<=0.4142 )//tan(22.5)
                Direct = Direction_Left;
            else if( k<=2.4142 )//tan(67.5)
                Direct = Direction_LeftUp;
            else
                Direct = Direction_Up;
        }
        else//�ں��Ḻ����
            Direct = Direction_Left;
	}
    //��������
    else{
		if( dy>0 )//������������
            Direct = Direction_Down;
        else//�����Ḻ����
            Direct = Direction_Up;
    }
    return	Direct;
}
/************************************************************
*   ������: LoadImageSurf(...)
*     ����: ����ͼ�������
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::LoadImageSurf( void )
{
    //���뿪ͷ����
    DDraw.CreateBMPSurface(lpDDSHeader,0,0,"data\\graphics\\ui\\Menu_background_without_title.bmp",DDSCAPS_SYSTEMMEMORY);
	//��������ʤ������
    DDraw.CreateBMPSurface(Win_Lose[0][0],0,0,"data\\graphics\\ui\\human\\victory.bmp",DDSCAPS_SYSTEMMEMORY);
	//��������ʧ�ܻ���
    DDraw.CreateBMPSurface(Win_Lose[0][1],0,0,"data\\graphics\\ui\\human\\defeat.bmp",DDSCAPS_SYSTEMMEMORY);
    //��������ʤ������
    DDraw.CreateBMPSurface(Win_Lose[1][0],0,0,"data\\graphics\\ui\\orc\\victory.bmp",DDSCAPS_SYSTEMMEMORY);
	//��������ʧ�ܻ���
    DDraw.CreateBMPSurface(Win_Lose[1][1],0,0,"data\\graphics\\ui\\orc\\defeat.bmp",DDSCAPS_SYSTEMMEMORY);
    //���밴ť
    if( DDraw.CreateBMPSurface(lpDDSButtonUp,0,0,"data\\graphics\\ButtonUP.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( lpDDSButtonUp,CLR_INVALID );
    if( DDraw.CreateBMPSurface(lpDDSButtonDown,0,0,"data\\graphics\\ButtonDown.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( lpDDSButtonDown,CLR_INVALID );
	if( DDraw.CreateBMPSurface(circle_draw.lpCircle[0],0,0,"data\\graphics\\Circlegreen.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( circle_draw.lpCircle[0],CLR_INVALID );
	if( DDraw.CreateBMPSurface(circle_draw.lpCircle[1],0,0,"data\\graphics\\Circlered.bmp",DDSCAPS_SYSTEMMEMORY) )
        DDraw.DDSetColorKey( circle_draw.lpCircle[1],CLR_INVALID );

	//�������
    DDraw.CreateBMPSurface(lpDDSGroundRC,0,0,"data\\graphics\\terrain\\summer.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Row,0,0,"data\\graphics\\Select_Row.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Col,0,0,"data\\graphics\\Select_Col.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSColor[0],0,0,"data\\graphics\\green.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSColor[1],0,0,"data\\graphics\\red.bmp",DDSCAPS_SYSTEMMEMORY);
	//����ͼ��
	DDraw.CreateBMPSurface(lpDDSIcons,0,0,"data\\graphics\\icons.bmp",DDSCAPS_SYSTEMMEMORY);
	
	ifstream inf;								
	int n,i=0;
	string a;

	inf.open("data\\Configure\\Human_Building.Conf");//��ȡ���彨������
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

	inf.open("data\\Configure\\Orc_Building.Conf");//��ȡ���彨������
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

	inf.open("data\\Configure\\Human_Solider.Conf");//��ȡ����ʿ������
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

	inf.open("data\\Configure\\Orc_Solider.Conf");//��ȡ����ʿ������
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

	inf.open("data\\Configure\\Neutral.Conf");//��ȡ������λ����
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

	inf.open("data\\Configure\\Interface.Conf");//��ȡ���涨��
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

	inf.open("data\\Configure\\Missile.Conf");//��ȡ��������
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
*   ������: InitDirectX(...)
*     ����: ��ʼ��DirectX�Լ���������ͼ������
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::InitDirectX( HWND _hWnd )
{
    hWnd = _hWnd;
    //>>>>>> ��ʼ��CDInput >>>>>>>>>>>
    Input.Set_bFullScr( FALSE );
    Input.InitDInput( hWnd );
    KeyKB = Input.Get_KeyKB();
    KeyMouse = Input.Get_KeyMouse();

    //>>>>>> ��ʼ��CDSound >>>>>>>>>>>
    Sound.Set_bSetPrimaryFormat( TRUE );
    //���ôμ���̬����������n��
    ifstream inf;							
	int n,k=0;
	string a;
    inf.open("data\\Configure\\Sound.Conf");//��ȡ��������
	inf>>n;
    Sound.Set_dwNumStatics( n );
    //���ôμ���ʽ����������Ϊ4��
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
        // ��������������������ʽ
        if( Sound.Get_bSetPrimaryFormat() )
            Sound.SetPrimaryBufferFormat( 2, 44100, 16 );
    }

    //>>>>>> ��ʼ��CDDraw >>>>>>>>>>>
    DDraw.InitDDraw(
        hWnd, 
        bFullScr,
        ScreenW, ScreenH,
        ColorBits );
    LoadImageSurf();//Ϊ��������ͼ������

    //>>>>>> ��ʼ��CText >>>>>>>>>>>
    Text.Set_lpDDSBack(DDraw.Get_lpDDSBack());
	//>>>>>> ��ʼ�������� >>>>>>>>>>>
	Initial_count();
}
/************************************************************
*   ������: SaveGroundDesc(...)
*     ����: ����ָ�������ĵ�������
*************************************************************/
void CGame::SaveGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	fwrite( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   ������: LoadGroundDesc(...)
*     ����: ����ָ�������ĵ�������
*************************************************************/
void CGame::LoadGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	//Ϊ���淽����������
	if( map.lSquareCount==0 )
        return;
    map.Grn = new LONG[map.lGrnN];
    fread( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   ������: SaveObject(...)
*     ����: ����ָ�������Ķ���
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
	//����
	LONG length =Neutral_Unit.size();
	fwrite( &length,sizeof(LONG),1,file);	
	for( LONG j=0;j<length;j++)
	{
		Unit unitTemp=Neutral_Unit[j];
		fwrite( &unitTemp,sizeof(Unit),1,file);
	}
}
/************************************************************
*   ������: LoadObject(...)
*     ����: ����ָ�������Ķ���
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
*   ������: SaveTerrainSquare(...)
*     ����: ����ָ�������ĵ�������
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::SaveTerrainSquare( FILE *file )
{
    if( file==NULL )
        return;
    fwrite( map.Square, sizeof(WORD)*map.lSquareCount,1,file);
}
/************************************************************
*   ������: LoadTerrainSquare(...)
*     ����: ����ָ�������ĵ�������
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
*   ������: SaveSceneData(...)
*     ����: ���泡������
*       (c) jk&ljq, 2010
*************************************************/
void CGame::SaveSceneData( BOOL bSaveAs )
{
    //�Ǳ���Ϊ�����ǵ�һ�α�����ѡ���ļ���
    if( !GetSceneFileName( 1 ) )
            return;
    //����ָ�������ļ�
    FILE *file = fopen(sceFilePath,"wb");

    //���泡���ṹ����
    fwrite(&map,sizeof(MAP),1,file);
	fwrite(&UI,sizeof(LONG),1,file);
    //������淽����������
    SaveGroundDesc( file );
    //�����������
    SaveObject( file );
    //������η�������
    SaveTerrainSquare( file);

    //�������湤��
    fclose(file);
    lSaveState = 1;//��ǰ���������Ѿ���Ϊ�ļ�
}
BOOL CGame::GetSceneFileName( DWORD dwFlag )
{
    // ����OPENFILENAME�ṹ
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
    if( dwFlag == 0 )//���ļ��༭
        ofn.lpstrTitle      = TEXT("ѡ��һ�������ļ�...");
    else//�����ļ�
        ofn.lpstrTitle      = TEXT("����Ϊָ�����ļ�...");
    ofn.Flags               = OFN_HIDEREADONLY;
    ofn.nFileOffset         = 0;
    ofn.nFileExtension      = 0;
    ofn.lpstrDefExt         = TEXT("map");
    ofn.lCustData           = 0L;
    ofn.lpfnHook            = NULL;
    ofn.lpTemplateName      = NULL;
	
    if( dwFlag == 0 )//���ļ��༭
        return GetOpenFileName( &ofn );
    else //�����ļ�
        return GetSaveFileName( &ofn );
}
/************************************************
*   ������: LoadSceneData(...)
*     ����: ����ָ������������
*************************************************/
void CGame::LoadSceneData( void )
{
	FILE *file;
	//��ô��ļ�������
	char OldDir[256];
	if(lAppState==2){
		  GetCurrentDirectory(256, OldDir);
		if( !GetSceneFileName( 0 ) ){
			lAppState=0;
			return;}
	}
    //�ͷžɳ���������
	DeleteSceneData( map );
    //��ָ�������ļ�
	if(lAppState==2){
		file = fopen(sceFilePath,"rb");
		SetCurrentDirectory(OldDir);
	}
	else if(lAppState == 1){
		file = fopen("data\\maps\\jk.map","rb");
	}
    //���볡���ṹ����
	if(file!=NULL){
    fread(&map,sizeof(MAP),1,file);
	fread(&UI,sizeof(LONG),1,file);
	}
    //������淽����������
    LoadGroundDesc( file );
    //�����������
    LoadObject( file );
    //������η�������
    LoadTerrainSquare( file );

    //�������빤��
   fclose(file);
}
/***********************************************
*   ������: MakeScene(...)
*     ����: ���ɵ�ǰ����(lSceneIDָ��)
************************************************/
void CGame::MakeScene( void )
{
    if( lAppState==1 || lAppState==2 )
		LoadSceneData();
	//>>>>>> ��ʼ��CGetPath >>>>>>>>>
	//���õ��η����СΪ16*16
	GPa.Set_lSquareC(16);
	//���ó�����С
	GPa.Set_SceneW(map.SceneW);
	GPa.Set_SceneH(map.SceneH);
	//�ѵ������鴫�͸�CGetPath����
	GPa.Set_Square((VOID*)map.Square);
}

/************************************************************
*   ������: DrawButton(...)
*     ����: ����ť
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::DrawButton( LPDIRECTDRAWSURFACE7 Surf,
                        WORD state, LPRECT lpRect )
{
    //��������
    RECT rect =GetRect(0,0,131,54);
    if( state == 0 )
        DDraw.BlitBack( lpRect->left,lpRect->top,lpDDSButtonDown,&rect,DDBLTFAST_SRCCOLORKEY);
    else
        DDraw.BlitBack( lpRect->left,lpRect->top,lpDDSButtonUp,&rect,DDBLTFAST_SRCCOLORKEY);
}

/*************************************************
*   ������: Header(...)
*     ����: ��Ϸ��ͷ����
*       (c) jk&ljq, 2010
**************************************************/
void CGame::Header( void )
{
	Sound.PlayStream(0,1);
    //��ʾ��������
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
    //��ť���
    if( KeyMouse[0].Pos.x>(320*x) &&
        KeyMouse[0].Pos.x<(450*x) &&
        KeyMouse[0].Pos.y>=270*y &&
        KeyMouse[0].Pos.y<324*y )
    {//�����"NEW GAME"ģʽ��
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//����������
            lAppState = 1;//��ģʽ
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
    {//�����"LOAD GAME"ģʽ��
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//����������
            lAppState = 2;//����ģʽ
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
    {//�����EXITģʽ��
        if( KeyMouse[0].Key==NULL &&KeyMouse[1].Key==DIMOFS_BUTTON0 )
        {//����������
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
*   ������: RestoreVariable(...)
*     ����: �ָ���Ҫ�ָ��ı���
*       (c) jk&ljq, 2010
***************************************************/
inline void CGame::RestoreVariable( void )
{
	
    LONG flag=0;
	//����select
	AdjustSelect();
	
	//ɾ��MissileQueue�������ĵ�λ
	vector<Missile>::iterator itr1 = MissileQueue.begin();
	for( itr1 = MissileQueue.begin(); itr1!=MissileQueue.end(); ++itr1 )
	{
		if(!CheckUI(itr1->select))//ͳһ��־����
		{
			itr1->select=AdjustUI(itr1->select);//����
			if(itr1->select.uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
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
	//ɾ��������λ
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
		//����
		itr = player[i].Building.begin();
		for( itr = player[i].Building.begin(); itr!=player[i].Building.end(); ++itr )
		{
			if( itr->bDead == TRUE )
			{
				//>>>>> �����Ӧ���ϰ� >>>>>>>
				LONG dis=soundDis(ScrPos,itr->Pos);
				Sound.SetStaticVolume(61+player[i].race,0-dis);
				Sound.PlayStatic(61+player[i].race,0,1);				
				player[i].Building.erase(itr);
				itr--;
			}
		}		
		//ʿ��
		itr = player[i].People.begin();
		for( itr = player[i].People.begin(); itr!=player[i].People.end(); ++itr )
		{
			if( itr->bDead == TRUE )
			{
				//>>>>> �����Ӧ���ϰ� >>>>>>>
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
				if(!CheckUI(itr->Target_Select))//ͳһ��־����
					itr->Target_Select=AdjustUI(itr->Target_Select);//����
				if(itr->Target_Select.uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
				{
					itr->state =UnitActionNone;
				    itr->pic_Index = itr->Direct;
					continue;
				}
				Unit *tempUnit = returnUnit(itr->Target_Select);
				if(tempUnit->bDead == TRUE|| tempUnit == NULL) //������������
				{
					itr->state =UnitActionNone;
					itr->pic_Index = itr->Direct;
				}
			}
		}
	}
	//ͬ��
	for( i=0;i<2;i++){//player
		//����
		/*itr = player[i].Building.begin();
		for( itr = player[i].Building.begin(); itr!=player[i].Building.end(); ++itr )
		{
			
		}*/		
		//ʿ��
		itr = player[i].People.begin();
		for( itr = player[i].People.begin(); itr!=player[i].People.end(); ++itr )
		{			
			if(itr->state == UnitActionFollow || itr->state == UnitActionAttackFollow ||
				itr->state == UnitActionAttack){
				if(!CheckUI(itr->Target_Select))//ͳһ��־����
					itr->Target_Select=AdjustUI(itr->Target_Select);//����
				if(itr->Target_Select.uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
				{
					itr->state =UnitActionNone;
					itr->pic_Index = itr->Direct;
					continue;
				}
			}
		}
	}
	//ͬ�������Ϣ
	AdjustAfterDelete();

	//���������е���Ϣ����
	itr = DeadQueue.begin();
	for( itr = DeadQueue.begin(); itr!=DeadQueue.end(); ++itr )
	{
		
		if( itr->state == UnitActionDie )
		{
			if(itr->lEndIndex-itr->pic_Index<8){//���ý���
				DeadQueue.erase( itr );
				itr--;
			}				
		}
	}
}
/**********************************************
*   ������: DeleteSceneData(...)
*     ����: ɾ��ָ������
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
    //�ͷŵ�������ռ�
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
*   ������: CheckScrPos(...)
*     ����: �����Ļλ��
***************************************************/
inline LONG CGame::CheckScrPos( void )
{
    //������Ļλ��
    POINT oldScrPos = ScrPos;

    //����ڿͻ����߽������ƶ���Ļ
    if( KeyMouse[0].Pos.x<=0 )
        ScrPos.x -= 10;
    else if( KeyMouse[0].Pos.x>=ScreenW-8 )
        ScrPos.x += 10;
    if( KeyMouse[0].Pos.y<=0 )
        ScrPos.y -= 10;
    else if( KeyMouse[0].Pos.y>=ScreenH-6 )
        ScrPos.y += 10;
    
    //��Ļλ�õı߽�����
    if( ScrPos.x<0 )
        ScrPos.x = 0;
    else if( ScrPos.x>(map.SceneW-ScreenW) )
        ScrPos.x = map.SceneW-ScreenW;
    if( ScrPos.y<0 )
        ScrPos.y = 0;
    else if( ScrPos.y>(map.SceneH-ScreenH) )
        ScrPos.y = map.SceneH-ScreenH;

    //�ı�����Ļλ���������Ļ
    if( oldScrPos.x!=ScrPos.x ||
        oldScrPos.y!=ScrPos.y )
        return 1;
    else//û�иı���Ļ�����������Ļ
        return 0;
}
/************************************************************
*   ������: MakeGround(...)
*     ����: ��ʾ����
*************************************************************/
void CGame::MakeGround( void )
{
    //��պ�̨����
    DDBLTFX ddbltfx; 
    ddbltfx.dwSize = sizeof(ddbltfx); 
    ddbltfx.dwFillColor = 0;//��ɫ
    DDraw.Get_lpDDSBack()->Blt(NULL,NULL,NULL,
                   DDBLT_COLORFILL,&ddbltfx);

    //>>>>>>> �������ʾ >>>>>>>>>>>
	LONG x,y,wN;
    RECT rect;
    //�󳡾�������ж��ٸ����淽��
    wN = map.SceneW/32;
	
    for( WORD n=0;n<map.lGrnN;n++ )
    {
        y = (LONG)(n/wN);//���к�
        x = n - y*wN;//���к�
        y = y*32-ScrPos.y;//��ת������Ļ�е�λ��
        x = x*32-ScrPos.x;//��ת������Ļ�е�λ��
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
*   ������: playsound(...)
*     ����: ����ѡ�ж�Ӧ��λ��������������Ƶ�ʱ仯
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
*   ������: soundDis(...)
*     ����: ���ݾ����������������
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
*   ������: soundIn(...)
*     ����: �жϷ�����λ�Ƿ�����Ļ֮��
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
*   ������: MakeEntity(...)
*     ����: ��ɫ�����������󡢹���ѡ��Ȧ��
*           ���ֵ���ʾ
*       (c) jk&ljq, 2010
*************************************************/
void CGame::MakeEntity( void )
{
    RECT rect;
    LONG x,y,i,j,w,h;
	Unit *temp;
	Unit_Common *temp1;
	vector<Unit>::iterator itr0 = DeadQueue.begin();
	//������ʾ
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
	//select��ʾ
	for(i=0;i<select.size();i++){
		temp = returnUnit(select[i]);
		if(temp->bDead == TRUE|| temp == NULL) //������������
		{
			continue;
		}
		temp1 = returnUnitCommon(select[i]);
		if(temp1==NULL)
			continue;
		w=temp1->eachWide;
		h=temp1->eachHeight;
		if(select[i].player!=2 &&select[i].Bui_Sol==1){//ʿ����Ȧ
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
			RectangleFrame(a,b);//�����ο�
		}
	}

	for(i=0;i<2;i++){//player
		//����
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
		//ʿ��
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
	//����
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
	//����
	vector<Missile>::iterator itr = MissileQueue.begin();
	for( itr = MissileQueue.begin(); itr!=MissileQueue.end(); ++itr )
	{
		temp1 = &Missile_Common[itr->kind];
		rect=GetRscRect(itr->pic_Index,temp1->eachWide,temp1->eachHeight,temp1->lSrcWCount);
		x=itr->pos.x-temp1->eachWide/2-ScrPos.x;
		y=itr->pos.y-temp1->eachHeight/2-ScrPos.y;
		DDraw.BlitBack(x,y,MissileSource[itr->kind],&rect,DDBLTFAST_SRCCOLORKEY );
	}   
	//����
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

	//select��
	if(select_draw){
		RectangleFrame(start_select,end_select);//�����ο�
	}

    //>>>>> ������ʾ����ʾ >>>>>
	for(i=0;i<2;i++){//player�������˿���ʾ
		for (j=0;j<5;j++)
		{
			Text.TextBackSurf(5+15*j,5+15*i,player[i].count[j],RGB(128,128,255),0,0);
		}
	}
	for (j=0;j<6;j++)//AI������ʾ
	{
		Text.TextBackSurf(5+15*j,35,player[1].strategy[j],RGB(128,128,255),0,0);
	}
	//select��ʾ
	rect=GetRect(0,0,46,38);
	if(select.size() == 1){		
		temp = returnUnit(select[0]);
		if(temp->bDead == TRUE|| temp == NULL) //��������
		{
		}
		else {
			temp1 = returnUnitCommon(select[0]);			
			if(select[0].Bui_Sol==0 && select[0].player!=2){//����
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
			else if(select[0].Bui_Sol==1 && select[0].player!=2)//ʿ��
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
			else//���
			{
				Text.TextBackSurf(ScreenW-120,10,"���",RGB(255,128,255),0,0);
			}
		}
	}
	else{
		for(i=0;i<select.size();i++){
			temp = returnUnit(select[i]);
			if(temp->bDead == TRUE|| temp == NULL) //������������
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
				//RectangleFrame(a,b);//�����ο�
			}
			DDraw.BlitBack(SelectRECT[i].left,SelectRECT[i].top,temp1->lpIcon,&rect,0 );
			LONG length = SelectRECT[i].right-SelectRECT[i].left;
			FLOAT rate = (FLOAT)temp->HP/temp1->Hp;
			DrawBloodBar(SelectRECT[i].left,SelectRECT[i].bottom,length,3,rate);
		}
	}    
}
/***********************************************
*   ������: CheckMessage(...)
*     ����: ����ɫ��Ϣ
*       (c) jk&ljq, 2010
************************************************/
void CGame::CheckMessage( void )
{
	if( KeyKB[1].Key==DIK_ESCAPE &&
		KeyKB[0].Key==NULL )
	{
		//�ص���ʼ����
		lAppState = 0;
		//ɾ���ɵĺ͵�ǰ�ĳ���
		DeleteSceneData(map);
		Sound.StopStream(1);
	}	
	if( KeyKB[1].Key==DIK_S &&
		KeyKB[0].Key==NULL )//���󣬽���ȥ��
	{
		//����
		SaveSceneData( TRUE );
		KeyKB[1].Key=NULL;
		return;
	}
	if( KeyKB[1].Key==DIK_1 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 1���
		vector<SelectUnit> temp_vector(select);
		select_1.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_1 &&
		KeyKB[0].Key==NULL )
	{
		//ѡȡ 1���
		//AdjustSelect(&select_1);
		vector<SelectUnit> temp_vector(select_1);
		select.swap(temp_vector);		
		return;
	}
	if( KeyKB[1].Key==DIK_2 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 2���
		vector<SelectUnit> temp_vector(select);
		select_2.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_2 &&
		KeyKB[0].Key==NULL )
	{
		//ѡȡ2���
		//AdjustSelect(&select_2);
		vector<SelectUnit> temp_vector(select_2);
		select.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_3 &&
		KeyKB[0].Key==DIK_LCONTROL )
	{
		//ctrl + 3���
		vector<SelectUnit> temp_vector(select);
		select_3.swap(temp_vector);
		return;
	}
	if( KeyKB[1].Key==DIK_3 &&
		KeyKB[0].Key==NULL )
	{
		//ѡȡ 3���
		//AdjustSelect(&select_3);
		vector<SelectUnit> temp_vector(select_3);
		select.swap(temp_vector);
		return;
	}
	//>>>>>> ������ָ���Ƿ񳬳�������Χ>>>>
    BOOL bMouseOutSce=FALSE;
    if( MouseScePos.x<0   ||
        MouseScePos.x >map.SceneW ||
        MouseScePos.y<0   ||
        MouseScePos.y >map.SceneH )
        bMouseOutSce = TRUE;
	
    //�����ѡ�����
	SelectUnit temp1;
	Unit *temp;
	LONG i,w,h;
	Unit_Common *temp2;
    if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 )
    {
		i = CheckIcon(KeyMouse[1].Pos);		
		//��⽨��������Ϣ
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
		//���ѡ��select
		else if(i!=-1){
			if(i<select.size()){
				temp1 = select[i];
				select.clear();
				select.push_back(temp1);
				return;
			}
		}
		//�������ͼ��
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
	//>>>>>>>>>��������Ҽ����߹���>>>>>>>>
    else if( KeyMouse[0].Key == DIMOFS_BUTTON1 &&
             !bMouseOutSce )
    {
		temp1 = returnSelect(MouseScePos);
		//select
		for(i=0;i<select.size();i++){
			if(!CheckUI(select[i]))//ͳһ��־����
				select[i]=AdjustUI(select[i]);//����
			if(select[i].uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
				continue;
			if(select[i].player ==2){//����
				
			}
			else if(select[i].Bui_Sol==0){//����
				
			}
			else{//ʿ��
				if((select[i].number < player[select[i].player].People.size())&& select[i].player==0 ){//�����Լ��ĵ�λ
					temp=&player[select[i].player].People[select[i].number];
					temp2=&People_Common[player[select[i].player].race][temp->kind];
					w=temp2->eachWide/2;
					h=temp2->eachHeight/2;
					if(temp1.uniform_identifier == -1){//������ǵ���
						temp->state = UnitActionMove;
						temp->Target=MouseScePos;						
						temp->lStartIndex = temp2->start_run;
						temp->lEndIndex = temp2->end_run;	
						playsound(select[i],1);
					}
					else {
						if(temp1.player==1){//ѡ�е���
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
							if(distance>=temp2->range){ //����̫Զ �޷�����
								temp->state = UnitActionAttackFollow;
								temp->lStartIndex = temp2->start_run;
								temp->lEndIndex = temp2->end_run;	
								playsound(select[i],2);
							}
						}
						if(temp1.player==0){//ѡ���Լ���λ
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
				else if(select[i].player==1){//���˵ĵ�λ
					select.clear();
				}
			}
		}   
	}

	//����ϳ�ѡ���
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
*   ������: CheckAIMessage(...)
*     ����: ����˹�������Ϣ
���㹥�����ȶ�  
����ʿ���������0.7+Ѫ����0.3 ԽС���ȹ���
���ڽ����������0.9+Ѫ����0.1 ԽС���ȹ���
************************************************/
void CGame::CheckAIMessage( void ){
	LONG threattemp,race,distance,count;
	LONG threat= 9999;
	Unit_Common *temp1;
	SelectUnit  temp2;
	//����Լ����˹����ܣ����빥����Χ�Զ�����
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
					//���빥����Χ
					if(temp1->sight >= distance){ 
						threattemp = distance*0.7+ itr1->HP*0.3;
						//���ȶȸ�
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
			//û���˿��Թ���ʱ��⽨��
			if(itr0->state == UnitActionNone)
			{
				itr1 = player[1].Building.begin();
				count=0;
				while (itr1 != player[1].Building.end()){
					if(itr1->bDead == FALSE){
						temp1 = &People_Common[race][itr0->kind];
						distance = GPa.Distance(itr0->Pos,itr1->Pos);
						//���빥����Χ
						if(temp1->sight >= distance){ 
							threattemp = distance*0.9+ itr1->HP*0.1;
							//���ȶȸ�
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

	//�����Ե��˹�����
	//״̬���
	if(player[1].state == 1  && player[1].population<=8){
		player[1].state = 0;//ֹͣ����  ��ʼ��չ
	}
	if(player[1].state == 0  && player[1].population-player[0].population>=8){
		player[1].state = 1;//ȫ������
		player[1].ArmyPos = CalArmyBuildingCenterPos(0);
	}
	DWORD dif_time = Time - game_start_time;
	if(player[1].strategy[5]!=2 && dif_time > 360000){//�������2
		setStrategy(1,2);
	}
	else if (player[1].strategy[5]!=2 &&
		player[1].strategy[5]!=1 && dif_time>180000)//�������1
	{
		setStrategy(1,1);
	}
	//��̬�޸Ĳ���
	if (player[0].count[0]>2 )
	{
		//��ħ��ũ��
		player[1].strategy[4] = player[0].count[0];
	}
	if (player[0].count[1]>5)
	{
		//��ʿ�˲���
		player[1].strategy[2] = player[0].count[1];
	}
	if (player[0].strategy[2]>5)
	{
		//ħ��ʦ����ʿ
		player[1].strategy[3] = player[0].count[2];
	}
	if (player[0].count[3]>5 || player[0].count[4]>5)
	{
		//������ħ��ʦ����ħ,������
		player[1].strategy[1] = player[0].count[3]+player[0].count[4];
	}
	//����ʿ��AI�����빥����Χ�Զ�����
	race = player[1].race;
	itr1 = player[1].People.begin();
	BOOL flag = FALSE;
	while (itr1 != player[1].People.end())
	{
		threat= 9999;
		if(itr1->state == UnitActionAttackFollow || itr1->state == UnitActionAttack){//����Ŀ��Ϊ����
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
					//���빥����Χ
					if(temp1->sight >= distance){ 
						threattemp = distance*0.7+ itr0->HP*0.3;
						//���ȶȸ�
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
			//û���˿��Թ���ʱ��⽨��
			if(itr1->state == UnitActionNone)
			{
				itr0 = player[0].Building.begin();
				count=0;
				while (itr0 != player[0].Building.end()){
					if(itr0->bDead == FALSE){
						temp1 = &People_Common[race][itr1->kind];
						distance = GPa.Distance(itr0->Pos,itr1->Pos);
						//���빥����Χ
						if(temp1->sight >= distance){ 
							threattemp = distance*0.9+ itr0->HP*0.1;
							//���ȶȸ�
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
	//���Խ���AI
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
*   ������: MessageProcess(...)
*     ����: �����ɫ��Ϣ
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
		//����
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
		//ʿ��
		itr0 = player[i].People.begin();
		for(itr0 = player[i].People.begin(); itr0!=player[i].People.end(); ++itr0){
			temp2 = &People_Common[player[i].race][itr0->kind];
			w = temp2->eachWide/2;
			h = temp2->eachHeight/2;
			dis=0;
			//�ƶ���׷��
			if(((itr0->state == UnitActionMove)||(itr0->state==UnitActionAttackFollow)||(itr0->state==UnitActionFollow))
				&& (Time-itr0->startTime >= temp2->movespeed)){
				if(itr0->state==UnitActionAttackFollow || itr0->state==UnitActionFollow){//����  ������
					Unit *temp3 = returnUnit(itr0->Target_Select);
					if(temp3->bDead==TRUE || temp3 == NULL){
						itr0->state = UnitActionNone;
						continue;
					}
					LONG distance = GPa.Distance(temp3->Pos,itr0->Pos);
					if(itr0->Target_Select.player==2 ){//��������Ϊ����  ����ӳ�
						Unit_Common *temp4 = &Neutral_Common[temp3->kind];
						dis = (temp4->eachHeight+temp4->eachWide)/4;
					}
					else if(itr0->Target_Select.Bui_Sol==0){//��������Ϊ����  ����ӳ�
						Unit_Common *temp4 = &Building_Common[itr0->Target_Select.player][itr0->Target_Select.number];
						dis = (temp4->eachHeight+temp4->eachWide)/4;
					}
					itr0->Target = temp3->Pos;
					if(distance<temp2->range+dis){ //�����㹻�� ��ʼ������ͣ��												
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
				//end ����  ������
				GPa.Set_Square((VOID*)map.Square);
				itr0->oldDirect = itr0->Direct;
				itr0->Direct = GetDirection( itr0->Pos,itr0->Target);				
				map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 0;//�������ķ���ָ�Ϊ���ϰ�
				itr0->lpPath = GPa.GetTheBestPath(itr0->Pos, itr0->Target, 1);
				if(itr0->lpPath == NULL ){
					itr0->state = UnitActionNone;
					itr0->pic_Index = itr0->oldDirect;
					map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 1;//�������ķ�����Ϊ�ϰ�
					//player[i].People[j] = temp;
					continue;
				}
				if(itr0->oldDirect != itr0->Direct){  //����ͼƬ����
					itr0->lStartIndex = temp2->start_run;
					itr0->lEndIndex = temp2->end_run;
					itr0->pic_Index = itr0->Direct+itr0->lStartIndex;
				}
				itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);
				itr0->Pos =  GPa.AdjustPos(itr0->lpPath->Pos);
				itr0->startTime = Time;
				map.Square[GPa.GetSquareFromPos(itr0->Pos)] = 1;//�������ķ�����Ϊ�ϰ�				
				GPa.DeletePath( itr0->lpPath );
				//player[i].People[j] = temp;
			}
			//����
			else if(itr0->state == UnitActionAttack && (Time-itr0->startTime >= temp2->attackspeed)){	
				itr0->oldDirect = itr0->Direct;
				Unit *temp3 = returnUnit(itr0->Target_Select);
				if(temp3 == NULL || temp3->bDead == TRUE){
					itr0->state = UnitActionNone;
					continue;
				}
				LONG distance = GPa.Distance(temp3->Pos,itr0->Pos);
				if(itr0->Target_Select.player==2 ){//��������Ϊ����
					Unit_Common *temp4 = &Neutral_Common[temp3->kind];
					distance =0;
				}
				else if(itr0->Target_Select.Bui_Sol==0){//��������Ϊ����
					distance =0;
				}
				if(distance>temp2->range){
					itr0->state = UnitActionAttackFollow;
					continue;
				}
				itr0->Direct = GetDirection( itr0->Pos,temp3->Pos);
				if(itr0->oldDirect != itr0->Direct){  //����ͼƬ����
					itr0->lStartIndex = temp2->start_attack;
					itr0->lEndIndex = temp2->end_attack;
					itr0->pic_Index = itr0->Direct+itr0->lStartIndex;
				}
				itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);
				if(itr0->lEndIndex-itr0->pic_Index<8){//�˺�
					LONG dis=soundDis(ScrPos,itr0->Pos);
					Sound.SetStaticVolume(People_Common[player[i].race][itr0->kind].sound[3],0-dis);
					Sound.PlayStatic(People_Common[player[i].race][itr0->kind].sound[3],0,1);
					//���̵�λ
					if(temp2->range<100){
						if(setDamage(itr0->Target_Select,temp2->attackpower,temp2->attack_kind)){
							itr0->state = UnitActionNone;
							itr0->pic_Index = itr0->Direct;
						}
					}
					//Զ�̵�λ
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
	//����
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
			//δ����
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
			//����Ŀ��
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
		if(Time-itr0->startTime >= blet_time){//��������
			itr0->pic_Index = DoFor(itr0->pic_Index,itr0->lEndIndex,itr0->lStartIndex,itr0->Direct);	
			itr0->startTime = Time;
		}
	}   
}
/*********************************************************
*   ������: CheckWin(...) 
*     ����: ���ʤ��
**********************************************************/
void CGame::CheckWin(){
	if(player[1].Building.size()==0)
		lAppState = 4;
	if(player[0].Building.size()==0)
		lAppState = 4;
}
/************************************************************
*   ������: DeadProcess(...)
*     ����: ������
*************************************************************/
void CGame::DeadProcess( void )
{
    //���¡�ESC�������¿�ʼ
   //���¡�ESC�������¿�ʼ
    if( KeyKB[1].Key==DIK_ESCAPE &&
        KeyKB[0].Key==NULL )
    {
        //�ص���ʼ����
        lAppState = 0;
        //ɾ���ɵĺ͵�ǰ�ĳ���
        DeleteSceneData(map);
		Sound.StopStream(2);
        Sound.StopStream(3);
    }
	else {
		//ʤ��
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
	//ʧ��
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
	    Text.TextBackSurf(150,200,"ʿ������",RGB(255,0,0),0,0);
		Text.TextBackSurf(250,200,"ʿ������",RGB(0,0,139),0,0);
		Text.TextBackSurf(350,200,"������ʧ",RGB(230,230,250),0,0);
		Text.TextBackSurf(450,200,"ʿ����ɱ",RGB(119,136,153),0,0);
		Text.TextBackSurf(550,200,"�����ݻ�",RGB(255,69,0),0,0);
		Text.TextBackSurf(650,200,"�ܵ÷�",RGB(64,224,208),0,0);
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
*   ������: GameProcess(...)
*     ����: ��Ϸ����
*       (c) jk&ljq, 2010
*********************************************/
void CGame::GameProcess( void )
{
    //����ʱ��
    Time = GetTickCount();
    //>>>>>> ����Ŀ�ʼ���� >>>>>>
    if( lAppState==0 )
    {
        //����Ŀ�ͷ
        Header();
        if( lAppState==1 || lAppState==2 )
        {//������Ϸ
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
    //>>>>> ����������� >>>>>>>>
    else if( lAppState == 3 ){
		Sound.PlayStream(1,1);
		//������λ��
		CheckScrPos();
		//��������Ӧ��Ϣ
		CheckMessage();
		//���AI��Ϣ
		CheckAIMessage();
		//����
        MessageProcess();
		//����λ״̬���
        RestoreVariable();
        //��������ʾ����̨����
		MakeGround();
        //��ʾ��λ
		MakeEntity();
        //���ֵ���ʾ
		Text.ReMain();
		//���ʤ����Ϣ
		CheckWin();
		if( KeyKB[1].Key==DIK_ESCAPE &&
			KeyKB[0].Key==NULL )
		{
			//�ص���ʼ����
			lAppState = 0;
			//ɾ���ɵĺ͵�ǰ�ĳ���
			DeleteSceneData(map);
			Sound.StopStream(1);
		}		

    }
    //>>>>> ��ɫ�������� >>>>>>>>
    else if( lAppState==4 )
    {
        DeadProcess();
        //���ֵ���ʾ
        Text.ReMain();
    }
    //>>>>>> �˳����� >>>>>>>
    else//lAppState==5
        DestroyWindow( hWnd );
}

/***************************************************
*   ������: GetInputDeviceData(...)
*     ����: ��������豸��״̬
*       (c) jk&ljq, 2010
****************************************************/
void CGame::GetInputDeviceData( LPRECT lpClientRt )
{
    //>>>> ��ü��̺��������״̬ >>>>
    Input.GetDInputData();

    //>>>> �������λ�õ�ֵ >>>>>>>
    if( !bFullScr )//�Ǵ���ģʽ��������λ��
    {
        KeyMouse[0].Pos.x -= lpClientRt->left;
        KeyMouse[0].Pos.y -= lpClientRt->top;
    }
    //����߽�����
    if( KeyMouse[0].Pos.x > ScreenW-8 )
        KeyMouse[0].Pos.x = ScreenW-8;
    else if( KeyMouse[0].Pos.x < 0 )
        KeyMouse[0].Pos.x = 0;
    //����߽�����
    if( KeyMouse[0].Pos.y > ScreenH-6 )
        KeyMouse[0].Pos.y = ScreenH-6;
    else if( KeyMouse[0].Pos.y < 0 )
        KeyMouse[0].Pos.y = 0;
    //�ڳ����е�λ��
    MouseScePos.x = KeyMouse[0].Pos.x+ScrPos.x;
    MouseScePos.y = KeyMouse[0].Pos.y+ScrPos.y;
}

/************************************************************
*   ������: UpdateScreen(...)
*     ����: ������Ļ
*       (c) jk&ljq, 2010
*************************************************************/
void CGame::UpdateScreen( LPRECT lpClientRect )
{
    DDraw.Flip( lpClientRect );
}
/*******************************************************
* ������ : GetRscRect(...)
*   ���� : ���ݶ������ȡ�ö�����ԴͼƬ����ľ���λ��
*   ���� : lIndex  Ҫ��ı��
*          lWidth  ÿ������ͼƬ�Ŀ�
*          lHeight ÿ������ͼƬ�ĸ�
*          lCount  ÿ���ж��ٸ�ͼƬ
********************************************************/
inline RECT GetRscRect( LONG lIndex, LONG lWidth,
                           LONG lHeight, LONG lCount )
{
    RECT rect;  LONG x,y;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //����ڵڼ���(0,1,2,3,...���ϵ���)
    y = (LONG)( lIndex/lCount );

    //������еĵڼ���(0,1,2,3,...������)
    x = lIndex - y*lCount;

    rect.left	= x*lWidth;
    rect.top	= y*lHeight;

    rect.right	= rect.left + lWidth;
    rect.bottom	= rect.top + lHeight;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    return rect;
}
/**************************************************
*   ������: DeleteSelect(...)
*     ����: ɾ��ָ���Ķ���
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
* ������ : SetRect(...)
*   ���� : ���þ����ڵĵ�������
*   ���� : left  ��ͼλ��
*          top  ��ͼλ��
*          Width ͼƬ��
*          Height  ͼƬ��
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
	if((x1*16)==(right+1))//����
		x1++;
    y1 = (LONG)( bottom/16 );
	if((y1*16)==(bottom+1))//����
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
* ������ : CheckRect(...)
*   ���� : �������ڵĵ�������
*   ���� : left  ��ͼλ��
*          top  ��ͼλ��
*          Width ͼƬ��
*          Height  ͼƬ��
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
	if((x1*16)==(right+1))//����
		x1++;
    y1 = (LONG)( bottom/16 );
	if((y1*16)==(bottom+1))//����
		y1++;
	xCount=(LONG)(map.SceneW/16);
	for(i=x;i<=x1;i++){
		for(j=y;j<=y1;j++){
			if(map.Square[j*xCount+i]!=data){//breakֻ����һ��ѭ��
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
* ������ : returnSelect(...)
*   ���� : �������ڵĵ�λ ����ѡ��
*   ���� : pos  ��ͼλ��
********************************************************/
SelectUnit CGame::returnSelect(POINT pos){
	LONG i,j;
	SelectUnit temp1;
	Unit *temp;
	for( i=0;i<2;i++){//player
			for(j=0;j<player[i].Building.size();j++){//����
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
			for(j=0;j<player[i].People.size();j++){//ʿ��
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
		for(j=0;j<Neutral_Unit.size();j++){//����
				temp=&Neutral_Unit[j];
				LONG w = Neutral_Common[temp->kind].eachWide/2;
				LONG h = Neutral_Common[temp->kind].eachHeight/2;
				if(pos.x>(temp->Pos.x-w) &&
					pos.x<(temp->Pos.x+w) &&
					pos.y>(temp->Pos.y-h) &&
					pos.y<(temp->Pos.y+h))
				{
					temp1.player=2;//����
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
* ������ : AdjustUI(...)
*   ���� : ����ͳһ��־ֵ
*   ���� : ui  ѡ��λ
********************************************************/
SelectUnit CGame::AdjustUI(SelectUnit ui){
	LONG i;
	if(ui.player ==2){//����
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
	else if(ui.Bui_Sol == 0){ //����
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
	else if(ui.Bui_Sol == 1){ //ʿ��
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
* ������ : CheckUI(...)
*   ���� : ���ѡ��λ
*   ���� : ui  ѡ��λ
********************************************************/
BOOL CGame::CheckUI(SelectUnit ui){
	if(ui.player ==2 && ui.number<Neutral_Unit.size() && 
		Neutral_Unit[ui.number].uniform_identifier==ui.uniform_identifier){//����
		return TRUE;						
	}
	else if(ui.Bui_Sol == 0){ //����
		if(ui.number < player[ui.player].Building.size() && 
			player[ui.player].Building[ui.number].uniform_identifier==ui.uniform_identifier)
			return TRUE;
	}
	else if(ui.Bui_Sol == 1){ //ʿ��
		if(ui.number < player[ui.player].People.size() &&
			player[ui.player].People[ui.number].uniform_identifier==ui.uniform_identifier)
			return TRUE;
	}
	return FALSE;
}
/*******************************************************
* ������ : returnUnit(...)
*   ���� : ����ѡ��λ
*   ���� : ui  ѡ��λ
********************************************************/
Unit *CGame::returnUnit(SelectUnit ui){
	Unit *temp=NULL;
	if(!CheckUI(ui))//ͳһ��־����
	{
		ui=AdjustUI(ui);//����
	}
	if(ui.uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
	{
		return temp;
	}
	if(ui.player ==2 && ui.number<Neutral_Unit.size() && 
		Neutral_Unit[ui.number].uniform_identifier==ui.uniform_identifier){//����
		temp = &Neutral_Unit[ui.number];
		return temp;						
	}
	else if(ui.Bui_Sol == 0){ //����
		if(ui.number < player[ui.player].Building.size() &&
			player[ui.player].Building[ui.number].uniform_identifier==ui.uniform_identifier)
			temp = &player[ui.player].Building[ui.number];
			return temp;
	}
	else if(ui.Bui_Sol == 1){ //ʿ��
		if(ui.number < player[ui.player].People.size() &&
			player[ui.player].People[ui.number].uniform_identifier==ui.uniform_identifier)
			temp = &player[ui.player].People[ui.number];
			return temp;
	}
	return temp;	
}
/*******************************************************
* ������ : returnUnitCommon(...)
*   ���� : ����ѡ��λ�Ĺ�������
*   ���� : ui  ѡ��λ
********************************************************/
Unit_Common *CGame::returnUnitCommon(SelectUnit ui){
	//Unit temp = returnUnit(ui);
	Unit_Common *temp1=NULL;
	if(ui.player ==2 ){//����
		if (ui.kind>=0 && ui.kind<1)
		{
			temp1 = &Neutral_Common[ui.kind];
			return temp1;
		}
								
	}
	else if(ui.Bui_Sol == 0){ //����
		if (ui.kind>=0 && ui.kind<5)
		{
			temp1 = &Building_Common[player[ui.player].race][ui.kind];
			return temp1;
		}
	}
	else if(ui.Bui_Sol == 1){ //ʿ��
		if (ui.kind>=0 && ui.kind<5)
		{
			temp1 = &People_Common[player[ui.player].race][ui.kind];
			return temp1;
		}
	}
	return temp1;
}
/*******************************************************
* ������ : RectangleFrame(...)
*   ���� : �����ο�
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
* ������ : setDamage(...)
*   ���� : ����ѡ��λ���˺�
*   ���� : ui  ѡ��λ
*	����ֵ��TRUE �˺���λ���� FALSE �˺���λ�����
********************************************************/
BOOL CGame::setDamage(SelectUnit ui,LONG damage,LONG _attack_kind){
	Unit *temp = returnUnit(ui);
	if(temp->bDead == TRUE|| temp == NULL)//�Է��Ѳ�����
			return TRUE;
	Unit_Common *temp2 = returnUnitCommon(ui);
	if(temp2==NULL)
		return TRUE;
	Unit *temp3;
	if(ui.player ==2 ){//����
		temp3= &Neutral_Unit[ui.number];
		damage = damage*(10-temp2->defense)*0.1;
		temp3->HP -= damage;
		if(temp3->HP<=0){
			temp3->HP=0;
			temp3->bDead = TRUE;
			temp3->state = UnitActionDie;
			//�������ķ���ָ�Ϊ���ϰ�
			LONG w=temp2->eachWide;
			LONG h=temp2->eachHeight;
			SetRect(temp3->Pos.x-w/2,temp3->Pos.y-h/2,w,h,FALSE);
			return TRUE;
		}
	}
	else if(ui.Bui_Sol == 0){ //����
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
			//�������ķ���ָ�Ϊ���ϰ�
			LONG w=temp2->eachWide;
			LONG h=temp2->eachHeight;
			SetRect(temp3->Pos.x-w/2,temp3->Pos.y-h/2,w,h,FALSE);
			return TRUE;
		}		
	}
	else if(ui.Bui_Sol == 1){ //ʿ��
		temp3= &player[ui.player].People[ui.number];
		damage = returnDamage(damage,temp2->defense,_attack_kind,temp2->defense_kind);
		temp3->HP -= damage;
		if(temp3->HP<=0){
			temp3->HP=0;
			player[ui.player].population -= temp2->population_cost;
			map.Square[GPa.GetSquareFromPos(temp3->Pos)] = 0;//�������ķ���ָ�Ϊ���ϰ�
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
* ������ : returnDamage(...)
*   ���� : ����ѡ��λ���˺�
*   ���� : ui  ѡ��λ

0:��� 1:�м� 2:�ؼ� 3:��ǿ�ͳǼ�
0:��ͨ 1:���� 2:ħ�� 3:����

-----���� ���� ���� ����  
��ͨ 150% 100% 100% 70%   
���� 75%  100% 200% 35%   
ħ�� 75%  200% 100% 35%   
���� 100% 100% 100% 100%  
 
�����˺�֮���� =((װ��)*0.06)/)(1+0.06*(װ��)
********************************************************/
LONG CGame::returnDamage(LONG damage,LONG defense,LONG src_attack_kind,LONG des_defense_kind){
	switch (src_attack_kind)
	{
	case 0://��ͨ����
		switch (des_defense_kind)
		{
		case 1: damage = (FLOAT)damage * 1.5;
			break;
		case 3: damage = (FLOAT)damage * 0.5;
			break;
		}
		break;
	case 1://���̹���
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
	case 2://ħ������
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
	case 3://���ҹ���
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
* ������ : selectInFrame(...)
*   ���� : ѡ����ѡ����еĵ�λ
*   ���� : a ��� b �յ�
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
	//ʿ��
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
		if(select_frame.size()>=8)//���һ��ѡ8����
			break;
	}
	if(select_frame.size()){
		swap(select,select_frame);
		playsound(temp,0);
	}
}
/*********************************************************
*   ������: OneStep(...) 
*     ����: ��ָ���������ƶ�һ��
* ����˵��: pos�����, direct ������
**********************************************************/
POINT CGame::OneStep( POINT pos,LONG direct ){
	pos = GPa.AdjustPos(pos);
	switch(direct){
	case Direction_Down:{	//��
		pos.y +=16;
		break;
		   }
	case Direction_LeftDown:{	//����
		pos.x -=16;
		pos.y +=16;
		break;
		   }
	case Direction_Left:{	//��
		pos.x -=16;
		break;
		   }
	case Direction_LeftUp:{	//����
		pos.x -=16;
		pos.y -=16;
		break;
		   }
	case Direction_Up:{	//��
		pos.y -=16;
		break;
		   }
	case Direction_RightUp:{	//����
		pos.x +=16;
		pos.y -=16;
		break;
		   }
	case Direction_Right:{	//��
		pos.x +=16;
		break;
		   }
	case Direction_RightDown:{	//����
		pos.x +=16;
		pos.y +=16;
		break;
		}
	}
	return pos;
}
/*********************************************************
*   ������: DrawIcon(...) 
*     ����: ��ָ��λ�û�ͼ��
* ����˵��: pos��λ��, i ��ͼ��Դ����ļ���
**********************************************************/
void CGame::DrawIcon(POINT pos,LONG i){
	RECT rect = GetRscRect(i,46,38,5);
	DDraw.BlitBack(pos.x,pos.y,lpDDSIcons,&rect,0 );
}
/*********************************************************
*   ������: CheckIcon(...) 
*     ����: ��⽻��  �Ƿ���ͼ�걻����
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
*   ������: DrawBloodBar(...) 
*     ����: ��ָ��λ�û�Ѫ��
* ����˵��: startx ���x starty ���y length �� height �� rate Ѫ����
**********************************************************/
void CGame::DrawBloodBar(LONG startx,LONG starty,LONG length,LONG height,FLOAT rate){	
	LONG x=length*rate;
	RECT rect = GetRect(0,0,x,height);
	DDraw.BlitBack(startx,starty,lpDDSColor[0],&rect,0 );
	rect.right = length - x;
	DDraw.BlitBack(startx+x,starty,lpDDSColor[1],&rect,0 );
}
/*********************************************************
*   ������: CalArmyBuildingCenterPos(...) 
*     ����: ������ҽ���������λ��
* ����˵��: i player[i]
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
		while (itr != player[0].Building.end())//����player[i]
		{
			dis = GPa.Distance(temp,itr->Pos);
			if(dis>400)//���������������
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
*   ������: setStrategy(...) 
*     ����: ������ҽ���������λ��
* ����˵��: i player[i]ѡ��j����
**********************************************************/
void CGame::setStrategy(LONG i,LONG j){
	for(LONG m=0;m<5;m++){
		player[i].strategy[m]=Strategy[j][m];
	}
	player[i].strategy[5]=j;
}
/*********************************************************
*   ������: AdjustSelect(...) 
*     ����: ����ͳһ��׼������select�е�����
* ����˵��: 
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
*   ������: AdjustSelect(...) 
*     ����: ɾ�������ͳһ��׼������select�е�����
* ����˵��: 
**********************************************************/
void CGame::AdjustAfterDelete(){
	vector<SelectUnit>::iterator itr0 = select.begin();
	for( itr0 = select.begin(); itr0!=select.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//ͳһ��־����
			*itr0=AdjustUI(*itr0);//����
		/*if(ui.uniform_identifier == -1) //�������ָõ�λ�Ѳ�����
		{
			return temp;
		}*/
	}

	itr0 = select_1.begin();
	for( itr0 = select_1.begin(); itr0!=select_1.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//ͳһ��־����
			*itr0=AdjustUI(*itr0);//����
	}

	itr0 = select_2.begin();
	for( itr0 = select_2.begin(); itr0!=select_2.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//ͳһ��־����
			*itr0=AdjustUI(*itr0);//����
	}

	itr0 = select_3.begin();
	for( itr0 = select_3.begin(); itr0!=select_3.end(); ++itr0 )
	{		
		if(!CheckUI(*itr0))//ͳһ��־����
			*itr0=AdjustUI(*itr0);//����
	}
}
