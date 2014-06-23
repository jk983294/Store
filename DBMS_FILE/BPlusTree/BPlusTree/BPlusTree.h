#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <windows.h>
#include <fstream>

#define POSITION void*
//#define RANGESEARCH void*

template <class TKey,class TData,INT TRank>
class BPlusTreeNode
{
public:
	INT m_nCnt;    //当前节点中的Key个数
	TKey  m_Key [2 * TRank]; //Key数组
	TData m_Data[2 * TRank]; //Data数组

	BPlusTreeNode<TKey,TData,TRank>* m_pChild[ 2 * TRank + 1 ]; //指向孩子的指针数组
public:
	BPlusTreeNode()
		:m_nCnt(0)
	{
		::ZeroMemory(m_Key, 2 * TRank * sizeof(TKey));
		::ZeroMemory(m_Data,2 * TRank * sizeof(TData));
		::ZeroMemory(m_pChild,( 2 * TRank + 1) * sizeof(BPlusTreeNode<TKey,TData,TRank>*));
	}
};


template <class TKey,class TData,INT TRank>
class BPlusTree
{
protected:
	BPlusTreeNode<TKey,TData,TRank>* m_pRoot; //树根节点
	INT  m_iHeight;        //树高度
	INT m_nKeyCnt;        //键总数
	INT m_nNodeCnt;       //节点总数
protected:
	INT m_nSearchIndex;       // 查找时找到的键在节点中的位置

	TData m_InsData;        // 与要插的键相对应的值
	TKey m_InsKey;        /// 要插入的键

	INT  m_iAccessLevel;       // 当前访问的节点所处的高度 
	BPlusTreeNode<TKey,TData,TRank>* m_pNewNode; // 在节点分割的时候指向新建的节点

	BOOL m_bFlag;         // 节点增减标志
public:
	BPlusTree()
		:m_iHeight(0)
		,m_nKeyCnt(0)
		,m_nNodeCnt(0)
		,m_nSearchIndex(0)
		,m_iAccessLevel(0)
		,m_pNewNode(NULL)
		,m_bFlag(FALSE)
	{
		//申请一个0节点的树根
		m_pRoot = NULL;
	}

	virtual ~BPlusTree()
	{
	}

protected:
	void Insert(BPlusTreeNode<TKey,TData,TRank>*& pNode,const TKey& Key,const TData& Data) 
	{ 
		INT i = 0;
		INT j = 0;
		INT k = 0;

		m_iAccessLevel --;

		if ( m_iAccessLevel < 0 )
		{ 
			m_pNewNode = NULL;
			m_InsKey = Key;
			m_InsData = Data;
			m_nKeyCnt ++; 
			m_bFlag = TRUE;
			//pNode = NewNode(pNode,Key,Data);
			return; 
		}


		for(i = 0, j = pNode->m_nCnt - 1; 
			i < j; 
			k= ( j + i) / 2, ( Key > pNode-> m_Key[k])?( i = k + 1 ):( j = k ));

		if ( Key == pNode-> m_Key[ i ] ) 
		{ 
			Error(1,Key); /* 键已经在树中 */ 
			m_bFlag = FALSE; 
			return; 
		}

		if ( Key > pNode-> m_Key[ i ] ) 
		{   /* i == pNode->m_nCnt - 1 时有可能出现 */ 
			i ++; 
		}

		//向孩子节点插入
		Insert( pNode -> m_pChild[ i ] , Key, Data );

		if ( FALSE == m_bFlag ) 
		{
			return; 
		}

		/* 有新键要插入到当前节点中 */ 
		if ( pNode->m_nCnt < 2 * TRank ) 
		{   /* 当前节点未满 */ 
			InsertAt( pNode, i ,Key,Data); /* 把键值+子树对插入当前节点中 */ 
			m_bFlag = FALSE; /* 指示上层节点没有需要插入的键值+子树，插入过程结束 */ 
		} 
		else 
		{   /* 当前节点已满，则分割这个页面并把键值+子树对插入当前节点中 */ 
			SplitNode(pNode, i); /* 继续指示上层节点把返回的键值+子树插入其中 */ 
		}
	}

	void InsertAt(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex,const TKey& Key,const TData& Data) 
	{ 
		int i = 0; 
		/* 把所有大于要插入的键值的键和对应的右子树右移 */ 
		for(i = pNode->m_nCnt; i > nIndex; i--)
		{ 
			pNode-> m_Key[ i ]  = pNode-> m_Key[i-1]; 
			pNode-> m_Data[ i ]  = pNode-> m_Data[i-1]; 
			pNode-> m_pChild[i+1] = pNode-> m_pChild[ i ]; 
		}

		/* 插入键和右子树 */ 
		pNode-> m_Key[ i ]  = Key; 
		pNode-> m_Data[ i ]  = Data;
		pNode-> m_pChild[ i + 1 ] = m_pNewNode; 
		pNode-> m_nCnt ++; 
	}

	void SplitNode( BPlusTreeNode<TKey,TData,TRank>*& pNode, INT nIndex ) 
	{     
		INT i = 0;
		INT j = 0; 
		BPlusTreeNode<TKey,TData,TRank>* pTmpNode = NULL; 
		TKey tmpKey; 
		TData tmpData; 
		/* 建立新节点 */ 
		pTmpNode = new BPlusTreeNode<TKey,TData,TRank>(); 

		if ( nIndex > TRank ) 
		{
			for( i = 2 * TRank - 1, j = TRank - 1; i >= nIndex; i --,j --) 
			{ 
				pTmpNode-> m_Key[j]    = pNode-> m_Key[ i ]; 
				pTmpNode-> m_Data[j]   = pNode-> m_Data[ i ]; 
				pTmpNode-> m_pChild[ j + 1 ] = pNode-> m_pChild[i + 1]; 
			}

			for(i = nIndex - 1, j = nIndex - TRank - 2; j >= 0; i--,j--) 
			{ 
				pTmpNode-> m_Key[j]    = pNode-> m_Key[ i ]; 
				pTmpNode-> m_Data[j]   = pNode-> m_Data[ i ]; 
				pTmpNode-> m_pChild[ j + 1 ] = pNode-> m_pChild[i + 1]; 
			}

			//把节点的最右子树转移成新节点的最左子树
			pTmpNode-> m_pChild[0] = pNode-> m_pChild[ TRank + 1 ];

			//在新节点中插入键和右子树
			pTmpNode-> m_Key[ nIndex - TRank - 1] = m_InsKey; 
			pTmpNode-> m_Data[ nIndex - TRank - 1] = m_InsData; 
			pTmpNode-> m_pChild[ nIndex - TRank ] = m_pNewNode;

			//设置要插入上层节点的键和值 
			m_InsKey = pNode-> m_Key[TRank]; 
			m_InsData = pNode-> m_Data[TRank];

			//tmpKey  = pNode->m_Key[TRank];
			//tmpData = pNode->m_Data[TRank];
		} 
		else 
		{   
			for( i = 2 * TRank - 1,j = TRank - 1; j >= 0; i--,j--) 
			{ 
				pTmpNode-> m_Key[j]   = pNode-> m_Key[ i ]; 
				pTmpNode-> m_Data[j]  = pNode-> m_Data[ i ]; 
				pTmpNode-> m_pChild[j + 1] = pNode-> m_pChild[ i + 1 ]; 
			}

			if (nIndex == TRank) 
			{
				pTmpNode-> m_pChild[0] = m_pNewNode; 
			}
			else 
			{
				pTmpNode-> m_pChild[0] = pNode-> m_pChild[ TRank ]; 
				// 保存要插入上层节点的键和值
				tmpKey = pNode-> m_Key[ TRank - 1]; 
				tmpData = pNode-> m_Data[ TRank - 1];

				// 把所有大于要插入的键值的键和对应的右子树右移
				for(i = TRank - 1; i > nIndex; i --) 
				{ 
					pNode-> m_Key[ i ]   = pNode-> m_Key[i-1]; 
					pNode-> m_Data[ i ]   = pNode-> m_Data[i - 1]; 
					pNode-> m_pChild[ i + 1 ] = pNode-> m_pChild[ i ]; 
				}

				pNode-> m_Key[nIndex]  = m_InsKey; 
				pNode-> m_Data[nIndex]  = m_InsData;

				pNode-> m_pChild[nIndex+1] = m_pNewNode;

				m_InsKey = tmpKey; 
				m_InsData = tmpData; 
			} 
		}

		pNode->m_nCnt = 2*TRank; 
		pTmpNode->m_nCnt= TRank; 
		m_pNewNode  = pTmpNode; 
		m_nNodeCnt ++;

		//pNode = NewNode(pNode,tmpKey,tmpData);
	}


	void Remove( BPlusTreeNode<TKey,TData,TRank>* pNode,const TKey& Key,TData& Data) 
	{ 
		INT i = 0;
		INT j = 0;
		INT k = 0; 
		BPlusTreeNode<TKey,TData,TRank>* pLeft = NULL;
		BPlusTreeNode<TKey,TData,TRank>* pRight = NULL; 
		INT nTmpLevel = 0;

		m_iAccessLevel -- ;

		if ( m_iAccessLevel < 0 ) 
		{ 
			Error(0,Key);  //在整个树中未找到要删除的键
			m_bFlag = FALSE; 
			return; 
		}

		for(i=0, j = pNode->m_nCnt - 1;
			i < j;
			k=( j + i ) / 2, ( Key > pNode-> m_Key[k] ) ? (i = k + 1):( j = k ));

		if ( Key == pNode-> m_Key[ i ] ) 
		{ // 找到要删除的键
			Data = pNode-> m_Data[ i ];

			if (m_iAccessLevel == 0) 
			{  
				DelFromNode(pNode,i);

				m_nKeyCnt -- ; 
				m_bFlag = TRUE; 
				return; 
			} 
			else 
			{   //这个键位于非叶节点
				nTmpLevel = m_iAccessLevel - 1;

				// 找到前驱节点
				pRight = pNode-> m_pChild[ i ];

				while (nTmpLevel > 0)  
				{ 
					pRight = pRight->m_pChild[pRight->m_nCnt]; 
					nTmpLevel--; 
				}

				pNode-> m_Key[ i ]  = pRight-> m_Key[pRight->m_nCnt-1]; 
				pNode-> m_Data[ i ]  = pRight-> m_Data[pRight->m_nCnt-1]; 
				pRight-> m_Data[pRight->m_nCnt-1] = NULL; 
				//Key = pRight-> m_Key[ pRight->m_nCnt - 1 ]; 
			} 
		} 
		else if (Key > pNode-> m_Key[ i ]) 
		{   // i == pNode->m_nCnt-1 时有可能出现
			i++;          
		}

		Remove(pNode-> m_pChild[ i ], Key , Data );

		// 调整平衡
		if ( FALSE == m_bFlag ) 
		{
			return; 
		}

		if (pNode-> m_pChild[ i ]->m_nCnt < TRank) 
		{ 
			if (i == pNode->m_nCnt) 
			{   // 在最右子树中发生了删除
				i--; // 调整最右键的左右子树平衡
			}

			pLeft = pNode-> m_pChild [ i ]; 
			pRight = pNode-> m_pChild[i+1];

			if (pRight->m_nCnt > TRank) 
			{
				MoveLeftNode(pNode,i); 
			}
			else if(pLeft->m_nCnt > TRank) 
			{
				MoveRightNode(pNode,i); 
			}
			else 
			{ 
				JoinNode(pNode,i); 
				// 继续指示上层节点本子树的键数量减少
				return; 
			}

			m_bFlag = FALSE; 
			//指示上层节点本子树的键数量没有减少，删除过程结束 
		} 
	}

	
	void JoinNode(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex) 
	{ 
		BPlusTreeNode<TKey,TData,TRank>* pLeft = NULL;
		BPlusTreeNode<TKey,TData,TRank>* pRight = NULL; 
		INT i = 0;
		INT j = 0;

		pLeft = pNode-> m_pChild[nIndex]; 
		pRight = pNode-> m_pChild[nIndex + 1];

		// 把这个键下移到它的左子树
		pLeft-> m_Key[pLeft->m_nCnt] = pNode-> m_Key[nIndex]; 
		pLeft-> m_Data[pLeft->m_nCnt] = pNode-> m_Data[nIndex];

		// 把右子树中的所有键值和子树转移到左子树
		for (j=pRight->m_nCnt-1,i=pLeft->m_nCnt+pRight->m_nCnt; j >= 0 ; j--,i--) 
		{ 
			pLeft-> m_Key[ i ]  = pRight-> m_Key[j]; 
			pLeft-> m_Data[ i ]  = pRight-> m_Data[j]; 
			pLeft-> m_pChild[ i ] = pRight-> m_pChild[j]; 
		}

		pLeft-> m_pChild[ pLeft->m_nCnt + pRight->m_nCnt + 1 ] = pRight-> m_pChild[ pRight->m_nCnt ]; 
		pLeft->m_nCnt += pRight->m_nCnt + 1;

		delete pRight; 
		// 把这个键右边的键和对应的右子树左移
		for (i=nIndex; i < pNode->m_nCnt-1; i++) 
		{ 
			pNode-> m_Key[ i ] = pNode-> m_Key[i+1]; 
			pNode-> m_Data[ i ] = pNode-> m_Data[i+1]; 
			pNode-> m_pChild[i+1] = pNode-> m_pChild[i+2]; 
		}

		pNode->m_nCnt --; 
		m_nNodeCnt --; 
	}

	

	void MoveLeftNode(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex) 
	{ 
		BPlusTreeNode<TKey,TData,TRank>* pLeft = NULL;
		BPlusTreeNode<TKey,TData,TRank>* pRight = NULL;

		INT k = 0; // 应转移的键的数目
		INT i = 0;
		INT j = 0;

		pLeft  = pNode-> m_pChild[nIndex]; 
		pRight = pNode-> m_pChild[nIndex + 1];

		k = ( pRight->m_nCnt - pLeft->m_nCnt ) / 2;

		// 把这个键下移到它的左子树
		pLeft-> m_Key[pLeft->m_nCnt] = pNode-> m_Key[nIndex]; 
		pLeft-> m_Data[pLeft->m_nCnt] = pNode-> m_Data[nIndex];

		//把右子树的最左子树转移成左子树的最右子树 
		//从右子树向左子树移动 k-1 个键+子树对
		for (j=k-2,i=pLeft->m_nCnt+k-1; j >= 0; j--,i--) 
		{ 
			pLeft-> m_Key[ i ]  = pRight-> m_Key[j]; 
			pLeft-> m_Data[ i ]  = pRight-> m_Data[j]; 
			pLeft-> m_pChild[ i ] = pRight-> m_pChild[j]; 
		}

		pLeft-> m_pChild[ pLeft->m_nCnt + k ] = pRight-> m_pChild[ k - 1 ];

		//把右子树的最左键提升到这个键的位置上
		pNode-> m_Key[nIndex] = pRight-> m_Key[k-1]; 
		pNode-> m_Data[nIndex] = pRight-> m_Data[k-1];

		//把右子树中的所有键值和子树左移 k 个位置
		pRight-> m_pChild[0] = pRight-> m_pChild[k];

		for (i=0; i<pRight->m_nCnt-k; i++) 
		{ 
			pRight-> m_Key[ i ]  = pRight-> m_Key[ i + k ]; 
			pRight-> m_Data[ i ] = pRight-> m_Data[ i + k ]; 
			pRight-> m_pChild[ i ] = pRight-> m_pChild[ i + k ]; 
		}

		pRight-> m_pChild[ pRight->m_nCnt - k ] = pRight-> m_pChild[pRight->m_nCnt]; 
		pLeft->m_nCnt += k; 
		pRight->m_nCnt -= k; 
	}

	

	void MoveRightNode( BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex) 
	{ 
		BPlusTreeNode<TKey,TData,TRank>* pLeft = NULL;
		BPlusTreeNode<TKey,TData,TRank>* pRight = NULL;

		INT k = 0; /* 应转移的键的数目 */ 
		INT i = 0;
		INT j = 0;

		pLeft = pNode-> m_pChild[nIndex]; 
		pRight = pNode-> m_pChild[nIndex + 1];

		k = ( pLeft->m_nCnt - pRight->m_nCnt ) / 2;

		/* 把右子树中的所有键值和子树右移 k 个位置 */ 
		pRight-> m_pChild[ pRight->m_nCnt + k ] = pRight-> m_pChild[ pRight->m_nCnt ];

		for ( i = pRight->m_nCnt - 1; i >= 0; i -- ) 
		{ 
			pRight-> m_Key[i + k]  = pRight-> m_Key[ i ]; 
			pRight-> m_Data[i + k]  = pRight-> m_Data[ i ]; 
			pRight-> m_pChild[ i + k ] = pRight-> m_pChild[ i ]; 
		}

		/* 把这个键下移到它的右子树 */ 
		pRight-> m_Key[k - 1] = pNode-> m_Key[nIndex]; 
		pRight-> m_Data[k - 1] = pNode-> m_Data[nIndex];

		/* 把左子树的最右子树转移成右子树的最左子树 */

		pRight-> m_pChild[k-1] = pLeft-> m_pChild[pLeft->m_nCnt];

		/* 从左子树向右子树移动 k-1 个键+子树对 */ 
		for ( i = pLeft->m_nCnt - 1,j = k - 2; j >= 0; j --,i -- ) 
		{ 
			pRight-> m_Key[j]  = pLeft-> m_Key[ i ]; 
			pRight-> m_Data[j]  = pLeft-> m_Data[ i ]; 
			pRight-> m_pChild[j] = pLeft-> m_pChild[ i ]; 
		}

		/* 把左子树的最右键提升到这个键的位置上 */ 
		pNode-> m_Key[nIndex] = pLeft-> m_Key[ i ]; 
		pNode-> m_Data[nIndex] = pLeft-> m_Data[ i ]; 
		pLeft->m_nCnt -= k; 
		pRight->m_nCnt += k; 
	}

	

	void DelFromNode(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex) 
	{ 
		INT i = 0; 
		/* 把所有大于要删除的键值的键左移 */ 
		for( i = nIndex; i < pNode->m_nCnt - 1; i ++ ) 
		{ 
			pNode-> m_Key[ i ] = pNode-> m_Key[i+1]; 
			pNode-> m_Data[ i ] = pNode-> m_Data[i+1]; 
		}

		pNode->m_nCnt --; 
	}

	

	BPlusTreeNode<TKey,TData,TRank>* NewNode(BPlusTreeNode<TKey,TData,TRank>* pNode,const TKey& Key,const TData& Data) 
	{   /* 在新的层上加入节点 */
		BPlusTreeNode<TKey,TData,TRank>* pTmpNode = NULL; 
		pTmpNode = new BPlusTreeNode<TKey,TData,TRank>(); 
		pTmpNode-> m_nCnt = 1; 
		pTmpNode-> m_pChild[0] = pNode;    //指向前结点
		pTmpNode-> m_pChild[1] = m_pNewNode; 
		pTmpNode-> m_Key[0]    = Key; 
		pTmpNode-> m_Data[0]   = Data; 
		m_iHeight ++; 
		m_nNodeCnt ++;

		return( pTmpNode ); 
	}

	
	BPlusTreeNode<TKey,TData,TRank>* FreeNode(BPlusTreeNode<TKey,TData,TRank>* pNode) 
	{ 
		BPlusTreeNode<TKey,TData,TRank>* pTmpNode = pNode-> m_pChild[0];
		delete pNode; 
		m_iHeight --; 
		m_nNodeCnt --; 
		return pTmpNode; 
	}

	void Error(int f,TKey Key) 
	{ 
		//自定义的出错处理代码
	}



	void DelAll(BPlusTreeNode<TKey,TData,TRank>* pNode) 
	{ 
		INT i = 0; 
		m_iAccessLevel --;

		if ( m_iAccessLevel > 0 )
		{
			for ( i = 0; i <= pNode->m_nCnt ; i ++ ) 
			{
				DelAll( pNode-> m_pChild[ i ] ); 
			}
		}

		delete pNode;

	} 
public:
	BOOL Add(const TKey& Key,const TData& Data) 
	{ 
		m_iAccessLevel = m_iHeight;

		Insert( m_pRoot, Key, Data );

		if ( m_bFlag )
		{
			m_pRoot = NewNode(m_pRoot,Key,Data);    /* 树的高度增加 */ 
		}

		return TRUE; 
	}

	BOOL Del( const TKey& Key, TData& Data ) 
	{ 
		m_iAccessLevel = m_iHeight;

		Remove( m_pRoot, Key, Data );

		if ( 0 == m_pRoot->m_nCnt ) 
		{
			m_pRoot = FreeNode( m_pRoot ); 
		}

		return TRUE; 
	}

	INT GetHeight() 
	{ 
		return m_iHeight; 
	}

	INT GetKeys() 
	{ 
		return m_nKeyCnt; 
	}

	double GetPayload() 
	{ 
		if (m_nNodeCnt==0) 
		{
			return 1; 
		}
		return (double) m_nKeyCnt / (double)( m_nNodeCnt *( 2 * TRank ) ); 
	}

	BPlusTreeNode<TKey,TData,TRank>* Destroy () 
	{     
		m_iAccessLevel = m_iHeight; 
		m_iHeight      = 0;

		return Delall(m_pRoot);

	} 
public:

	POSITION Search(const TKey& Key, TData& Data) 
	{ 
		INT i = 0;
		INT j = 0;
		INT k = 0;

		INT nLevel = m_iHeight;
		BPlusTreeNode<TKey,TData,TRank>* pNode = m_pRoot;

		while ( nLevel >= 0 && pNode != NULL)
		{
			for( i = 0, j = pNode->m_nCnt - 1; 
				i < j; 
				k = ( j + i ) / 2, ( Key > pNode->m_Key[ k ] ) ? ( i = k + 1) : ( j = k ) );

			if ( Key == pNode->m_Key [ i ] )
			{   //找到
				Data = pNode->m_Data[i];
				return (POSITION) &pNode->m_Key[i]; 
			}

			if ( Key > pNode->m_Key [ i ] )
			{//节点在右侧
				i ++; 
			}
            
			pNode = pNode->m_pChild[ i ];
			nLevel --; //到下层查找
		}
		Data = -1;
		return NULL; 
	}
    
	void RangeSearch(int f,int t,int *a)
	{
		int i;
		int temp=0;

		for(i=0;f<=t;f++)
		{
			Search(f,temp);
			a[i]=temp;
			i++;
		}
	}

	BPlusTreeNode<TKey,TData,TRank>* GetRoot()
	{
		return m_pRoot;
	}

	//void Show2Tree(CTreeCtrl*pTree,HTREEITEM hItem,BPlusTreeNode<TKey,TData,TRank>*pNode)
	//{
	//    if( NULL == pNode )
	//    {
	//        return;
	//    }


	//    HTREEITEM hTmp = NULL;
	//    CString strTmp;

	//    for( INT i = 0; i < pNode->m_nCnt; i ++ )
	//    {
	//        if( 0 == i )
	//        {
	//            hTmp = pTree->InsertItem(_T("Child(Start)"),hItem);
	//            Show2Tree(pTree,hTmp,pNode->m_pChild[0]);
	//        }
	//        strTmp.Format(_T("Key(%d)"),pNode->m_Key[i]);

	//        hTmp = pTree->InsertItem(strTmp,hItem);
	//        Show2Tree(pTree,hTmp,pNode->m_pChild[i+1]);
	//    }
	//}
public:   //APP
	void SingleSearch()  //单独查询一个值
	{
		int key=0;
		int data=0;
		cout<<"Please input the Phone number: ";
		cin>>key;
		Search(key,data);
		if (data >= 0) cout<<"The position is: "<<data<<endl;
		else cout<<"Data Not Found."<<endl;
	}

	void RangeSearchApp()  //范围查询
	{
		int from=0;
		int to=0;
		int i;

		cout<<"Please input begin number:";
        cin>>from;
		cout<<"Please input end number:";
		cin>>to;

        int temp1,temp2;
		temp1=from;
		temp2=to;

		int *range=new int[to-from];

		RangeSearch(from,to,range);

		from=temp1;
		to=temp2;
		for(i=0;from<=to;from++)
		{
			if( range[i]!=-1 )
				cout<<"Phone number:"<<from<<" "<<"Phone position:"<<range[i]<<endl;
			else
				cout<<"Phone Number "<<from<<" Not Found..."<<endl;
			i++;
		}

	}

	void DeleteApp()   
	{
		int key=0;
		int data=0;
		BOOL flag=FALSE;
		cout<<"Please input the Phone number you want to delete:";
		cin>>key;
		flag=Del(key,data);
		if(flag)
			cout<<"Delete success."<<endl;
		else
			cout<<"Delete fail."<<endl;
	}

	void OutputBPTree()
	{
		ofstream outputFile;
		outputFile.open("Disk.txt",ios::out);

		BPlusTreeNode<TKey,TData,TRank> *TempPtr;

		TempPtr=this->m_pRoot;
		
	}
};



#endif