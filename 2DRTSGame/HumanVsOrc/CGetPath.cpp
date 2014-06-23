/*-------------------------------------------
|              CGetPath.cpp
|   A-Star寻路算法相关处理功能具体代码
|           版权所有  jk&ljq
---------------------------------------------*/

#include <windows.h>
#include <math.h>
#include "CGetPath.h"

#define  SafeDeletePath(p) {DeletePath(p);p=NULL;}

/************************************************************
*   函数名: CGetPath(...) 属于CGetPath类的成员
*     功能: 构造函数
*       (c) jk&ljq, 2010
*************************************************************/
CGetPath::CGetPath( LONG SquareC, LPVOID lpSquare,
                    DWORD SceW, DWORD SceH,
                    RECT MonsterRt )
{
    lSquareC = SquareC;//获得格子的宽度
    lHarfSquC = lSquareC/2;
    //获得场景格子数组首地址
    Square   = (LPWORD)lpSquare;
    lCount =0;
    SceneW = SceW;
    SceneH = SceH;
    SceSquareW = SceneW/lSquareC;
    SceSquareH = SceneH/lSquareC;
    MonsterRect = MonsterRt;

    //根据怪物激活区的大小求出最大搜索步数
    LONG w=MonsterRect.right-MonsterRect.left;
    LONG h=MonsterRect.bottom-MonsterRect.top;
    lMaxStep = (w/lSquareC)*(h/lSquareC);
}

/************************************************************
*   函数名: CGetPath(...) 属于CGetPath类的成员
*     功能: 构造函数重载
*       (c) jk&ljq, 2010
*************************************************************/
CGetPath::CGetPath( void )
{
    lSquareC = 30;//获得格子的宽度
    lHarfSquC = lSquareC/2;

    //场景格子数组首地址置为NULL,以后可
    //以通过调用Set_Square成员获得地址
    Square   = NULL;

    lCount =0;
    SceneW = 1024;//默认的场景宽度
    SceneH = 768;//默认的场景高度
    SceSquareW = SceneW/lSquareC;
    SceSquareH = SceneH/lSquareC;

    //默认的怪物激活区的大小为1000*800
    MonsterRect.left  =0;
    MonsterRect.right =1000;
    MonsterRect.top   =0;
    MonsterRect.bottom=800;

    //根据怪物激活区的大小求出最大搜索步数
    LONG w=MonsterRect.right-MonsterRect.left;
    LONG h=MonsterRect.bottom-MonsterRect.top;
    lMaxStep = (w/lSquareC)*(h/lSquareC);
}

/*******************************************************
*  函数名: MakeSquare(...) 属于CGetPath类的成员
*   功能 : 求两维方式所表达的方块
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
*   函数名: GetSquareFromPos(...) 属于CGetPath类的成员
*     功能: 求出指定位置所对应的方块
*       (c) jk&ljq, 2010
*************************************************************/
inline LONG  CGetPath::GetSquareFromPos( POINT Pos )
{
    return MakeSquare( Pos.y / lSquareC,
                       Pos.x / lSquareC);
}

/************************************************************
*   函数名: GetPosFromSquare(...) 属于CGetPath类的成员
*     功能: 求出指定方块的中央位置
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
*   函数名: Distance(...) 属于CGetPath类的成员
*     功能: 求两个位置间的距离
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
*   函数名: Distance(...) 属于CGetPath类的成员
*     功能: 重载，求两个方块间的距离
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
*  函数名: AdjustPos(...) 属于CGetPath类的成员
*   功能 : 调整指定位置，使其处于所在方块的中央
*            (c)  jk&ljq, 2010
********************************************************/
inline POINT CGetPath::AdjustPos( POINT P )
{
    return GetPosFromSquare(GetSquareFromPos(P));
}

/************************************************************
* 函数名: GetAroundSquare(...) 属于CGetPath类的成员
*   功能: 获得指定位置的四周方块,其返回值是八个
*         元素的数组首地址。
*       (c) jk&ljq, 2010
*************************************************************/
inline LPLONG CGetPath::GetAroundSquare( POINT Pos )
{
    static LONG lpAroSqu[8];

    LONG lSquare = GetSquareFromPos(Pos);
    //求以两维方式表达的方块行号
    LONG Index = lSquare/SceSquareW;
    //求以两维方式表达的方块列号
    LONG leave = lSquare - Index*SceSquareW;

    //场景左上角的方块
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
    //场景右上角的方块
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
    //场景右下角的方块
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
    //场景左下角的方块
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
    //场景左边界上的方块
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
    //场景上边界上的方块
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
    //场景底边界上的方块
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
    //场景右边界上的方块
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
    //不在场景边界上的方块
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
*  函数名: GetNearSquare(...) 属于CGetPath类的成员
*   功能 : 求指定位置的对应于指定目的位置的近方块
*            (c)  jk&ljq, 2010
********************************************************/
LONG  CGetPath::GetNearSquare( POINT Pos, POINT DestPos )
{
    LPLONG  lpSquare;
    LONG    destSquare;
    LONG    lDis[8];

    //将目的位置转化为方块
    destSquare = GetSquareFromPos(DestPos);
    //获得四周的方块
    lpSquare = GetAroundSquare( Pos );
    //求四周方块到目的方块的距离
    for( WORD i=0;i<8;i++ )
        lDis[i]=Distance(lpSquare[i],destSquare);

    //求最小距离
    WORD Min=0;//假设第一个是最小值
    for( i=0;i<8;i++ )
    {
        if(lDis[Min]>lDis[i])
            Min = i;
    }
    return lpSquare[Min];
}

/*******************************************************
*  函数名: Movable(...) 属于CGetPath类的成员
*   功能 : 以指定方式求出指定位置到目的位置间是
           否可以直线到达。
           求出的路径不包括起点,但包括目的点
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

    //>>>>> 以Bresenham算法搜索前进 >>>>>>>
    LONG lStartSqu = GetSquareFromPos(StartPos);
    LONG lDestSqu  = GetSquareFromPos(DestPos);
    LONG lStartY = lStartSqu/SceSquareW;
    LONG lStartX = lStartSqu-SceSquareW*lStartY;
    LONG lDestY = lDestSqu/SceSquareW;
    LONG lDestX = lDestSqu-SceSquareW*lDestY;

    dx =lDestX-lStartX;
    dy =lDestY-lStartY;

    //判断横向坐标的前进方向
    if( dx==0 )
        inCX = 0;
    else
        inCX = dx>0?1:-1;

    //判断纵向坐标的前进方向
    if( dy==0 )
        inCY = 0;
    else
        inCY = dy>0?1:-1;

    //求差值的绝对值
    if(dx<0)
        dx = -dx;
    if(dy<0)
        dy = -dy;

    //求大值
    step = dx>dy?dx:dy;

    //>>>>> 采用Bresenham算法搜索方块 >>>>>>>>>
    LONG lSqu;
    LPPATH lpStep=NULL, lpTemp=NULL;
    //不包括起点
    Ex = dx;  Ey = dy;
    for (WORD i=0; i<step && lCount<lMaxStep; i++)
    {
        Ex+=dx;//累积
        Ey+=dy;//累积
        if( Ex>step )
        {//可以在横向坐标前进一个方块
            Ex-=step;//去掉所前进的方块的量
            lStartX+=inCX;//前进
        }
        if( Ey>step )
        {//可以在纵向坐标前进一个方块
            Ey-=step;//去掉所前进的方块的量
            lStartY+=inCY;//前进
        }

        lSqu = MakeSquare(lStartY,lStartX);
        if( lSqu==-1 )//超出场景范围
            return FALSE;
        //障碍属性的方块
        if( Square[lSqu]!=0 )
            return FALSE;
        if( bFlags==TRUE )//获得路径,即开辟空间
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
*  函数名: GetLastMember(...) 属于CGetPath类的成员
*   功能 : 获得指定路径的最后一个位置
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
*  函数名: IsWalkableSquare(...)属于CGetPath类的成员
*   功能 : 求指定方块是否是非障碍方块
*            (c)  jk&ljq, 2010
********************************************************/
inline BOOL CGetPath::IsWalkableSquare( LONG lSquare )
{
    if( lSquare==-1 )//方块超出场景范围
        return FALSE;
    if( Square[lSquare]==0 )//方块不是障碍
        return TRUE;
    return FALSE;//障碍方块
}

/*******************************************************
*  函数名: DoFor(...) 属于CGetPath类的成员
*   功能 : 以指定的方向循环变化0到7间的整数
*            (c)  jk&ljq, 2010
********************************************************/
inline LONG CGetPath::DoFor( LONG lIndex,
                             LONG lFlags )
{
    //顺时针寻找
    if( lFlags > 0 )
    {
        // 不大于倒数第二个数，且不小于第一个数
	    if( lIndex<7 && lIndex>=0 )
            ++lIndex;
        // 是最后一个数则转到第一个数
        else
            lIndex = 0;
    }
    //逆时针寻找
    else {
        if( lIndex<=7 && lIndex>0 )
            --lIndex;
        else
            lIndex = 7;
    }

	return	lIndex;
}

/************************************************************
*   函数名: GetNextPos(...) 属于CGetPath类的成员
*     功能: 以指定的方向搜索位置
*       (c) jk&ljq, 2010
*************************************************************/
POINT CGetPath::GetNextPos( POINT p1,
                            LPLONG lpRelateSqu,
                            LONG lFlags )
{
    //求指定位置的"八个方块"
    LPLONG lpAroSqu =GetAroundSquare(p1);
    LONG lIndex=-1, lOldIndex;
    //求关联方块
    for(WORD i=0;i<8;i++)
    {
        if( lpAroSqu[i]==(*lpRelateSqu) )
        {
            lIndex=i;//获取关联方块
            break;
        }
    }

    if( lIndex==-1 )//无效的方块
    {
        p1.x = -1; p1.y = -1;
        return p1;
    }

    //求位置
    for(i=0;i<7;i++)
    {
        lOldIndex = lIndex;
        //求将作为下一行走方块的"八个方块"的索引
        lIndex = DoFor( lIndex, lFlags );
        if( IsWalkableSquare(lpAroSqu[lIndex]) )
        {//非障碍方块
            p1 =GetPosFromSquare(lpAroSqu[lIndex]);
            *lpRelateSqu = lpAroSqu[lOldIndex];
            break;
        }
    }

    return p1;
}

/************************************************************
*   函数名: IsOldPos(...) 属于CGetPath类的成员
*     功能: 判定指定的位置是否处于指定的路径中
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::IsOldPos( POINT Pos,
                         LPPATH lpPath )
{
    //是空路径则直接返回
    if(lpPath==NULL)
        return FALSE;

    LONG lPathSqu;//当前方块
    LONG lPathOldSqu;//旧的方块
    LONG dn,dx;//差值
    LONG lSqu = GetSquareFromPos(Pos);

    //旧的方块的初始值是路径的最后一个方块
    LPPATH p = GetLastMember(lpPath);
    lPathOldSqu = GetSquareFromPos(p->Pos);
    while( lpPath!=NULL )
    {
        lPathSqu = GetSquareFromPos(lpPath->Pos);
        if( lPathSqu==lSqu )
            return TRUE;

        //斜向连接的路径
        dn = lPathOldSqu - lPathSqu;
        dx = lPathOldSqu - lSqu;
        //旧的方块在右下，当前的方块在左上
        if( dn==SceSquareW+1 ) {
            if( dx==SceSquareW || dx==1 )
                return TRUE;
        }
        //旧的方块在左上，当前的方块在右下
        else if( dn==-SceSquareW-1 ) {
            if( dx==-SceSquareW || dx==-1 )
                return TRUE;
        }
        //旧的方块在左下，当前的方块在右上
        else if( dn==SceSquareW-1 ) {
            if( dx==SceSquareW || dx==-1 )
                return TRUE;
        }
        //旧的方块在右上，当前的方块在左下
        else if( dn==1-SceSquareW ) {
            if( dx==-SceSquareW || dx==1 )
                return TRUE;
        }

        //旧的和新的下一
        lpPath = lpPath->next;
        lPathOldSqu = lPathSqu;
    }
    return FALSE;
}

/************************************************************
*   函数名: DeletePath(...) 属于CGetPath类的成员
*     功能: 释放指定路径的空间
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
*   函数名: ArcSearch(...) 属于CGetPath类的成员
*     功能: 根据指定方块以半吸附方式搜索路径。即实
            现搜索起点的跨越。运行本函数后，得出的
            结果只有三个：实现了起点的跨越(没有释
            放路径,返回值是真)、到达了目的点(没有
            释放路径,返回值是假)、无法到达目的点或
            者搜索的步数超出了上限(释放路径,返回值
            是假)
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
    //目的方块
    LONG  lStartSqu = GetSquareFromPos(StartPos);
    //开始位置
    POINT p1 = StartPos;

    //求第一次在障碍边缘搜索方块。则其关联方块
    //是“八个方块”中最靠近目的点的方块
    LONG lRelateSqu = GetNearSquare(p1,DestPos);

    while( lCount<lMaxStep )
    {
        //求下一位置
        p1 = GetNextPos( p1, &lRelateSqu, lFlags );
        lP1Squ = GetSquareFromPos(p1);

        //回到了起点,超出了场景范围
        if( lP1Squ==lStartSqu || p1.x==-1 )
        {
            SafeDeletePath(lpPath);
            return FALSE;
        }

        //开辟空间记录求出的位置
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

        //新的位置可以直线到达目的点,搜索路径结束
        if( Movable(p1,DestPos,lpPath3,TRUE) )
        {
            lpStep->next=lpPath3;
            lpPath3->before=lpStep;
            return FALSE;
        }
        //新的位置可以朝着目的方向直线经过"一些
        //方块"，但不能到达目的点
        else if( lpPath3!=NULL )
        {
            //求"一些方块"最后一个方块
            LPPATH p=GetLastMember(lpPath3);
            //这个最后方块处于以前搜索到的路径中
            if( IsOldPos(p->Pos,lpOldPath) )
                {SafeDeletePath(lpPath3);}
            //这个最后方块处于本函数搜索到的路径中
            else if( IsOldPos(p->Pos,lpPath) )
                {SafeDeletePath(lpPath3);}
            else {//"一些方块"可以作为路径加入
                lpStep->next=lpPath3;
                lpPath3->before=lpStep;
                return TRUE;
            }
        }
        ++lCount;
    }
    //超出了最大搜索步数
    SafeDeletePath( lpPath );
    return FALSE;
}

/************************************************************
*   函数名: RoundSearch(...) 属于CGetPath类的成员
*     功能: 检测与指定方块毗邻的连续的障碍方块是
            否是一分离的障碍群。
            如果搜索到的点超出了场景范围，则本函
            数将认为路径是闭合的。
            运行本函数后，得出的结果只有三个：路
            径是闭合的(没有释放该路径,返回值是真)
            、到达了目的点(没有释放路径,返回值是
            假)、无法到达目的点或者搜索的步数超
            出了上限(释放路径,返回值是假)
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
    //开始点
    POINT p1 = StartPos;

    //求第一次在障碍边缘搜索方块。则其关联方块
    //是“八个方块”中最靠近目的点的方块
    LONG lRelateSqu = GetNearSquare(p1,DestPos);

    while( lCount<lMaxStep )
    {
        //根据关联方块搜索下一位置
        p1 = GetNextPos( p1, &lRelateSqu, lFlags );
        lP1Squ = GetSquareFromPos(p1);

        //开辟空间记录搜索到的位置
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

        //回到了起点,超出了场景范围
        if( lP1Squ==lStartSqu || p1.x==-1 )
            return TRUE;

        //新的位置可以直线到达目的点
        if( Movable(p1,DestPos,lpPath3,TRUE) )
        {
            lpStep->next=lpPath3;
            lpPath3->before=lpStep;
            return FALSE;
        }
        else//不可以则释放路径的空间
            SafeDeletePath( lpPath3 );
        ++lCount;
    }
    //超出了最大搜索步数
    SafeDeletePath( lpPath );
    return FALSE;
}

/************************************************************
*   函数名: MethodOne(...) 属于CGetPath类的成员
*     功能: 吸附式与半吸附式的结合使用
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

    //搜索过程
    while( lCount<lMaxStep )
    {
        oldlCount = lCount;
        //是环形路径,则跨越到下一障碍群
        if(RoundSearch(p1,DestPos,lpPath3,lFlags))
        {
            lCount = oldlCount;
            lpTemp = lpPath3;
            //实现搜索跨越
            if( ArcSearch(p1,DestPos,lpPath3,lpTemp,lFlags) )
            {
                SafeDeletePath( lpTemp );
                if( lpStep==NULL )
                    lpPath2=lpPath3;
                else
                    lpStep->next=lpPath3;
            }
            //到达目的点
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
            //无法到达目的点
            else {
                SafeDeletePath( lpTemp );
                SafeDeletePath( lpPath3 );
                break;
            }
        }
        //到达目的点
        else if( lpPath3!=NULL )
        {
            if( lpStep==NULL )
                lpPath2=lpPath3;
            else
                lpStep->next=lpPath3;
            *lplGet = TRUE;
            break;
        }
        //无法到达目的点
        else
            break;
        //每次循环里lpStep都代表着
        //第二个路径的最后一位置
        lpStep = GetLastMember(lpPath2);
        p1 = lpStep->Pos;//新的起点
    }
    return lpPath2;
}

/************************************************************
*   函数名: MethodTwo(...) 属于CGetPath类的成员
*     功能: 只使用吸附式搜索路径
*       (c) jk&ljq, 2010
*************************************************************/
inline LPPATH CGetPath::MethodTwo(POINT  p1,
                                  POINT  DestPos,
                                  LPLONG lplGet,
                                  LONG   lFlags)
{
    LPPATH lpPath2=NULL;

    //是环形路径
    if(RoundSearch(p1,DestPos,lpPath2,lFlags))
    {
        SafeDeletePath( lpPath2 );
        *lplGet = FALSE;
    }
    //到达目的点
    else if( lpPath2!=NULL )
    {
        *lplGet = TRUE;
    }
    //无法到达目的点
    else
        *lplGet = FALSE;

    return lpPath2;
}

/************************************************************
*   函数名: MethodThree(...) 属于CGetPath类的成员
*     功能: 只使用半吸附方式搜索路径
*       (c) jk&ljq, 2010
*************************************************************/
inline LPPATH CGetPath::MethodThree(POINT  p1,
                                  POINT  DestPos,
                                  LPPATH lpPath1,
                                  LPLONG lplGet,
                                  LONG   lFlags)
{
    LPPATH lpPath2=NULL;

    //实现了搜索跨越
    if(ArcSearch(p1,DestPos,lpPath2,lpPath1,lFlags))
    {
        *lplGet = 3;
    }
    //到达目的点
    else if( lpPath2!=NULL )
    {
        *lplGet = TRUE;
    }
    //无法到达目的点
    else
        *lplGet = FALSE;

    return lpPath2;
}

/************************************************************
*   函数名: GetBasicPath(...) 属于CGetPath类的成员
*     功能: 寻找原始路径
* 参数说明: StartPos 是起点, DestPos 是目的点,
*           lFlags 表示搜索方向(大于0 =>逆时针,
            小于0 =>顺时针),lplGet指向记录是否
            到达了目的位置的变量
*  返回值: 返回值为TRUE表示路径是弯曲的,
           返回值为FALSE表示路径是直线的
*       (c) jk&ljq, 2010
*************************************************************/
BOOL CGetPath::GetBasicPath( POINT StartPos,
                             POINT DestPos,
                             LPPATH &lpPath,
                             LPLONG lplGet,
                             LONG   lFlags)
{
    //调整起点与目的点,使他们处于格子的中心位置
    StartPos = AdjustPos( StartPos );
    DestPos  = AdjustPos( DestPos );

    //搜索步数的记录,初始值为0
    lCount = 0;

    *lplGet = FALSE;

    //>>>>>>>> 第一步搜索 >>>>>>>>>>
    LPPATH lpPath1 = NULL;
    //如果起点与目的点间没有障碍
    //则直接返回第一个路径
    if( Movable(StartPos, DestPos, lpPath1, TRUE) )
    {
        lpPath = lpPath1;
        return FALSE;
    }

    //>>>>>>>> 第二步搜索 >>>>>>>>>>
    POINT  p1;
    LPPATH lpPath1LastMem = NULL;
    LPPATH lpPath2=NULL;

    //获得第一路径的最后一点
    lpPath1LastMem = GetLastMember( lpPath1 );
    if( lpPath1LastMem != NULL )
        p1 = lpPath1LastMem->Pos;
    else
        p1 = StartPos;

    //>>>>>>>> 第二步搜索过程 >>>>>>>>>>
    //是吸附式和半吸附式的结合搜索
    if( lFlags==1 || lFlags==-1 )
        lpPath2 = MethodOne(p1,DestPos,lplGet,lFlags);
    //是吸附式搜索
    else if( lFlags==2 || lFlags==-2 )
        lpPath2 = MethodTwo(p1,DestPos,lplGet,lFlags);
    //是半吸附式搜索
    else if( lFlags==3 || lFlags==-3 )
        lpPath2 = MethodThree(p1,DestPos,lpPath1,lplGet,lFlags);

    //连接两步搜索得出的路径
    if( lpPath2 != NULL )
    {
        if(lpPath1LastMem!=NULL)
        {//有第一路径,也有第二路径
            lpPath1LastMem->next = lpPath2;
            lpPath2->before = lpPath1LastMem;
            lpPath = lpPath1;
        }
        else//没有第一路径
            lpPath = lpPath2;
        return TRUE;//找到的路径需要调整
    }
    else
        lpPath = lpPath1;
    return FALSE;
}

/*********************************************************
*   函数名: GetPathCount(...) 属于CGetPath类的成员
*     功能: 求指定路径的位置数(即步数)
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
*   函数名: GetTheBestPath(...) 属于CGetPath类的成员
*     功能: 以指定的等级寻路
* 参数说明: StartPos 是起点, DestPos 是目的点。
*  lFlags 表示搜索等级。描述如下：
*  值是1级时,将进行深度搜索,耗费的时间多
*  值是2级时,将进行比一级较简单的搜索,达到目的
*            地的可能性比1级小,耗费的时间也较少
*  值是3级时,将进行更简单的搜索,耗费的时间很少。
*       (c) jk&ljq, 2010
**********************************************************/
LPPATH CGetPath::GetTheBestPath(POINT StartPos,
                                POINT DestPos,
                                LONG lFlags )
{
    LPPATH lpPath1=NULL,lpPath2=NULL;
    LONG   lGet1,lGet2;

    //起点是障碍点则不寻路
    if( check_eight_direction(StartPos) )
        return NULL;

    //获得顺时针与逆时针方向扫描的路径
    GetBasicPath( StartPos, DestPos,
                  lpPath1, &lGet1, lFlags );
    GetBasicPath( StartPos, DestPos,
                  lpPath2, &lGet2, -lFlags );

    //两个方向都找到路径或实现了跨越则取最短的
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
    //只有顺时针方向找到则取顺时针的路径
    else if( lGet1==1 ) {
        SafeDeletePath( lpPath2 );
        return lpPath1;
    }
    //只有逆时针方向找到则取逆时针的路径
    else if( lGet2==1 ) {
        SafeDeletePath( lpPath1 );
        return lpPath2;
    }
    //只有顺时针方向实现了搜索跨越
    else if( lGet1==3 ) {
        SafeDeletePath( lpPath2 );
        return lpPath1;
    }
    //只有逆时针方向实现了搜索跨越
    else if( lGet2==3 ) {
        SafeDeletePath( lpPath1 );
        return lpPath2;
    }
    // 两个方向都找不到
    // 或实现不了跨越则取直线路径
    // 或者可以直接到达目的点
    SafeDeletePath( lpPath2 );
    return lpPath1;
}
/*********************************************************
*   函数名: check_eight_direction(...) 属于CGetPath类的成员
*     功能:  检测改点附近及其本身是否为障碍点
* 参数说明: a 检查点
返回值： TRUE 是障碍点 FALSE 不是障碍点
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
