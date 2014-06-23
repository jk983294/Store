/*-------------------------------------------
|              CGame.h
|          ��Ϸ���Ա����
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
    CDInput     Input;//�����豸����
    CDSound     Sound;//��������
    CGetPath    GPa;//·������
    CText       Text;//�ַ�������
	//�������ļ�������
	LONG GroundRC[100];
	//LPDIRECTDRAWSURFACE7 Building[2][5];//0:Hum 1:Orc	
	//LPDIRECTDRAWSURFACE7 People[2][5];//0:Hum 1:Orc
	LPDIRECTDRAWSURFACE7 Win_Lose[2][2];//0:Hum 1:Orc
    LPDIRECTDRAWSURFACE7 lpDDSGroundRC;//����Դ����	
	LPDIRECTDRAWSURFACE7 lpDDSIcons;//ͼ��Դ����	
    LPDIRECTDRAWSURFACE7 lpDDSSelect_Row;//ѡ���Դ����
	LPDIRECTDRAWSURFACE7 lpDDSSelect_Col;//ѡ������
	LPDIRECTDRAWSURFACE7 Neutral[2];//����
    LPDIRECTDRAWSURFACE7 Interface[8];//����
	LPDIRECTDRAWSURFACE7 MissileSource[4];//����
	Unit_Common Building_Common[2][5];//0:Hum 1:Orc
	Unit_Common People_Common[2][5];//0:Hum 1:Orc
	Unit_Common  Neutral_Common[2];//����
	Unit_Common  Interface_Common[8];//����
	Unit_Common  Missile_Common[4];//����
    countNum Count;//���������
	Player player[2];//0:�Լ�  1:AI
	vector<SelectUnit> select;//���ѡ�е�λ
	vector<SelectUnit> select_frame;//����Ͽ�ѡ�е�λ
	vector<SelectUnit> select_1;//���ѡ�е�λ
	vector<SelectUnit> select_2;//���ѡ�е�λ
	vector<SelectUnit> select_3;//���ѡ�е�λ
	vector<Unit> Neutral_Unit;//������λ
	vector<Missile> MissileQueue;//����
	vector<Unit> DeadQueue;//������λ

    LPDIRECTDRAWSURFACE7 lpDDSHeader;//��ͷ����
    LPDIRECTDRAWSURFACE7 lpDDSButtonUp;//��ť͹��
    LPDIRECTDRAWSURFACE7 lpDDSButtonDown;//��ť����
	LPDIRECTDRAWSURFACE7 lpDDSColor[2];//��ɫ 0:�� 1:��

    HWND        hWnd;//������
    BOOL        bFullScr;//��Ļģʽ
    LONG        ScreenW;//�ͻ����Ŀ��
    LONG        ScreenH;//�ͻ����ĸ߶�
    LONG        ColorBits;//ͼ����ɫ���

    LONG        MaxWObject;//�������ͼƬ�������
    LONG        MaxHObject;//�������ͼƬ�����߶�

	MAP			map;//��ͼ
    POINT       ScrPos;//��Ļ��λ��
    LONG        lAppState;//Ӧ�ó���״̬
    DWORD       Time;//��ǰʱ��
	DWORD       oldTime;//���ʱ��
	DWORD       game_start_time;//��Ϸ��ʼʱ��
	//�Ƿ���Ҫ���º�̨����Ļ����
    LONG        lUpdateScreen;
    LONG        lSaveState;//����״̬
    
    LONG        GRNKind;//ѡ��ĵ�������
    LONG        oldGRNKind;//�ɵ�������

	//0:HUM 1:ORC
	LONG		IorAI;//0:I 1:AI
	LONG		kind_building;//��������
	LONG		kind_people;//������
	LONG		kind_neutral;//��������
	LONG		UI;//ͳһ��ʶ��
	LONG		flag;

	TCHAR       sceFilePath[MAX_PATH];//�����ļ���

    PKEY        KeyKB;//����״ָ̬��
    PKEY        KeyMouse;//���״ָ̬��(��Ļ�е�)
    POINT       MouseScePos;//���ת���������е�λ��

	POINT		start_select;
	POINT		end_select;
	BOOL		select_draw;
	Circle_Draw circle_draw;


    
public:
    CGame(void);
    ~CGame();

    //>>>>>>> ���ó�Ա���� >>>>>>>>>>>

    void Set_hWnd( HWND hwnd ) {hWnd = hwnd;}
    void Set_bFullScr( BOOL bFullScreen )
    {//������Ļģʽ
        bFullScr = bFullScreen;
    }
    //���ÿͻ������
    void Set_ScreenW( LONG ScrW ) {ScreenW = ScrW;}
    //���ÿͻ����߶�
    void Set_ScreenH( LONG ScrH ) {ScreenH = ScrH;}
    void Set_ColorBits( LONG ColorBitCount )
    {//������ɫ���
        ColorBits = ColorBitCount;
    }
    void Set_MaxWObject( LONG MaxWideObject )
    {//���ö���ͼƬ�������
        MaxWObject = MaxWideObject;
    }
	void Initial_count(void)
	{//��ʼ��������
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
    {//���ö���ͼƬ�����߶�
        MaxHObject = MaxHeightObject;
    }

    //>>>>>>> ������Ա���� >>>>>>>>>>>
    //��ȡһ������
    LONG DoFor( LONG lIndex,
                LONG lEndNum,
                LONG lStartNum,LONG Direct);
    INT  GetDirection( POINT p1, POINT p2 );
    RECT GetActionRect( LONG lIndex,
                        LONG lWidth,
                        LONG lHeight,
                        LONG lCount);

    //>>>> ��ʼ��DirectX��Ա���� >>>>>>>
    //������Ϸ��ͼ������
    void LoadImageSurf( void );
    //��ʼ��DirectX(�ⲿͨ������������ʼ����Ϸ)
    void InitDirectX( HWND _hWnd );

    //>>>>> ��Ϸ��ͷ������ش����� >>>>>>
    //����ť
    void DrawButton( LPDIRECTDRAWSURFACE7 Surf,
                     WORD state, LPRECT lpRect );
    //��Ϸ��ͷ����
    void Header( void );

	//>>>>>> ����༭��س�Ա >>>>>>>
    //ɾ������
    //void DeleteObject( LONG uniform_temp );
	void DeleteSelect(LONG uniform_temp);
    //�¼Ӷ���
    void NewObject( void );
    //�༭����
    LONG ObjectEdit( void );

    void MakeScene( void );
    //ɾ��ָ������������
    void DeleteSceneData( MAP map );
	//>>>>>>> �ļ����������س�Ա >>>>>>>
	void SaveGroundDesc( FILE *file );
    void LoadGroundDesc( FILE *file );
    void SaveObject( FILE *file );
    void LoadObject( FILE *file);
    void SaveTerrainSquare( FILE *file);
    void LoadTerrainSquare( FILE *file);
    void SaveSceneData( BOOL bSaveAs );
    void LoadSceneData( void );
	BOOL GetSceneFileName( DWORD dwFlag );
    //>>>>>> �ָ���Ҫ�ָ��ĳ�Ա����>>>>>
    void RestoreVariable( void );
	//�������λ�øı���Ļ��λ��
    LONG CheckScrPos( void );



    //>>>>>> ���ɵ��� >>>>>>>>>>
    void MakeGround( void );
    //>>>>>> ������ʾ����ĸ��� >>>>>>>>>
    //>>> ��ɫ������������͹���ѡ��Ȧ����ʾ>>>
    void MakeEntity( void );
    //>>> ��ɫ����Ĵ��� >>>>>>>
    void DeadProcess( void );
	void CheckMessage( void );
	void CheckAIMessage( void );
	void MessageProcess( void );

    //>>>> ��Ϸ�Ľ���(�ⲿͨ����������������Ϸ)>>>
    void GameProcess( void );
    //>>>> ��ȡ�����豸״̬���� >>>>>
    void GetInputDeviceData( LPRECT lpClientRect );
    //>>>> ������Ļ��Ա���� >>>>>>>>
    void UpdateScreen( LPRECT lpClientRect );
	//���õ��η���
	void SetRect( LONG left, LONG top,LONG Width, LONG Height,BOOL data  );
	//�����η���
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