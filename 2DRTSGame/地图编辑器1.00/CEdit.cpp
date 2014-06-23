/*-------------------------------------------
|              CEdit.cpp
|             �༭�����
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
*   ������: CEdit(...)
*     ����: ���캯��
*************************************************************/
CEdit::CEdit( void )
{
    hWnd        = NULL;
    bFullScr    = FALSE;//Ĭ��Ϊ����ģʽ
    ScreenW     = 640;//
    ScreenH     = 480;//Ĭ�Ϸֱ���640*480
    ColorBits   = 16;//Ĭ����ɫ���Ϊ16ɫ
    MaxWObject  = 340;//�������ͼƬ�������
    MaxHObject  = 300;//�������ͼƬ�����߶�
    ScrPos.x    = 0;
    ScrPos.y    = 0;
    ZeroMemory( &map, sizeof(MAP) );
    lAppState   = 0;
    lUpdateScreen = 1;
    lSaveState  = 0;
    lEditState  = 0;//����
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
	player[0].race = 0;//Ĭ������
	player[1].race = 1;//Ĭ������
	IorAI=0;//Ĭ���Լ�
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
	ifstream inf;							//��ȡ��ͼ���淽����
	inf.open("data\\Configure\\Map.Conf");
	LONG data;
	while (inf>>data)
	{
		GroundRC[i++]=data;
	}
	inf.close();
}

/************************************************************
*   ������: ~CEdit(...)
*     ����: ��������
*       (c) jk&ljq, 2010
*************************************************************/
CEdit::~CEdit( void )
{
    DeleteSceneData(map);
}

/********************************************************
*  ������:  GetRect(...)
*  ����:    ��ȡһ�����νṹ
*       (c) jk&ljq, 2010
*********************************************************/
RECT CEdit::GetRect(LONG left,LONG top,LONG right,LONG bottom)
{
    RECT rect = {left,top,right,bottom};
    return rect;
}


/************************************************************
*   ������: LoadImageSurf(...)
*     ����: ����ͼ�������
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::LoadImageSurf( void )
{
	//�������
    DDraw.CreateBMPSurface(lpDDSGroundRC,0,0,"data\\graphics\\terrain\\summer.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Row,0,0,"data\\graphics\\Select_Row.bmp",DDSCAPS_SYSTEMMEMORY);
	DDraw.CreateBMPSurface(lpDDSSelect_Col,0,0,"data\\graphics\\Select_Col.bmp",DDSCAPS_SYSTEMMEMORY);
	//����ͼ��
	
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

	/*inf.open("data\\Configure\\Interface.Conf");//��ȡ���涨��
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

	/*inf.open("data\\Configure\\Missile.Conf");//��ȡ��������
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
*   ������: InitDirectX(...)
*     ����: ��ʼ��DirectX�Լ���������ͼ������
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::InitDirectX( HWND _hWnd )
{
    hWnd = _hWnd;
    //>>>>>> ��ʼ��CDInput >>>>>>>>>>>
    Input.Set_bFullScr( FALSE );
    Input.InitDInput( hWnd );
    KeyKB = Input.Get_KeyKB();
    KeyMouse = Input.Get_KeyMouse();

    //>>>>>> ��ʼ��CDDraw >>>>>>>>>>>
    DDraw.InitDDraw(
        hWnd, 
        bFullScr,
        ScreenW, ScreenH,
        ColorBits );
    LoadImageSurf();//Ϊ��������ͼ������

    //>>>>>> ��ʼ��CText >>>>>>>>>>>
    Text.Set_lpDDSBack(DDraw.Get_lpDDSBack());
}
/***********************************************
*   ������: NewScene(...)
*     ����: ����ָ���ĳ�����С�½�һ������
************************************************/
void CEdit::NewScene( void )
{
    //>>>>>> ���ɳ������� >>>>>>>
    //���淽��Ĵ�СΪ32*32
    LONG hN = (LONG)(map.SceneH/32);
    LONG wN = (LONG)(map.SceneW/32);
    map.lGrnN = hN * wN;

    //Ϊ���淽�鿪�ٿռ�
	map.Grn =new LONG[map.lGrnN];
	ZeroMemory(map.Grn,sizeof(LONG)*(hN*wN));
	LONG tmp;
    for( LONG n=0;n<map.lGrnN;n++ )
    {
		tmp=random(3);
		if(GRNKind==1)       //�ݵ�
			tmp = tmp+3;		
		map.Grn[n]=tmp;
    }
	if(GRNKind==1)       //�ݵ�
		GRNKind=tmp;
    map.lObjectN = 0;
    //���η���Ĵ�СΪ16*16
    hN = (LONG)(map.SceneH/16);
    wN = (LONG)(map.SceneW/16);
    map.lSquareCount = hN * wN;
    map.Square = new WORD[hN * wN];
    ZeroMemory(map.Square,sizeof(WORD)*(hN*wN));

	//>>>>>> ��ʼ��CGetPath >>>>>>>>>
	//���õ��η����СΪ16*16
	GPa.Set_lSquareC(16);
	//���ó�����С
	GPa.Set_SceneW(map.SceneW);
	GPa.Set_SceneH(map.SceneH);
	//�ѵ������鴫�͸�CGetPath����
	GPa.Set_Square((VOID*)map.Square);

    lSaveState = 0;//��ǰ�������ݻ�δ��Ϊ�ļ�
    //����������
    lAppState = 1;
    lUpdateScreen = 1;
}

BOOL CEdit::GetSceneFileName( DWORD dwFlag )
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
/************************************************************
*   ������: SaveGroundDesc(...)
*     ����: ����ָ�������ĵ�������
*************************************************************/
void CEdit::SaveGroundDesc( FILE *file )
{
    if( file==NULL )
        return;
	fwrite( map.Grn, sizeof(LONG)*map.lGrnN,1,file);
}
/************************************************************
*   ������: LoadGroundDesc(...)
*     ����: ����ָ�������ĵ�������
*************************************************************/
void CEdit::LoadGroundDesc( FILE *file )
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
*   ������: LoadObject(...)
*     ����: ����ָ�������Ķ���
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
*   ������: SaveTerrainSquare(...)
*     ����: ����ָ�������ĵ�������
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::SaveTerrainSquare( FILE *file )
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
*   ������: SaveSceneData(...)
*     ����: ���泡������
*       (c) jk&ljq, 2010
*************************************************/
void CEdit::SaveSceneData( BOOL bSaveAs )
{
    //�Ǳ���Ϊ�����ǵ�һ�α�����ѡ���ļ���
    if( bSaveAs || lSaveState==0 )
    {
        if( !GetSceneFileName( 1 ) )
            return;
        if( map.SceneH==0 || map.SceneW==0 )
        {
            MessageBox( hWnd,
                TEXT("��û�н�����������\n")
                TEXT("���������κ�����"),
                TEXT("Warning!"),MB_OK );
            return;
        }
    }
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
/************************************************
*   ������: LoadSceneData(...)
*     ����: ����ָ������������
*       (c) jk&ljq, 2010
*************************************************/
void CEdit::LoadSceneData( void )
{
    //��ô��ļ�������
    if( !GetSceneFileName( 0 ) ){
        return;
	}
    //�ͷžɳ���������
    DeleteSceneData( map );
    //��ָ�������ļ�
    FILE *file = fopen(sceFilePath,"rb");
    //���볡���ṹ����
    fread(&map,sizeof(MAP),1,file);
	fread(&UI,sizeof(LONG),1,file);
    //������淽����������
    LoadGroundDesc( file );
    //�����������
    LoadObject( file );
    //������η�������
    LoadTerrainSquare( file );

    //�������빤��
    fclose(file);
    lSaveState = 1;//��ǰ���������Ѿ��ж�Ӧ���ļ�
    //����������
    lAppState = 1;
    lUpdateScreen = 1;//���º�̨���弰��Ļ
}


/**************************************************
*   ������: RestoreVariable(...)
*     ����: �ָ���Ҫ�ָ��ı���
*       (c) jk&ljq, 2010
***************************************************/
inline void CEdit::RestoreVariable( void )
{
    //�ָ�������ʾ��������
}

/**************************************************
*   ������: CheckScrPos(...)
*     ����: �����Ļλ��
***************************************************/
inline LONG CEdit::CheckScrPos( void )
{
    //������Ļλ��
    POINT oldScrPos = ScrPos;

    //����ڿͻ����߽������ƶ���Ļ
    if( KeyMouse[0].Pos.x<=0 )
        ScrPos.x -= 20;
    else if( KeyMouse[0].Pos.x>=ScreenW-8 )
        ScrPos.x += 20;
    if( KeyMouse[0].Pos.y<=-15 )
        ScrPos.y -= 20;
    else if( KeyMouse[0].Pos.y>=ScreenH-6 )
        ScrPos.y += 20;
    
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

/**************************************************
*   ������: GroundEdit(...)
*     ����: ����༭
*       (c) jk&ljq, 2010
***************************************************/
LONG CEdit::GroundEdit( void )
{
    //��û�н�������
    if( map.SceneH==0 || map.SceneH==0 )
        return 0;
    //ѡ��Ĳ������ǵ���༭
    if( (lEditState!=0)&&(lEditState!=1) )
        return 0;
    //û�а��²�����������
    if( !(KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0) )
        return 0;
    //�޸����ϴα��������
    if( lSaveState!=0 )
        lSaveState = 2;

    //>>>>>>>> ����굥�������ĸ���Ԫ >>>>>>>>>>
	LONG x = (LONG)( (MouseScePos.x)/32 );
	LONG y = (LONG)( (MouseScePos.y)/32 );
    LONG wN = map.SceneW/32;
    // ���ѡ���з���ĳ������
    LONG grn_Index = y*wN+x;
	map.Grn[grn_Index]=GRNKind;
	if(lEditState==1)//���µ�������
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
*   ������: DeleteObject(...)
*     ����: ɾ��ָ���Ķ���
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
				//>>>>> �����Ӧ���ϰ� >>>>>>>
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
				//>>>>> �����Ӧ���ϰ� >>>>>>>
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
			//>>>>> �����Ӧ���ϰ� >>>>>>>
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
*   ������: DeleteSelect(...)
*     ����: ɾ��ָ���Ķ���
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
*   ������: NewObject(...)
*     ����: �¼Ӷ���
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
			//>>>>> ���ɶ�Ӧ���ϰ� >>>>>>>
			SetRect(temp.Pos.x-w/2,temp.Pos.y-h/2,w,h,TRUE);
		}
		else if(lEditState==3){
			player[IorAI].People.push_back(temp);
			player[IorAI].PeopleCount=player[IorAI].People.size();
			player[IorAI].population +=temp1.population_cost;
			//>>>>> ���ɶ�Ӧ���ϰ� >>>>>>>
			map.Square[GPa.GetSquareFromPos(MouseScePos)] = 1;
		}
		else if(lEditState==4){
			Neutral_Unit.push_back(temp);
			//>>>>> ���ɶ�Ӧ���ϰ� >>>>>>>
			SetRect(temp.Pos.x-w/2,temp.Pos.y-h/2,w,h,TRUE);
		}
		
	}	
}
/**************************************************
*   ������: ObjectEdit(...)
*     ����: �༭����
*       (c) jk&ljq, 2010
***************************************************/
LONG CEdit::ObjectEdit( void )
{
    //��û�н�������
    if( map.SceneH==0 || map.SceneH==0 )
        return 0;
    //ѡ��Ĳ������ǵ���༭
    if( lEditState!=2 && lEditState!=3 && lEditState!=4)
        return 0;

    //>>>>>>> �༭���� >>>>>>>>>>
    //�Ҽ�ѡ�����
    if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON1 )
    {
		select.clear();
		SelectUnit temp1;
		Unit temp;
		LONG i,j;
		for(i=0;i<2;i++){//player
			for(j=0;j<player[i].Building.size();j++){//����
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
			for(j=0;j<player[i].People.size();j++){//ʿ��
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
		for(j=0;j<Neutral_Unit.size();j++){//����
				temp=Neutral_Unit[j];
				LONG w = Neutral_Common[temp.kind].eachWide/2;
				LONG h = Neutral_Common[temp.kind].eachHeight/2;
				if(MouseScePos.x>(temp.Pos.x-w) &&
					MouseScePos.x<(temp.Pos.x+w) &&
					MouseScePos.y>(temp.Pos.y-h) &&
					MouseScePos.y<(temp.Pos.y+h))
				{
					temp1.player=2;//����
					temp1.Bui_Sol=0;
					temp1.number=j;
					temp1.uniform_identifier=temp.uniform_identifier;
					select.push_back(temp1);
					break;
				}
			}
		return 1;
    }
    //������ɶ���
    else if( KeyMouse[0].Key==NULL &&
        KeyMouse[1].Key==DIMOFS_BUTTON0 )
    {
        NewObject();
        select.clear();
        //�޸����ϴα��������
        if( lSaveState!=0 )
            lSaveState = 2;
        return 1;
    }
    //"Delete"��ɾ����ѡ����
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
*   ������: DeleteSceneData(...)
*     ����: ɾ��ָ������
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

    //�ͷŵ�������ռ�
    delete[] map.Square;
}
/************************************************************
*   ������: MakeGround(...)
*     ����: ��ʾ����
*       (c) jk&ljq, 2010
*************************************************************/
void CEdit::MakeGround( void )
{
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
		rect = GetRscRect(GroundRC[map.Grn[n]],32,32,16);
		DDraw.BlitBack( x,y,lpDDSGroundRC,&rect,0);
    }
}
/************************************************
*   ������: MakeEntity(...)
*     ����: �����ϵĺ�������ʾ����ʾ
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
		for(j=0;j<player[i].Building.size();j++){//����
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

		for(j=0;j<player[i].People.size();j++){//ʿ��
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
	for(j=0;j<Neutral_Unit.size();j++){//����
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
	//select��ʾ
	for(i=0;i<select.size();i++){
		if(select[i].player ==2){//����
			temp=Neutral_Unit[select[i].number];
			temp1=Neutral_Common[temp.kind];
			w=temp1.eachWide;
			h=temp1.eachHeight;
		}
		else if(select[i].Bui_Sol==0){//����
			temp=player[select[i].player].Building[select[i].number];
			temp1=Building_Common[player[select[i].player].race][temp.kind];
			w=temp1.eachWide;
			h=temp1.eachHeight;
			}
		else{//ʿ��
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

    //>>>>> ������ʾ����ʾ >>>>>
    //��Ļ���Ͻ�λ�õ���ʾ
    Text.TextBackSurf(5,5,ScrPos.x,RGB(255,255,255),0);
    Text.TextBackSurf(5,25,ScrPos.y,RGB(255,255,255),0);
    //�������Ե���ʾ
    if( map.SceneH!=0 ) {
        Text.TextBackSurf(ScreenW-150,5,"������Ϊ:",RGB(128,128,255),0);
        Text.TextBackSurf(ScreenW-60,5,map.SceneW,RGB(255,128,255),0);
        Text.TextBackSurf(ScreenW-150,25,"������Ϊ:",RGB(128,128,255),0);
        Text.TextBackSurf(ScreenW-60,25,map.SceneH,RGB(255,128,255),0);
    }
    //ѡ���˱༭�����������ʾ
    if( (lEditState==0) && map.SceneH!=0 )
        Text.TextBackSurf(ScreenW/2-100,5, "����༭",RGB(255,255,0),0);
    //ѡ���˱༭������������ʾ
    else if( (lEditState==1) && map.SceneH!=0 )
    {
        Text.TextBackSurf(ScreenW/2-100,5,"��ľ�༭",RGB(255,255,0),0);
        Text.TextBackSurf(ScreenW/2-20,5,map.lObjectN,RGB(255,128,255),0);
    }
    //ѡ���˱༭С����������ʾ
    else if( (lEditState==4) && map.SceneH!=0 )
    {
        Text.TextBackSurf(ScreenW/2-100,5,"���༭",RGB(255,255,0),0);
        Text.TextBackSurf(ScreenW/2-20,5,map.lObjectN,RGB(255,128,255),0);
    }
    //�ļ�����ʾ
    Text.TextBackSurf(5,ScreenH-20,sceFilePath,RGB(255,255,0),0);
    //����״̬����ʾ
    if( (lSaveState==2||lSaveState==0) && map.SceneH!=0 )
        Text.TextBackSurf(5,ScreenH-40,"��δ����",RGB(255,255,0),0);
}

/********************************************
*   ������: EditProcess(...)
*     ����: �༭������
*       (c) jk&ljq, 2010
*********************************************/
BOOL CEdit::EditProcess( void )
{
    //����ʱ��
    Time = GetTickCount();
    //>>>>> ����������� >>>>>>>>
    if( lAppState == 1 ){
        //״̬������ԭ
        RestoreVariable();
        //�����Ļλ��
        lUpdateScreen += CheckScrPos();
        //�༭����
        lUpdateScreen += GroundEdit();
        //�༭����
        lUpdateScreen += ObjectEdit();
        //��������ʾ����̨����
        if( lUpdateScreen>0 )
        {
            MakeGround();
            //�����ϵĺ�������ʾ����ʾ
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
*   ������: GetInputDeviceData(...)
*     ����: ��������豸��״̬
*       (c) jk&ljq, 2010
****************************************************/
void CEdit::GetInputDeviceData( LPRECT lpClientRt )
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
    else if( KeyMouse[0].Pos.y < -15 )
        KeyMouse[0].Pos.y = -15;
    //�ڳ����е�λ��
    MouseScePos.x = KeyMouse[0].Pos.x+ScrPos.x;
    MouseScePos.y = KeyMouse[0].Pos.y+ScrPos.y;
}
/*******************************************************
* ������ : SetRect(...)
*   ���� : ���þ����ڵĵ�������
*   ���� : left  ��ͼλ��
*          top  ��ͼλ��
*          Width ͼƬ��
*          Height  ͼƬ��
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
	if((x1*16)==(right+1))//����
		x1++;
    y1 = (LONG)( bottom/16 );
	if((y1*16)==(bottom+1))//����
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
* ������ : CheckRect(...)
*   ���� : �������ڵĵ�������
*   ���� : left  ��ͼλ��
*          top  ��ͼλ��
*          Width ͼƬ��
*          Height  ͼƬ��
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
/************************************************************
*   ������: UpdateScreen(...)
*     ����: ������Ļ
*************************************************************/
void CEdit::UpdateScreen( LPRECT lpClientRect )
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
