/*-------------------------------------------
|              CGetPath.h
|      A-StarѰ·�㷨��ش�����
|           ��Ȩ����  jk&ljq
---------------------------------------------*/

#include <windows.h>

//·���ṹ����
typedef struct PATH {
    POINT  Pos;
    PATH * before;
    PATH * next;
} PATH, * LPPATH;

class   CGetPath
{
protected:
    LONG    lSquareC;//���ӵĿ��
    LONG    lHarfSquC;//���ӿ�ȵ�һ��
    LPWORD  Square;//�������������׵�ַ
    LONG    lMaxStep;//��̽�������
    LONG    lCount;//��ǰ�����Ĳ���
    LONG    SceneW;//�����Ŀ��(���ص�λ)
    LONG    SceSquareW;//�����Ŀ��(���鵥λ)
    LONG    SceneH;//�����ĸ߶�(���ص�λ)
    LONG    SceSquareH;//�����ĸ߶�(���鵥λ)
    RECT    MonsterRect;//���Ｄ�����Ĵ�С

public:
    //���캯��
    CGetPath( LONG SquareC, LPVOID lpSquare,
              DWORD SceW, DWORD SceH,
              RECT MonsterRt );
    CGetPath( void );

    //��������
    void Set_lSquareC( LONG SquareC )
    {
        lSquareC=SquareC;
        lHarfSquC=lSquareC/2;
        LONG w=MonsterRect.right-MonsterRect.left;
        LONG h=MonsterRect.bottom-MonsterRect.top;
        lMaxStep = (w/lSquareC)*(h/lSquareC);
        SceSquareW=SceneW/lSquareC;
        SceSquareH=SceneH/lSquareC;
    }
    void Set_Square( LPVOID lpSquare )
    {
        Square=(LPWORD)lpSquare;
    }
    void Set_SceneW( LONG SceW )
    {
        SceneW=SceW;
        SceSquareW=SceneW/lSquareC;
    }
    void Set_SceneH( LONG SceH )
    {
        SceneH=SceH;
        SceSquareH=SceneH/lSquareC;
    }
    void Set_MonsterRect(RECT rect)
    {
        MonsterRect=rect;
        LONG w=MonsterRect.right-MonsterRect.left;
        LONG h=MonsterRect.bottom-MonsterRect.top;
        lMaxStep = (w/lSquareC)*(h/lSquareC);
    }

    //���ܺ���
    LONG  MakeSquare(LONG y, LONG x);
    LONG  GetSquareFromPos( POINT );
    POINT GetPosFromSquare( LONG );
    LONG  Distance( POINT StartPos,
                    POINT DestPos );
    LONG  Distance( LONG StartSquare,
                    LONG DestSquare );
    POINT AdjustPos( POINT Pos );
    LPLONG GetAroundSquare( POINT Pos );
    LONG  GetNearSquare( POINT Pos,
                         POINT DestPos );
    BOOL  Movable( POINT StartPos, POINT DestPos,
                   LPPATH &lpPath, BOOL bFlags );
    LPPATH GetLastMember( LPPATH lpPath );
    BOOL  IsWalkableSquare( LONG lSquare );
    LONG  DoFor( LONG lIndex, LONG lFlags );
    POINT GetNextPos( POINT p1,
                      LPLONG lpRelateSqu,
                      LONG lFlags );
    BOOL IsOldPos( POINT NextPos, LPPATH lpPath );
    void DeletePath( LPPATH lpPath );
    BOOL ArcSearch( POINT StartPos, POINT DestPos,
                    LPPATH &lpPath,
                    LPPATH lpOldPath,
                    LONG lFlags );
    BOOL RoundSearch( POINT StartPos,POINT DestPos,
                      LPPATH &lpPath,LONG lFlags );
    LPPATH MethodOne( POINT p1, POINT DestPos,
                      LPLONG lplGet,
                      LONG lFlags );
    LPPATH MethodTwo( POINT p1, POINT DestPos,
                      LPLONG lplGet,
                      LONG lFlags );
    LPPATH MethodThree( POINT p1, POINT DestPos,
                        LPPATH lpPath1,
                        LPLONG lplGet,
                        LONG lFlags );
    BOOL GetBasicPath( POINT p1, POINT DestPos,
                       LPPATH &lpPath,
                       LPLONG bGet,
                       LONG lFlags );
    LONG GetPathCount( LPPATH lpPath );
    LPPATH GetTheBestPath( POINT StartPos,
                           POINT DestPos,
                           LONG lFlags );

    //�ⲿ���ó�Ա����
    LONG  Get_lSquareC( void ) {return lSquareC;}
    LONG  Get_lHarfSquC( void ){return lHarfSquC;}
    LPWORD Get_Square( void )  {return Square;}
    LONG  Get_lMaxStep( void ) {return lMaxStep;}
    LONG  Get_lCount( void )   {return lCount;}
    LONG  Get_SceneW( void )   {return SceneW;}
    LONG  Get_SceneH( void )   {return SceneH;}
    LONG  Get_SceSquareW(void) {return SceSquareW;}
    LONG  Get_SceSquareH(void) {return SceSquareH;}
    RECT  Get_MonsterRect(void){return MonsterRect;}
	BOOL check_eight_direction(POINT a);
};

