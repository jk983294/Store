/*-------------------------------------------
|              CGetPath.cpp
|   A-StarѰ·�㷨��ش����ܾ������
|           ��Ȩ����  jk&ljq
---------------------------------------------*/

#include <windows.h>
#include <math.h>
#include "CGetPath.h"

#define  SafeDeletePath(p) {DeletePath(p);p=NULL;}

/************************************************************
*   ������: CGetPath(...) ����CGetPath��ĳ�Ա
*     ����: ���캯��
*       (c) jk&ljq, 2010
*************************************************************/
CGetPath::CGetPath( LONG SquareC, LPVOID lpSquare,
                    DWORD SceW, DWORD SceH,
                    RECT MonsterRt )
{
    lSquareC = SquareC;//��ø��ӵĿ��
    lHarfSquC = lSquareC/2;
    //��ó������������׵�ַ
    Square   = (LPWORD)lpSquare;
    lCount =0;
    SceneW = SceW;
    SceneH = SceH;
    SceSquareW = SceneW/lSquareC;
    SceSquareH = SceneH/lSquareC;
    MonsterRect = MonsterRt;

    //���ݹ��Ｄ�����Ĵ�С��������������
    LONG w=MonsterRect.right-MonsterRect.left;
    LONG h=MonsterRect.bottom-MonsterRect.top;
    lMaxStep = (w/lSquareC)*(h/lSquareC);
}

/************************************************************
*   ������: CGetPath(...) ����CGetPath��ĳ�Ա
*     ����: ���캯������
*       (c) jk&ljq, 2010
*************************************************************/
CGetPath::CGetPath( void )
{
    lSquareC = 30;//��ø��ӵĿ��
    lHarfSquC = lSquareC/2;

    //�������������׵�ַ��ΪNULL,�Ժ��
    //��ͨ������Set_Square��Ա��õ�ַ
    Square   = NULL;

    lCount =0;
    SceneW = 1024;//Ĭ�ϵĳ������
    SceneH = 768;//Ĭ�ϵĳ����߶�
    SceSquareW = SceneW/lSquareC;
    SceSquareH = SceneH/lSquareC;

    //Ĭ�ϵĹ��Ｄ�����Ĵ�СΪ1000*800
    MonsterRect.left  =0;
    MonsterRect.right =1000;
    MonsterRect.top   =0;
    MonsterRect.bottom=800;

    //���ݹ��Ｄ�����Ĵ�С��������������
    LONG w=MonsterRect.right-MonsterRect.left;
    LONG h=MonsterRect.bottom-MonsterRect.top;
    lMaxStep = (w/lSquareC)*(h/lSquareC);
}

/*******************************************************
*  ������: MakeSquare(...) ����CGetPath��ĳ�Ա
*   ���� : ����ά��ʽ�����ķ���
*            (c)  jk&ljq, 2010
********************************************************/
inline LONG CGetPath::MakeSquare(LONG y, LONG x)
{
    if( y>=SceSquareH || y<0 )
        return -1;
    if( x>=SceSquareW || x<0 )
        return -1;
    return y*SceSquareW+x;
}

/************************************************************
*   ������: GetSquareFromPos(...) ����CGetPath��ĳ�Ա
*     ����: ���ָ��λ������Ӧ�ķ���
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG  CGetPath::GetSquareFromPos( POINT Pos )
{
    return MakeSquare( Pos.y / lSquareC,
                       Pos.x / lSquareC);
}

/************************************************************
*   ������: GetPosFromSquare(...) ����CGetPath��ĳ�Ա
*     ����: ���ָ�����������λ��
*       (c) jk&ljq, 2010
*************************************************************/
inline POINT CGetPath::GetPosFromSquare( LONG lSqu )
{
    LONG y = lSqu/SceSquareW;
    LONG x = lSqu-y*SceSquareW;

    POINT pos;
    pos.x = lHarfSquC +x*lSquareC;
    pos.y = lHarfSquC +y*lSquareC;

    return pos;
}

/************************************************************
*   ������: Distance(...) ����CGetPath��ĳ�Ա
*     ����: ������λ�ü�ľ���
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG  CGetPath::Distance(POINT StartPos,
                                POINT DestPos )
{
    LONG dx=StartPos.x-DestPos.x;
    LONG dy=StartPos.y-DestPos.y;
    return (LONG)sqrtf((FLOAT)(dx*dx+dy*dy));
}
/************************************************************
*   ������: Distance(...) ����CGetPath��ĳ�Ա
*     ����: ���أ������������ľ���
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG  CGetPath::Distance(LONG StartSquare,
                                LONG DestSquare)
{
    POINT StartPos=GetPosFromSquare(StartSquare);
    POINT DestPos=GetPosFromSquare(DestSquare);
    return Distance(StartPos,DestPos);
}

/*******************************************************
*  ������: AdjustPos(...) ����CGetPath��ĳ�Ա
*   ���� : ����ָ��λ�ã�ʹ�䴦�����ڷ��������
*            (c)  jk&ljq, 2010
********************************************************/
inline POINT CGetPath::AdjustPos( POINT P )
{
    return GetPosFromSquare(GetSquareFromPos(P));
}

/************************************************************
* ������: GetAroundSquare(...) ����CGetPath��ĳ�Ա
*   ����: ���ָ��λ�õ����ܷ���,�䷵��ֵ�ǰ˸�
*         Ԫ�ص������׵�ַ��
*       (c) jk&ljq, 2010
*************************************************************/
inline LPLONG CGetPath::GetAroundSquare( POINT Pos )
{
    static LONG lpAroSqu[8];

    LONG lSquare = GetSquareFromPos(Pos);
    //������ά��ʽ���ķ����к�
    LONG Index = lSquare/SceSquareW;
    //������ά��ʽ���ķ����к�
    LONG leave = lSquare - Index*SceSquareW;

    //�������Ͻǵķ���
    if( lSquare==0 )
    {
        lpAroSqu[0]=-1;
        lpAroSqu[1]=-1;
        lpAroSqu[2]=-1;
        lpAroSqu[3]=1;
        lpAroSqu[4]=SceSquareW+1;
        lpAroSqu[5]=SceSquareW;
        lpAroSqu[6]=-1;
        lpAroSqu[7]=-1;
    }
    //�������Ͻǵķ���
    else if( lSquare==SceSquareW-1 )
    {
        lpAroSqu[0]=-1;
        lpAroSqu[1]=-1;
        lpAroSqu[2]=-1;
        lpAroSqu[3]=-1;
        lpAroSqu[4]=-1;
        lpAroSqu[5]=lSquare+SceSquareW;
        lpAroSqu[6]=lSquare+SceSquareW-1;
        lpAroSqu[7]=lSquare-1;
    }
    //�������½ǵķ���
    else if( lSquare==SceSquareH*SceSquareW-1 )
    {
        lpAroSqu[0]=lSquare-SceSquareW-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=-1;
        lpAroSqu[3]=-1;
        lpAroSqu[4]=-1;
        lpAroSqu[5]=-1;
        lpAroSqu[6]=-1;
        lpAroSqu[7]=lSquare-1;
    }
    //�������½ǵķ���
    else if( Index==SceSquareH-1 && leave==0 )
    {
        lpAroSqu[0]=-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=lSquare-SceSquareW+1;
        lpAroSqu[3]=lSquare+1;
        lpAroSqu[4]=-1;
        lpAroSqu[5]=-1;
        lpAroSqu[6]=-1;
        lpAroSqu[7]=-1;
    }
    //������߽��ϵķ���
    else if( leave==0 )
    {
        lpAroSqu[0]=-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=lSquare-SceSquareW+1;
        lpAroSqu[3]=lSquare+1;
        lpAroSqu[4]=lSquare+SceSquareW+1;
        lpAroSqu[5]=lSquare+SceSquareW;
        lpAroSqu[6]=-1;
        lpAroSqu[7]=-1;
    }
    //�����ϱ߽��ϵķ���
    else if( Index==0 )
    {
        lpAroSqu[0]=-1;
        lpAroSqu[1]=-1;
        lpAroSqu[2]=-1;
        lpAroSqu[3]=lSquare+1;
        lpAroSqu[4]=lSquare+SceSquareW+1;
        lpAroSqu[5]=lSquare+SceSquareW;
        lpAroSqu[6]=lSquare+SceSquareW-1;
        lpAroSqu[7]=lSquare-1;
    }
    //�����ױ߽��ϵķ���
    else if( Index==SceSquareH-1 )
    {
        lpAroSqu[0]=lSquare-SceSquareW-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=lSquare-SceSquareW+1;
        lpAroSqu[3]=lSquare+1;
        lpAroSqu[4]=-1;
        lpAroSqu[5]=-1;
        lpAroSqu[6]=-1;
        lpAroSqu[7]=lSquare-1;
    }
    //�����ұ߽��ϵķ���
    else if( leave==SceSquareW-1 )
    {
        lpAroSqu[0]=lSquare-SceSquareW-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=-1;
        lpAroSqu[3]=-1;
        lpAroSqu[4]=-1;
        lpAroSqu[5]=lSquare+SceSquareW;
        lpAroSqu[6]=lSquare+SceSquareW-1;
        lpAroSqu[7]=lSquare-1;
    }
    //���ڳ����߽��ϵķ���
    else {
        lpAroSqu[0]=lSquare-SceSquareW-1;
        lpAroSqu[1]=lSquare-SceSquareW;
        lpAroSqu[2]=lSquare-SceSquareW+1;
        lpAroSqu[3]=lSquare+1;
        lpAroSqu[4]=lSquare+SceSquareW+1;
        lpAroSqu[5]=lSquare+SceSquareW;
        lpAroSqu[6]=lSquare+SceSquareW-1;
        lpAroSqu[7]=lSquare-1;
    }
    return lpAroSqu;
}

/*******************************************************
*  ������: GetNearSquare(...) ����CGetPath��ĳ�Ա
*   ���� : ��ָ��λ�õĶ�Ӧ��ָ��Ŀ��λ�õĽ�����
*            (c)  jk&ljq, 2010
********************************************************/
LONG  CGetPath::GetNearSquare( POINT Pos, POINT DestPos )
{
    LPLONG  lpSquare;
    LONG    destSquare;
    LONG    lDis[8];

    //��Ŀ��λ��ת��Ϊ����
    destSquare = GetSquareFromPos(DestPos);
    //������ܵķ���
    lpSquare = GetAroundSquare( Pos );
    //�����ܷ��鵽Ŀ�ķ���ľ���
    for( WORD i=0;i<8;i++ )
        lDis[i]=Distance(lpSquare[i],destSquare);

    //����С����
    WORD Min=0;//�����һ������Сֵ
    for( i=0;i<8;i++ )
    {
        if(lDis[Min]>lDis[i])
            Min = i;
    }
    return lpSquare[Min];
}

/*******************************************************
*  ������: Movable(...) ����CGetPath��ĳ�Ա
*   ���� : ��ָ����ʽ���ָ��λ�õ�Ŀ��λ�ü���
           �����ֱ�ߵ��
           �����·�����������,������Ŀ�ĵ�
*            (c)  jk&ljq, 2010
********************************************************/
BOOL CGetPath::Movable( POINT StartPos,
                        POINT DestPos,
                        LPPATH &lpPath,
                        BOOL bFlags )
{
    lpPath = NULL;

    LONG step;
    LONG Ex=0,Ey=0,dx,dy,inCX,inCY;

    //>>>>> ��Bresenham�㷨����ǰ�� >>>>>>>
    LONG lStartSqu = GetSquareFromPos(StartPos);
    LONG lDestSqu  = GetSquareFromPos(DestPos);
    LONG lStartY = lStartSqu/SceSquareW;
    LONG lStartX = lStartSqu-SceSquareW*lStartY;
    LONG lDestY = lDestSqu/SceSquareW;
    LONG lDestX = lDestSqu-SceSquareW*lDestY;

    dx =lDestX-lStartX;
    dy =lDestY-lStartY;

    //�жϺ��������ǰ������
    if( dx==0 )
        inCX = 0;
    else
        inCX = dx>0?1:-1;

    //�ж����������ǰ������
    if( dy==0 )
        inCY = 0;
    else
        inCY = dy>0?1:-1;

    //���ֵ�ľ���ֵ
    if(dx<0)
        dx = -dx;
    if(dy<0)
        dy = -dy;

    //���ֵ
    step = dx>dy?dx:dy;

    //>>>>> ����Bresenham�㷨�������� >>>>>>>>>
    LONG lSqu;
    LPPATH lpStep=NULL, lpTemp=NULL;
    //���������
    Ex = dx;  Ey = dy;
    for (WORD i=0; i<step && lCount<lMaxStep; i++)
    {
        Ex+=dx;//�ۻ�
        Ey+=dy;//�ۻ�
        if( Ex>step )
        {//�����ں�������ǰ��һ������
            Ex-=step;//ȥ����ǰ���ķ������
            lStartX+=inCX;//ǰ��
        }
        if( Ey>step )
        {//��������������ǰ��һ������
            Ey-=step;//ȥ����ǰ���ķ������
            lStartY+=inCY;//ǰ��
        }

        lSqu = MakeSquare(lStartY,lStartX);
        if( lSqu==-1 )//����������Χ
            return FALSE;
        //�ϰ����Եķ���
        if( Square[lSqu]!=0 )
            return FALSE;
        if( bFlags==TRUE )//���·��,�����ٿռ�
        {
            ++lCount;
            lpTemp = new PATH;
            lpTemp->next = NULL;
            if( lpStep==NULL )
            {
                lpPath = lpTemp;
                lpPath->before = NULL;
                lpStep = lpPath;
            }
            else {
                lpStep->next = lpTemp;
                lpTemp->before = lpStep;
                lpStep=lpStep->next;
            }
            lpStep->Pos = GetPosFromSquare(lSqu);
        }
    }
    return TRUE;
}

/*******************************************************
*  ������: GetLastMember(...) ����CGetPath��ĳ�Ա
*   ���� : ���ָ��·�������һ��λ��
*            (c)  jk&ljq, 2010
********************************************************/
LPPATH CGetPath::GetLastMember( LPPATH lpPath )
{
    while( lpPath!=NULL )
    {
        if( lpPath->next!=NULL )
            lpPath = lpPath->next;
        else
            break;
    }
    return lpPath;
}

/*******************************************************
*  ������: IsWalkableSquare(...)����CGetPath��ĳ�Ա
*   ���� : ��ָ�������Ƿ��Ƿ��ϰ�����
*            (c)  jk&ljq, 2010
********************************************************/
inline BOOL CGetPath::IsWalkableSquare( LONG lSquare )
{
    if( lSquare==-1 )//���鳬��������Χ
        return FALSE;
    if( Square[lSquare]==0 )//���鲻���ϰ�
        return TRUE;
    return FALSE;//�ϰ�����
}

/*******************************************************
*  ������: DoFor(...) ����CGetPath��ĳ�Ա
*   ���� : ��ָ���ķ���ѭ���仯0��7�������
*            (c)  jk&ljq, 2010
********************************************************/
inline LONG CGetPath::DoFor( LONG lIndex,
                             LONG lFlags )
{
    //˳ʱ��Ѱ��
    if( lFlags > 0 )
    {
        // �����ڵ����ڶ��������Ҳ�С�ڵ�һ����
	    if( lIndex<7 && lIndex>=0 )
            ++lIndex;
        // �����һ������ת����һ����
        else
            lIndex = 0;
    }
    //��ʱ��Ѱ��
    else {
        if( lIndex<=7 && lIndex>0 )
            --lIndex;
        else
            lIndex = 7;
    }

	return	lIndex;
}

/************************************************************
*   ������: GetNextPos(...) ����CGetPath��ĳ�Ա
*     ����: ��ָ���ķ�������λ��
*       (c) jk&ljq, 2010
*************************************************************/
POINT CGetPath::GetNextPos( POINT p1,
                            LPLONG lpRelateSqu,
                            LONG lFlags )
{
    //��ָ��λ�õ�"�˸�����"
    LPLONG lpAroSqu =GetAroundSquare(p1);
    LONG lIndex=-1, lOldIndex;
    //���������
    for(WORD i=0;i<8;i++)
    {
        if( lpAroSqu[i]==(*lpRelateSqu) )
        {
            lIndex=i;//��ȡ��������
            break;
        }
    }

    if( lIndex==-1 )//��Ч�ķ���
    {
        p1.x = -1; p1.y = -1;
        return p1;
    }

    //��λ��
    for(i=0;i<7;i++)
    {
        lOldIndex = lIndex;
        //����Ϊ��һ���߷����"�˸�����"������
        lIndex = DoFor( lIndex, lFlags );
        if( IsWalkableSquare(lpAroSqu[lIndex]) )
        {//���ϰ�����
            p1 =GetPosFromSquare(lpAroSqu[lIndex]);
            *lpRelateSqu = lpAroSqu[lOldIndex];
            break;
        }
    }

    return p1;
}

/************************************************************
*   ������: IsOldPos(...) ����CGetPath��ĳ�Ա
*     ����: �ж�ָ����λ���Ƿ���ָ����·����
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::IsOldPos( POINT Pos,
                         LPPATH lpPath )
{
    //�ǿ�·����ֱ�ӷ���
    if(lpPath==NULL)
        return FALSE;

    LONG lPathSqu;//��ǰ����
    LONG lPathOldSqu;//�ɵķ���
    LONG dn,dx;//��ֵ
    LONG lSqu = GetSquareFromPos(Pos);

    //�ɵķ���ĳ�ʼֵ��·�������һ������
    LPPATH p = GetLastMember(lpPath);
    lPathOldSqu = GetSquareFromPos(p->Pos);
    while( lpPath!=NULL )
    {
        lPathSqu = GetSquareFromPos(lpPath->Pos);
        if( lPathSqu==lSqu )
            return TRUE;

        //б�����ӵ�·��
        dn = lPathOldSqu - lPathSqu;
        dx = lPathOldSqu - lSqu;
        //�ɵķ��������£���ǰ�ķ���������
        if( dn==SceSquareW+1 ) {
            if( dx==SceSquareW || dx==1 )
                return TRUE;
        }
        //�ɵķ��������ϣ���ǰ�ķ���������
        else if( dn==-SceSquareW-1 ) {
            if( dx==-SceSquareW || dx==-1 )
                return TRUE;
        }
        //�ɵķ��������£���ǰ�ķ���������
        else if( dn==SceSquareW-1 ) {
            if( dx==SceSquareW || dx==-1 )
                return TRUE;
        }
        //�ɵķ��������ϣ���ǰ�ķ���������
        else if( dn==1-SceSquareW ) {
            if( dx==-SceSquareW || dx==1 )
                return TRUE;
        }

        //�ɵĺ��µ���һ
        lpPath = lpPath->next;
        lPathOldSqu = lPathSqu;
    }
    return FALSE;
}

/************************************************************
*   ������: DeletePath(...) ����CGetPath��ĳ�Ա
*     ����: �ͷ�ָ��·���Ŀռ�
*       (c) jk&ljq, 2010
*************************************************************/
void CGetPath::DeletePath( LPPATH lpPath )
{
    LPPATH lpTemp = NULL;
    while( lpPath!=NULL )
	{
          lpTemp = lpPath->next;
          delete lpPath;
          lpPath = lpTemp;
	}
}

/************************************************************
*   ������: ArcSearch(...) ����CGetPath��ĳ�Ա
*     ����: ����ָ�������԰�������ʽ����·������ʵ
            ���������Ŀ�Խ�����б������󣬵ó���
            ���ֻ��������ʵ�������Ŀ�Խ(û����
            ��·��,����ֵ����)��������Ŀ�ĵ�(û��
            �ͷ�·��,����ֵ�Ǽ�)���޷�����Ŀ�ĵ��
            �������Ĳ�������������(�ͷ�·��,����ֵ
            �Ǽ�)
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::ArcSearch( POINT  StartPos,
                          POINT  DestPos,
                          LPPATH &lpPath,
                          LPPATH lpOldPath,
                          LONG   lFlags )
{
    LPPATH lpStep=NULL,lpTemp=NULL,lpPath3=NULL;
    LONG  lP1Squ;
    //Ŀ�ķ���
    LONG  lStartSqu = GetSquareFromPos(StartPos);
    //��ʼλ��
    POINT p1 = StartPos;

    //���һ�����ϰ���Ե�������顣�����������
    //�ǡ��˸����顱�����Ŀ�ĵ�ķ���
    LONG lRelateSqu = GetNearSquare(p1,DestPos);

    while( lCount<lMaxStep )
    {
        //����һλ��
        p1 = GetNextPos( p1, &lRelateSqu, lFlags );
        lP1Squ = GetSquareFromPos(p1);

        //�ص������,�����˳�����Χ
        if( lP1Squ==lStartSqu || p1.x==-1 )
        {
            SafeDeletePath(lpPath);
            return FALSE;
        }

        //���ٿռ��¼�����λ��
        lpTemp = new PATH;
        lpTemp->next = NULL;
        if( lpStep==NULL )
        {
            lpPath = lpTemp;
            lpPath->before = NULL;
            lpStep = lpPath;
        }
        else {
            lpStep->next = lpTemp;
            lpTemp->before = lpStep;
            lpStep=lpStep->next;
        }
        lpStep->Pos = p1;

        //�µ�λ�ÿ���ֱ�ߵ���Ŀ�ĵ�,����·������
        if( Movable(p1,DestPos,lpPath3,TRUE) )
        {
            lpStep->next=lpPath3;
            lpPath3->before=lpStep;
            return FALSE;
        }
        //�µ�λ�ÿ��Գ���Ŀ�ķ���ֱ�߾���"һЩ
        //����"�������ܵ���Ŀ�ĵ�
        else if( lpPath3!=NULL )
        {
            //��"һЩ����"���һ������
            LPPATH p=GetLastMember(lpPath3);
            //�����󷽿鴦����ǰ��������·����
            if( IsOldPos(p->Pos,lpOldPath) )
                {SafeDeletePath(lpPath3);}
            //�����󷽿鴦�ڱ�������������·����
            else if( IsOldPos(p->Pos,lpPath) )
                {SafeDeletePath(lpPath3);}
            else {//"һЩ����"������Ϊ·������
                lpStep->next=lpPath3;
                lpPath3->before=lpStep;
                return TRUE;
            }
        }
        ++lCount;
    }
    //�����������������
    SafeDeletePath( lpPath );
    return FALSE;
}

/************************************************************
*   ������: RoundSearch(...) ����CGetPath��ĳ�Ա
*     ����: �����ָ���������ڵ��������ϰ�������
            ����һ������ϰ�Ⱥ��
            ����������ĵ㳬���˳�����Χ���򱾺�
            ������Ϊ·���Ǳպϵġ�
            ���б������󣬵ó��Ľ��ֻ��������·
            ���Ǳպϵ�(û���ͷŸ�·��,����ֵ����)
            ��������Ŀ�ĵ�(û���ͷ�·��,����ֵ��
            ��)���޷�����Ŀ�ĵ���������Ĳ�����
            ��������(�ͷ�·��,����ֵ�Ǽ�)
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::RoundSearch( POINT  StartPos,
                            POINT  DestPos,
                            LPPATH &lpPath,
                            LONG   lFlags )
{
    LPPATH lpStep=NULL,lpTemp=NULL,lpPath3=NULL;
    LONG  lP1Squ;
    LONG  lStartSqu = GetSquareFromPos(StartPos);
    //��ʼ��
    POINT p1 = StartPos;

    //���һ�����ϰ���Ե�������顣�����������
    //�ǡ��˸����顱�����Ŀ�ĵ�ķ���
    LONG lRelateSqu = GetNearSquare(p1,DestPos);

    while( lCount<lMaxStep )
    {
        //���ݹ�������������һλ��
        p1 = GetNextPos( p1, &lRelateSqu, lFlags );
        lP1Squ = GetSquareFromPos(p1);

        //���ٿռ��¼��������λ��
        lpTemp = new PATH;
        lpTemp->next = NULL;
        if( lpStep==NULL )
        {
            lpPath = lpTemp;
            lpPath->before = NULL;
            lpStep = lpPath;
        }
        else {
            lpStep->next = lpTemp;
            lpTemp->before = lpStep;
            lpStep=lpStep->next;
        }
        lpStep->Pos = p1;

        //�ص������,�����˳�����Χ
        if( lP1Squ==lStartSqu || p1.x==-1 )
            return TRUE;

        //�µ�λ�ÿ���ֱ�ߵ���Ŀ�ĵ�
        if( Movable(p1,DestPos,lpPath3,TRUE) )
        {
            lpStep->next=lpPath3;
            lpPath3->before=lpStep;
            return FALSE;
        }
        else//���������ͷ�·���Ŀռ�
            SafeDeletePath( lpPath3 );
        ++lCount;
    }
    //�����������������
    SafeDeletePath( lpPath );
    return FALSE;
}

/************************************************************
*   ������: MethodOne(...) ����CGetPath��ĳ�Ա
*     ����: ����ʽ�������ʽ�Ľ��ʹ��
*       (c) jk&ljq, 2010
*************************************************************/
LPPATH CGetPath::MethodOne( POINT p1,
                            POINT DestPos,
                            LPLONG lplGet,
                            LONG  lFlags )
{
    LONG oldlCount;
    LPPATH lpPath2=NULL, lpPath3=NULL;
    LPPATH lpStep=NULL, lpTemp=NULL;

    //��������
    while( lCount<lMaxStep )
    {
        oldlCount = lCount;
        //�ǻ���·��,���Խ����һ�ϰ�Ⱥ
        if(RoundSearch(p1,DestPos,lpPath3,lFlags))
        {
            lCount = oldlCount;
            lpTemp = lpPath3;
            //ʵ��������Խ
            if( ArcSearch(p1,DestPos,lpPath3,lpTemp,lFlags) )
            {
                SafeDeletePath( lpTemp );
                if( lpStep==NULL )
                    lpPath2=lpPath3;
                else
                    lpStep->next=lpPath3;
            }
            //����Ŀ�ĵ�
            else if( lpPath3!=NULL )
            {
                SafeDeletePath( lpTemp );
                if( lpStep==NULL )
                    lpPath2=lpPath3;
                else
                    lpStep->next=lpPath3;
                *lplGet = TRUE;
                break;
            }
            //�޷�����Ŀ�ĵ�
            else {
                SafeDeletePath( lpTemp );
                SafeDeletePath( lpPath3 );
                break;
            }
        }
        //����Ŀ�ĵ�
        else if( lpPath3!=NULL )
        {
            if( lpStep==NULL )
                lpPath2=lpPath3;
            else
                lpStep->next=lpPath3;
            *lplGet = TRUE;
            break;
        }
        //�޷�����Ŀ�ĵ�
        else
            break;
        //ÿ��ѭ����lpStep��������
        //�ڶ���·�������һλ��
        lpStep = GetLastMember(lpPath2);
        p1 = lpStep->Pos;//�µ����
    }
    return lpPath2;
}

/************************************************************
*   ������: MethodTwo(...) ����CGetPath��ĳ�Ա
*     ����: ֻʹ������ʽ����·��
*       (c) jk&ljq, 2010
*************************************************************/
inline LPPATH CGetPath::MethodTwo(POINT  p1,
                                  POINT  DestPos,
                                  LPLONG lplGet,
                                  LONG   lFlags)
{
    LPPATH lpPath2=NULL;

    //�ǻ���·��
    if(RoundSearch(p1,DestPos,lpPath2,lFlags))
    {
        SafeDeletePath( lpPath2 );
        *lplGet = FALSE;
    }
    //����Ŀ�ĵ�
    else if( lpPath2!=NULL )
    {
        *lplGet = TRUE;
    }
    //�޷�����Ŀ�ĵ�
    else
        *lplGet = FALSE;

    return lpPath2;
}

/************************************************************
*   ������: MethodThree(...) ����CGetPath��ĳ�Ա
*     ����: ֻʹ�ð�������ʽ����·��
*       (c) jk&ljq, 2010
*************************************************************/
inline LPPATH CGetPath::MethodThree(POINT  p1,
                                  POINT  DestPos,
                                  LPPATH lpPath1,
                                  LPLONG lplGet,
                                  LONG   lFlags)
{
    LPPATH lpPath2=NULL;

    //ʵ����������Խ
    if(ArcSearch(p1,DestPos,lpPath2,lpPath1,lFlags))
    {
        *lplGet = 3;
    }
    //����Ŀ�ĵ�
    else if( lpPath2!=NULL )
    {
        *lplGet = TRUE;
    }
    //�޷�����Ŀ�ĵ�
    else
        *lplGet = FALSE;

    return lpPath2;
}

/************************************************************
*   ������: GetBasicPath(...) ����CGetPath��ĳ�Ա
*     ����: Ѱ��ԭʼ·��
* ����˵��: StartPos �����, DestPos ��Ŀ�ĵ�,
*           lFlags ��ʾ��������(����0 =>��ʱ��,
            С��0 =>˳ʱ��),lplGetָ���¼�Ƿ�
            ������Ŀ��λ�õı���
*  ����ֵ: ����ֵΪTRUE��ʾ·����������,
           ����ֵΪFALSE��ʾ·����ֱ�ߵ�
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::GetBasicPath( POINT StartPos,
                             POINT DestPos,
                             LPPATH &lpPath,
                             LPLONG lplGet,
                             LONG   lFlags)
{
    //���������Ŀ�ĵ�,ʹ���Ǵ��ڸ��ӵ�����λ��
    StartPos = AdjustPos( StartPos );
    DestPos  = AdjustPos( DestPos );

    //���������ļ�¼,��ʼֵΪ0
    lCount = 0;

    *lplGet = FALSE;

    //>>>>>>>> ��һ������ >>>>>>>>>>
    LPPATH lpPath1 = NULL;
    //��������Ŀ�ĵ��û���ϰ�
    //��ֱ�ӷ��ص�һ��·��
    if( Movable(StartPos, DestPos, lpPath1, TRUE) )
    {
        lpPath = lpPath1;
        return FALSE;
    }

    //>>>>>>>> �ڶ������� >>>>>>>>>>
    POINT  p1;
    LPPATH lpPath1LastMem = NULL;
    LPPATH lpPath2=NULL;

    //��õ�һ·�������һ��
    lpPath1LastMem = GetLastMember( lpPath1 );
    if( lpPath1LastMem != NULL )
        p1 = lpPath1LastMem->Pos;
    else
        p1 = StartPos;

    //>>>>>>>> �ڶ����������� >>>>>>>>>>
    //������ʽ�Ͱ�����ʽ�Ľ������
    if( lFlags==1 || lFlags==-1 )
        lpPath2 = MethodOne(p1,DestPos,lplGet,lFlags);
    //������ʽ����
    else if( lFlags==2 || lFlags==-2 )
        lpPath2 = MethodTwo(p1,DestPos,lplGet,lFlags);
    //�ǰ�����ʽ����
    else if( lFlags==3 || lFlags==-3 )
        lpPath2 = MethodThree(p1,DestPos,lpPath1,lplGet,lFlags);

    //�������������ó���·��
    if( lpPath2 != NULL )
    {
        if(lpPath1LastMem!=NULL)
        {//�е�һ·��,Ҳ�еڶ�·��
            lpPath1LastMem->next = lpPath2;
            lpPath2->before = lpPath1LastMem;
            lpPath = lpPath1;
        }
        else//û�е�һ·��
            lpPath = lpPath2;
        return TRUE;//�ҵ���·����Ҫ����
    }
    else
        lpPath = lpPath1;
    return FALSE;
}

/*********************************************************
*   ������: GetPathCount(...) ����CGetPath��ĳ�Ա
*     ����: ��ָ��·����λ����(������)
*       (c) jk&ljq, 2010
**********************************************************/
LONG CGetPath::GetPathCount(LPPATH lpPath )
{
    LONG lCount=0;
    while( lpPath!=NULL )
    {
        ++lCount;
        lpPath = lpPath->next;
    }
    return lCount;
}

/*********************************************************
*   ������: GetTheBestPath(...) ����CGetPath��ĳ�Ա
*     ����: ��ָ���ĵȼ�Ѱ·
* ����˵��: StartPos �����, DestPos ��Ŀ�ĵ㡣
*  lFlags ��ʾ�����ȼ����������£�
*  ֵ��1��ʱ,�������������,�ķѵ�ʱ���
*  ֵ��2��ʱ,�����б�һ���ϼ򵥵�����,�ﵽĿ��
*            �صĿ����Ա�1��С,�ķѵ�ʱ��Ҳ����
*  ֵ��3��ʱ,�����и��򵥵�����,�ķѵ�ʱ����١�
*       (c) jk&ljq, 2010
**********************************************************/
LPPATH CGetPath::GetTheBestPath(POINT StartPos,
                                POINT DestPos,
                                LONG lFlags )
{
    LPPATH lpPath1=NULL,lpPath2=NULL;
    LONG   lGet1,lGet2;

    //������ϰ�����Ѱ·
    if( check_eight_direction(StartPos) )
        return NULL;

    //���˳ʱ������ʱ�뷽��ɨ���·��
    GetBasicPath( StartPos, DestPos,
                  lpPath1, &lGet1, lFlags );
    GetBasicPath( StartPos, DestPos,
                  lpPath2, &lGet2, -lFlags );

    //���������ҵ�·����ʵ���˿�Խ��ȡ��̵�
    if( lGet1==lGet2 && lGet1 )
    {
        LONG lPath1C,lPath2C;
        lPath1C = GetPathCount(lpPath1);
        lPath2C = GetPathCount(lpPath2);

        if(lPath1C < lPath2C)
        {
            SafeDeletePath( lpPath2 );
            return lpPath1;
        }
        else {
            SafeDeletePath( lpPath1 );
            return lpPath2;
        }
    }
    //ֻ��˳ʱ�뷽���ҵ���ȡ˳ʱ���·��
    else if( lGet1==1 ) {
        SafeDeletePath( lpPath2 );
        return lpPath1;
    }
    //ֻ����ʱ�뷽���ҵ���ȡ��ʱ���·��
    else if( lGet2==1 ) {
        SafeDeletePath( lpPath1 );
        return lpPath2;
    }
    //ֻ��˳ʱ�뷽��ʵ����������Խ
    else if( lGet1==3 ) {
        SafeDeletePath( lpPath2 );
        return lpPath1;
    }
    //ֻ����ʱ�뷽��ʵ����������Խ
    else if( lGet2==3 ) {
        SafeDeletePath( lpPath1 );
        return lpPath2;
    }
    // ���������Ҳ���
    // ��ʵ�ֲ��˿�Խ��ȡֱ��·��
    // ���߿���ֱ�ӵ���Ŀ�ĵ�
    SafeDeletePath( lpPath2 );
    return lpPath1;
}
/*********************************************************
*   ������: check_eight_direction(...) ����CGetPath��ĳ�Ա
*     ����:  ���ĵ㸽�����䱾���Ƿ�Ϊ�ϰ���
* ����˵��: a ����
����ֵ�� TRUE ���ϰ��� FALSE �����ϰ���
**********************************************************/
BOOL CGetPath::check_eight_direction(POINT a){
	LONG x=GetSquareFromPos(a);
	LONG y = x-SceSquareW;
	LONG z = x+SceSquareW;
	if( Square[x]!=0 )
		return TRUE;
	if(Square[x-1]!=0 &&Square[x+1]!=0
		&& Square[y]!=0 && Square[y-1]!=0 && Square[y+1]!=0
		&&Square[z+1]!=0 && Square[z]!=0 && Square[z-1]!=0)
		return TRUE;
	return FALSE;
}
