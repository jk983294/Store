/*-------------------------------------------
|              CEdit.h
|          �༭���Ա����
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
    CDInput     Input;//�����豸����
	CGetPath    GPa;//·������
    CText       Text;//�ַ�������
	//�������ļ�������
	LONG GroundRC[100];
	//LPDIRECTDRAWSURFACE7 Building[2][10];//0:Hum 1:Orc
	//LPDIRECTDRAWSURFACE7 People[2][10];//0:Hum 1:Orc	
    LPDIRECTDRAWSURFACE7 lpDDSGroundRC;//����Դ����	
    LPDIRECTDRAWSURFACE7 lpDDSSelect_Row;//ѡ���Դ����
	LPDIRECTDRAWSURFACE7 lpDDSSelect_Col;//ѡ������
	LPDIRECTDRAWSURFACE7 Neutral[10];//����
	Unit_Common People_Common[2][10];//0:Hum 1:Orc
	Unit_Common Building_Common[2][10];//0:Hum 1:Orc
	Unit_Common  Neutral_Common[10];//����
	Player player[2];//0:�Լ�  1:AI
	vector<Unit> Neutral_Unit;//������λ
	vector<SelectUnit> select;//���ѡ�е�λ	

    HWND        hWnd;//������
    BOOL        bFullScr;//��Ļģʽ
    LONG        ScreenW;//�ͻ����Ŀ��
    LONG        ScreenH;//�ͻ����ĸ߶�
    LONG        ColorBits;//ͼ����ɫ���

    LONG        MaxWObject;//�������ͼƬ�������
    LONG        MaxHObject;//�������ͼƬ�����߶�

    MAP			map;//��ͼ
    POINT       ScrPos;//��Ļ��λ��
    LONG        lAppState;//����״̬
    //�Ƿ���Ҫ���º�̨����Ļ����
    LONG        lUpdateScreen;
    LONG        lSaveState;//����״̬
    LONG        lEditState;//�༭״̬
    DWORD       Time;//��ǰʱ��
    LONG        GRNKind;//ѡ��ĵ�������
    LONG        oldGRNKind;//�ɵ�������
	//0:HUM 1:ORC
	LONG		IorAI;//0:I 1:AI
	LONG		kind_building;//��������
	LONG		kind_people;//������
	LONG		kind_neutral;//��������
	LONG		UI;//ͳһ��ʶ��

    TCHAR       sceFilePath[MAX_PATH];//�����ļ���

    PKEY        KeyKB;//����״ָ̬��
    PKEY        KeyMouse;//���״ָ̬��(��Ļ�е�)
    POINT       MouseScePos;//���ת���������е�λ��
	

public:
    CEdit(void);
    ~CEdit();
	
	//Player AI;

    //>>>>>>> ���ó�Ա���� >>>>>>>>>>>
    //����Ӧ�ó�����
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
    void Set_MaxHObject( LONG MaxHeightObject )
    {//���ö���ͼƬ�����߶�
        MaxHObject = MaxHeightObject;
    }
    //���ó������
    void Set_SceneW( LONG SceW )
    {
        //����������Ϊ80��������
        SceW = (SceW/32)*32;
        map.SceneW = SceW;
    }
    //���ó����߶�
    void Set_SceneH( LONG SceH )
    {
        //����������Ϊ80��������
        SceH = (SceH/32)*32;
        map.SceneH = SceH;
    }
    //����ѡ��ĵ�������
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
    //����ѡ��ĵ�������
    void Set_lEditState( LONG EditState )
    {
        lEditState = EditState;
        lUpdateScreen = 1;
    }

    //>>>>>>> ������Ա���� >>>>>>>>>>>
    //��ȡһ������
    RECT GetRect(LONG left,LONG top,
                 LONG right,LONG bottom);

    //>>>> ��ʼ��DirectX��Ա���� >>>>>>>
    //������Ϸ��ͼ������
    void LoadImageSurf( void );
    //��ʼ��DirectX(�ⲿͨ������������ʼ����Ϸ)
    void InitDirectX( HWND _hWnd );

    void NewScene( void );
    void DeleteSceneData( MAP map );
    //>>>>>>> �ļ����������س�Ա >>>>>>>
    BOOL GetSceneFileName( DWORD dwFlag );
    void SaveGroundDesc( FILE *file );
    void LoadGroundDesc( FILE *file );
    void SaveObject( FILE *file );
    void LoadObject( FILE *file);
    void SaveTerrainSquare( FILE *file);
    void LoadTerrainSquare( FILE *file);
    void SaveSceneData( BOOL bSaveAs );
    void LoadSceneData( void );

    //>>>>>> �ָ���Ҫ�ָ��ĳ�Ա����>>>>>
    void RestoreVariable( void );

    //�������λ�øı���Ļ��λ��
    LONG CheckScrPos( void );
    //>>>>>> ����ر༭��س�Ա >>>>>>>>
    //����ָ������������ȡ��Ӧ���淽���Դλ��
    //RECT GetSrcRect( GROUNDDESC *grn,BOOL bGetNew);
    //����ָ���ĵ��淽����Ż�ö�Ӧ���淽��ָ��
    //GROUNDDESC * GetGrnPointer( LONG lN );
    //����ر༭
    LONG GroundEdit( void );
	//���õ��η���
	void SetRect( LONG left, LONG top,LONG Width, LONG Height,WORD data  );
	//�����η���
	BOOL CheckRect( LONG left, LONG top,LONG Width, LONG Height,WORD data );

    //>>>>>> ����༭��س�Ա >>>>>>>
    //ɾ������
    void DeleteObject( LONG uniform_temp );
	void DeleteSelect(LONG uniform_temp);
    //�¼Ӷ���
    void NewObject( void );
    //�༭����
    LONG ObjectEdit( void );

    //>>>>>> ���ɵ��� >>>>>>>>>>
    void MakeGround( void );
    //>>>>>>> �����ϵ��Լ����ֵ���ʾ >>>>>>>
    void MakeEntity( void );

    //>>>>>> �༭���Ľ��� >>>>>>>>
    BOOL EditProcess( void );
    //>>>> ��ȡ�����豸״̬���� >>>>>
    void GetInputDeviceData( LPRECT lpClientRect );
    //>>>> ������Ļ��Ա���� >>>>>>>>
    void UpdateScreen( LPRECT lpClientRect );

    //>>>> ��ñ���״̬ >>>>>>>
    LONG Get_lSaveState( void ){return lSaveState;}

};