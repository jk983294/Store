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
    LONG        player;//0:自己 1:AI 2:中立
    LONG        Bui_Sol;//0:建筑 1:士兵
    LONG        number;//第几个
	LONG		uniform_identifier;//统一标识符
	LONG		kind;//类型
}SelectUnit;

typedef struct Unit_Common {
	LPDIRECTDRAWSURFACE7 lpSurf;//图标源表面
    LPDIRECTDRAWSURFACE7 lpIcon;//小图标源表面
	LONG    lSrcWCount;//源图像每一行的图片数
	LONG	lSrcHCount;//源图像每一列的图片数
	LONG    Wide;//源图片的宽度
	LONG    Height;//源图片的高度
	LONG    eachWide;//每个图片的宽度
	LONG    eachHeight;//每个图片的高度
    LONG	start_run;//行走开始图片号码
	LONG	end_run;//行走结束图片号码
	LONG	start_attack;//攻击开始图片号码
	LONG	end_attack;//攻击结束图片号码
	LONG	start_die;//死亡开始图片号码
	LONG	end_die;//死亡结束图片号码
	LONG    sound[5];//声音
	LONG	Hp;//总血量
	LONG	range;//攻击范围
	LONG	attackpower;//攻击力
	LONG	attack_kind;//攻击类型 0:普通 1:穿刺 2:魔法 3:混乱
	LONG	attackspeed;//攻击速度
	LONG	movespeed;//移动速度
	LONG	defense;//防御力
	LONG	defense_kind;//防御类型 0:轻甲 1:中甲 2:重甲 3:加强型城甲
	LONG	sight;//视野	
	LONG	goldspeed;//采金速度
	POINT	pos;//位置  出生点
	LONG	missile_kind; //导弹类型
	LONG	abletoproduct; //建筑能生产的兵种类型
	LONG	train_time;//训练时间
	LONG	count;//本单位计数
	LONG	population_cost;//耗人口数
	LONG	gold_cost;//耗黄金数
} Unit_Common;
typedef struct Unit {
    LONG kind;//类型
	UnitAction state;//状态 
	LONG HP;//当前血量
	POINT Pos;//场景中的位置
	
	BOOL    bDead;//是否死了

	POINT	Target;//旧的位置
	SelectUnit Target_Select;
    LPPATH  lpPath;//路径
    LPPATH  lpStep;//路径

	LONG    Direct;//方向
	LONG    oldDirect;//方向
	LONG    lStartIndex;//当前动作种类的开始动作号
	LONG    lEndIndex;//当前动作种类的开始动作号
	LONG	pic_Index;//源图片位置

	DWORD   newTime;//时间
	DWORD   startTime;//时间

	SelectUnit	Producting;
	LONG		uniform_identifier;//统一标识符	
} Unit;
struct Player {
public:
    LONG race;//种族
	LONG gold;//黄金
	LONG population;//人口
	POINT ArmyPos;//敌人位置
	LONG BuildingCount;//建筑数
	LONG PeopleCount;//人口数
	LONG state;//玩家状态   0:发展 1:攻击
	LONG count[5];//各兵种计数
	LONG strategy[6];//策略
	vector<Unit> Building;//建筑数组
	vector<Unit> People;//士兵数组	
};
typedef struct MAP {
    LONG        SceneW;//场景宽度
    LONG        SceneH;//场景高度
    LONG        lObjectN;//地面对象个数
    LONG        lGrnN;//地面方块个数
    LONG        lSquareCount;//地形方块个数

    LPLONG  Grn;//地面数组
    LPWORD      Square;//场景地形数组首地址
}MAP;
typedef struct missile{
	LONG kind;//类型
	SelectUnit select;//攻击对象
	POINT pos;//当前位置
	POINT target_pos;//目标位置
	LONG	pic_Index;//源图片位置
	LONG	oldDirect;//源图片位置
	DWORD   startTime;//时间
	LONG	attackpower;//攻击力
	LONG	attack_kind;//攻击类型 0:普通 1:穿刺 2:魔法 3:混乱
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
	DWORD   startTime;//时间
	LONG flag_color;//0:green 1:red
	BOOL flag;//0:不显示 1:显示
	POINT pos;
}Circle_Draw;