#include <ddraw.h>
#include "CGetPath.h"
#include<vector>
using namespace std;
typedef enum _unit_action_ {
	UnitActionNone,         /// No valid action

	UnitActionStill,        /// unit stand still, does nothing
	UnitActionFollow,       /// unit follows units
	UnitActionAttackFollow,       /// unit attacks follow position/unit
	UnitActionMove,         /// unit moves to position/unit
	UnitActionAttack,       /// unit attacks position/unit
	UnitActionDie,          /// unit dies

	UnitActionSpellCast,    /// unit casts spell

	UnitActionTrain,        /// building is training
	UnitActionUpgradeTo,    /// building is upgrading itself
	UnitActionResearch,     /// building is researching spell
	UnitActionBuilt,      /// building is under construction

	UnitActionBoard,        /// unit entering transporter
	UnitActionUnload,       /// unit leaving transporter
	UnitActionPatrol,       /// unit paroling area
	UnitActionBuild,        /// unit builds building

	UnitActionRepair,       /// unit repairing
	UnitActionResource,     /// unit harvesting resources
	UnitActionReturnGoods,  /// unit returning any resource
	UnitActionTransformInto /// unit transform into type.
} UnitAction;

typedef struct SelectUnit {
    LONG        player;//0:�Լ� 1:AI 2:����
    LONG        Bui_Sol;//0:���� 1:ʿ��
    LONG        number;//�ڼ���
	LONG		uniform_identifier;//ͳһ��ʶ��
	LONG		kind;//����
}SelectUnit;

typedef struct Unit_Common {
	LPDIRECTDRAWSURFACE7 lpSurf;//ͼ��Դ����
    LPDIRECTDRAWSURFACE7 lpIcon;//Сͼ��Դ����
	LONG    lSrcWCount;//Դͼ��ÿһ�е�ͼƬ��
	LONG	lSrcHCount;//Դͼ��ÿһ�е�ͼƬ��
	LONG    Wide;//ԴͼƬ�Ŀ��
	LONG    Height;//ԴͼƬ�ĸ߶�
	LONG    eachWide;//ÿ��ͼƬ�Ŀ��
	LONG    eachHeight;//ÿ��ͼƬ�ĸ߶�
    LONG	start_run;//���߿�ʼͼƬ����
	LONG	end_run;//���߽���ͼƬ����
	LONG	start_attack;//������ʼͼƬ����
	LONG	end_attack;//��������ͼƬ����
	LONG	start_die;//������ʼͼƬ����
	LONG	end_die;//��������ͼƬ����
	LONG    sound[5];//����
	LONG	Hp;//��Ѫ��
	LONG	range;//������Χ
	LONG	attackpower;//������
	LONG	attack_kind;//�������� 0:��ͨ 1:���� 2:ħ�� 3:����
	LONG	attackspeed;//�����ٶ�
	LONG	movespeed;//�ƶ��ٶ�
	LONG	defense;//������
	LONG	defense_kind;//�������� 0:��� 1:�м� 2:�ؼ� 3:��ǿ�ͳǼ�
	LONG	sight;//��Ұ	
	LONG	goldspeed;//�ɽ��ٶ�
	POINT	pos;//λ��  ������
	LONG	missile_kind; //��������
	LONG	abletoproduct; //�����������ı�������
	LONG	train_time;//ѵ��ʱ��
	LONG	count;//����λ����
	LONG	population_cost;//���˿���
	LONG	gold_cost;//�Ļƽ���
} Unit_Common;
typedef struct Unit {
    LONG kind;//����
	UnitAction state;//״̬ 
	LONG HP;//��ǰѪ��
	POINT Pos;//�����е�λ��
	
	BOOL    bDead;//�Ƿ�����

	POINT	Target;//�ɵ�λ��
	SelectUnit Target_Select;
    LPPATH  lpPath;//·��
    LPPATH  lpStep;//·��

	LONG    Direct;//����
	LONG    oldDirect;//����
	LONG    lStartIndex;//��ǰ��������Ŀ�ʼ������
	LONG    lEndIndex;//��ǰ��������Ŀ�ʼ������
	LONG	pic_Index;//ԴͼƬλ��

	DWORD   newTime;//ʱ��
	DWORD   startTime;//ʱ��

	SelectUnit	Producting;
	LONG		uniform_identifier;//ͳһ��ʶ��	
} Unit;
struct Player {
public:
    LONG race;//����
	LONG gold;//�ƽ�
	LONG population;//�˿�
	POINT ArmyPos;//����λ��
	LONG BuildingCount;//������
	LONG PeopleCount;//�˿���
	LONG state;//���״̬   0:��չ 1:����
	LONG count[5];//�����ּ���
	LONG strategy[6];//����
	vector<Unit> Building;//��������
	vector<Unit> People;//ʿ������	
};
typedef struct MAP {
    LONG        SceneW;//�������
    LONG        SceneH;//�����߶�
    LONG        lObjectN;//����������
    LONG        lGrnN;//���淽�����
    LONG        lSquareCount;//���η������

    LPLONG  Grn;//��������
    LPWORD      Square;//�������������׵�ַ
}MAP;
typedef struct missile{
	LONG kind;//����
	SelectUnit select;//��������
	POINT pos;//��ǰλ��
	POINT target_pos;//Ŀ��λ��
	LONG	pic_Index;//ԴͼƬλ��
	LONG	oldDirect;//ԴͼƬλ��
	DWORD   startTime;//ʱ��
	LONG	attackpower;//������
	LONG	attack_kind;//�������� 0:��ͨ 1:���� 2:ħ�� 3:����
}Missile;
typedef struct countNum{
	LONG playerSolProduce[5];
	LONG playerSolLost[5];
	LONG playerBuiLost[5];
	LONG AISolProduce[5];
	LONG AISolLost[5];
	LONG AIBuiLost[5];
}countNum;
typedef struct Circle_Draw{
	LPDIRECTDRAWSURFACE7 lpCircle[2];//0:green 1:red
	DWORD   startTime;//ʱ��
	LONG flag_color;//0:green 1:red
	BOOL flag;//0:����ʾ 1:��ʾ
	POINT pos;
}Circle_Draw;