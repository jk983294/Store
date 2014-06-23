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
	INT m_nCnt;    //��ǰ�ڵ��е�Key����
	TKey  m_Key [2 * TRank]; //Key����
	TData m_Data[2 * TRank]; //Data����

	BPlusTreeNode<TKey,TData,TRank>* m_pChild[ 2 * TRank + 1 ]; //ָ���ӵ�ָ������
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
	BPlusTreeNode<TKey,TData,TRank>* m_pRoot; //�����ڵ�
	INT  m_iHeight;        //���߶�
	INT m_nKeyCnt;        //������
	INT m_nNodeCnt;       //�ڵ�����
protected:
	INT m_nSearchIndex;       // ����ʱ�ҵ��ļ��ڽڵ��е�λ��

	TData m_InsData;        // ��Ҫ��ļ����Ӧ��ֵ
	TKey m_InsKey;        /// Ҫ����ļ�

	INT  m_iAccessLevel;       // ��ǰ���ʵĽڵ������ĸ߶� 
	BPlusTreeNode<TKey,TData,TRank>* m_pNewNode; // �ڽڵ�ָ��ʱ��ָ���½��Ľڵ�

	BOOL m_bFlag;         // �ڵ�������־
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
		//����һ��0�ڵ������
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
			Error(1,Key); /* ���Ѿ������� */ 
			m_bFlag = FALSE; 
			return; 
		}

		if ( Key > pNode-> m_Key[ i ] ) 
		{   /* i == pNode->m_nCnt - 1 ʱ�п��ܳ��� */ 
			i ++; 
		}

		//���ӽڵ����
		Insert( pNode -> m_pChild[ i ] , Key, Data );

		if ( FALSE == m_bFlag ) 
		{
			return; 
		}

		/* ���¼�Ҫ���뵽��ǰ�ڵ��� */ 
		if ( pNode->m_nCnt < 2 * TRank ) 
		{   /* ��ǰ�ڵ�δ�� */ 
			InsertAt( pNode, i ,Key,Data); /* �Ѽ�ֵ+�����Բ��뵱ǰ�ڵ��� */ 
			m_bFlag = FALSE; /* ָʾ�ϲ�ڵ�û����Ҫ����ļ�ֵ+������������̽��� */ 
		} 
		else 
		{   /* ��ǰ�ڵ���������ָ����ҳ�沢�Ѽ�ֵ+�����Բ��뵱ǰ�ڵ��� */ 
			SplitNode(pNode, i); /* ����ָʾ�ϲ�ڵ�ѷ��صļ�ֵ+������������ */ 
		}
	}

	void InsertAt(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex,const TKey& Key,const TData& Data) 
	{ 
		int i = 0; 
		/* �����д���Ҫ����ļ�ֵ�ļ��Ͷ�Ӧ������������ */ 
		for(i = pNode->m_nCnt; i > nIndex; i--)
		{ 
			pNode-> m_Key[ i ]  = pNode-> m_Key[i-1]; 
			pNode-> m_Data[ i ]  = pNode-> m_Data[i-1]; 
			pNode-> m_pChild[i+1] = pNode-> m_pChild[ i ]; 
		}

		/* ������������� */ 
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
		/* �����½ڵ� */ 
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

			//�ѽڵ����������ת�Ƴ��½ڵ����������
			pTmpNode-> m_pChild[0] = pNode-> m_pChild[ TRank + 1 ];

			//���½ڵ��в������������
			pTmpNode-> m_Key[ nIndex - TRank - 1] = m_InsKey; 
			pTmpNode-> m_Data[ nIndex - TRank - 1] = m_InsData; 
			pTmpNode-> m_pChild[ nIndex - TRank ] = m_pNewNode;

			//����Ҫ�����ϲ�ڵ�ļ���ֵ 
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
				// ����Ҫ�����ϲ�ڵ�ļ���ֵ
				tmpKey = pNode-> m_Key[ TRank - 1]; 
				tmpData = pNode-> m_Data[ TRank - 1];

				// �����д���Ҫ����ļ�ֵ�ļ��Ͷ�Ӧ������������
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
			Error(0,Key);  //����������δ�ҵ�Ҫɾ���ļ�
			m_bFlag = FALSE; 
			return; 
		}

		for(i=0, j = pNode->m_nCnt - 1;
			i < j;
			k=( j + i ) / 2, ( Key > pNode-> m_Key[k] ) ? (i = k + 1):( j = k ));

		if ( Key == pNode-> m_Key[ i ] ) 
		{ // �ҵ�Ҫɾ���ļ�
			Data = pNode-> m_Data[ i ];

			if (m_iAccessLevel == 0) 
			{  
				DelFromNode(pNode,i);

				m_nKeyCnt -- ; 
				m_bFlag = TRUE; 
				return; 
			} 
			else 
			{   //�����λ�ڷ�Ҷ�ڵ�
				nTmpLevel = m_iAccessLevel - 1;

				// �ҵ�ǰ���ڵ�
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
		{   // i == pNode->m_nCnt-1 ʱ�п��ܳ���
			i++;          
		}

		Remove(pNode-> m_pChild[ i ], Key , Data );

		// ����ƽ��
		if ( FALSE == m_bFlag ) 
		{
			return; 
		}

		if (pNode-> m_pChild[ i ]->m_nCnt < TRank) 
		{ 
			if (i == pNode->m_nCnt) 
			{   // �����������з�����ɾ��
				i--; // �������Ҽ�����������ƽ��
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
				// ����ָʾ�ϲ�ڵ㱾�����ļ���������
				return; 
			}

			m_bFlag = FALSE; 
			//ָʾ�ϲ�ڵ㱾�����ļ�����û�м��٣�ɾ�����̽��� 
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

		// ����������Ƶ�����������
		pLeft-> m_Key[pLeft->m_nCnt] = pNode-> m_Key[nIndex]; 
		pLeft-> m_Data[pLeft->m_nCnt] = pNode-> m_Data[nIndex];

		// ���������е����м�ֵ������ת�Ƶ�������
		for (j=pRight->m_nCnt-1,i=pLeft->m_nCnt+pRight->m_nCnt; j >= 0 ; j--,i--) 
		{ 
			pLeft-> m_Key[ i ]  = pRight-> m_Key[j]; 
			pLeft-> m_Data[ i ]  = pRight-> m_Data[j]; 
			pLeft-> m_pChild[ i ] = pRight-> m_pChild[j]; 
		}

		pLeft-> m_pChild[ pLeft->m_nCnt + pRight->m_nCnt + 1 ] = pRight-> m_pChild[ pRight->m_nCnt ]; 
		pLeft->m_nCnt += pRight->m_nCnt + 1;

		delete pRight; 
		// ��������ұߵļ��Ͷ�Ӧ������������
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

		INT k = 0; // Ӧת�Ƶļ�����Ŀ
		INT i = 0;
		INT j = 0;

		pLeft  = pNode-> m_pChild[nIndex]; 
		pRight = pNode-> m_pChild[nIndex + 1];

		k = ( pRight->m_nCnt - pLeft->m_nCnt ) / 2;

		// ����������Ƶ�����������
		pLeft-> m_Key[pLeft->m_nCnt] = pNode-> m_Key[nIndex]; 
		pLeft-> m_Data[pLeft->m_nCnt] = pNode-> m_Data[nIndex];

		//������������������ת�Ƴ����������������� 
		//�����������������ƶ� k-1 ����+������
		for (j=k-2,i=pLeft->m_nCnt+k-1; j >= 0; j--,i--) 
		{ 
			pLeft-> m_Key[ i ]  = pRight-> m_Key[j]; 
			pLeft-> m_Data[ i ]  = pRight-> m_Data[j]; 
			pLeft-> m_pChild[ i ] = pRight-> m_pChild[j]; 
		}

		pLeft-> m_pChild[ pLeft->m_nCnt + k ] = pRight-> m_pChild[ k - 1 ];

		//����������������������������λ����
		pNode-> m_Key[nIndex] = pRight-> m_Key[k-1]; 
		pNode-> m_Data[nIndex] = pRight-> m_Data[k-1];

		//���������е����м�ֵ���������� k ��λ��
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

		INT k = 0; /* Ӧת�Ƶļ�����Ŀ */ 
		INT i = 0;
		INT j = 0;

		pLeft = pNode-> m_pChild[nIndex]; 
		pRight = pNode-> m_pChild[nIndex + 1];

		k = ( pLeft->m_nCnt - pRight->m_nCnt ) / 2;

		/* ���������е����м�ֵ���������� k ��λ�� */ 
		pRight-> m_pChild[ pRight->m_nCnt + k ] = pRight-> m_pChild[ pRight->m_nCnt ];

		for ( i = pRight->m_nCnt - 1; i >= 0; i -- ) 
		{ 
			pRight-> m_Key[i + k]  = pRight-> m_Key[ i ]; 
			pRight-> m_Data[i + k]  = pRight-> m_Data[ i ]; 
			pRight-> m_pChild[ i + k ] = pRight-> m_pChild[ i ]; 
		}

		/* ����������Ƶ����������� */ 
		pRight-> m_Key[k - 1] = pNode-> m_Key[nIndex]; 
		pRight-> m_Data[k - 1] = pNode-> m_Data[nIndex];

		/* ������������������ת�Ƴ����������������� */

		pRight-> m_pChild[k-1] = pLeft-> m_pChild[pLeft->m_nCnt];

		/* �����������������ƶ� k-1 ����+������ */ 
		for ( i = pLeft->m_nCnt - 1,j = k - 2; j >= 0; j --,i -- ) 
		{ 
			pRight-> m_Key[j]  = pLeft-> m_Key[ i ]; 
			pRight-> m_Data[j]  = pLeft-> m_Data[ i ]; 
			pRight-> m_pChild[j] = pLeft-> m_pChild[ i ]; 
		}

		/* �������������Ҽ��������������λ���� */ 
		pNode-> m_Key[nIndex] = pLeft-> m_Key[ i ]; 
		pNode-> m_Data[nIndex] = pLeft-> m_Data[ i ]; 
		pLeft->m_nCnt -= k; 
		pRight->m_nCnt += k; 
	}

	

	void DelFromNode(BPlusTreeNode<TKey,TData,TRank>* pNode, INT nIndex) 
	{ 
		INT i = 0; 
		/* �����д���Ҫɾ���ļ�ֵ�ļ����� */ 
		for( i = nIndex; i < pNode->m_nCnt - 1; i ++ ) 
		{ 
			pNode-> m_Key[ i ] = pNode-> m_Key[i+1]; 
			pNode-> m_Data[ i ] = pNode-> m_Data[i+1]; 
		}

		pNode->m_nCnt --; 
	}

	

	BPlusTreeNode<TKey,TData,TRank>* NewNode(BPlusTreeNode<TKey,TData,TRank>* pNode,const TKey& Key,const TData& Data) 
	{   /* ���µĲ��ϼ���ڵ� */
		BPlusTreeNode<TKey,TData,TRank>* pTmpNode = NULL; 
		pTmpNode = new BPlusTreeNode<TKey,TData,TRank>(); 
		pTmpNode-> m_nCnt = 1; 
		pTmpNode-> m_pChild[0] = pNode;    //ָ��ǰ���
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
		//�Զ���ĳ��������
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
			m_pRoot = NewNode(m_pRoot,Key,Data);    /* ���ĸ߶����� */ 
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
			{   //�ҵ�
				Data = pNode->m_Data[i];
				return (POSITION) &pNode->m_Key[i]; 
			}

			if ( Key > pNode->m_Key [ i ] )
			{//�ڵ����Ҳ�
				i ++; 
			}
            
			pNode = pNode->m_pChild[ i ];
			nLevel --; //���²����
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
	void SingleSearch()  //������ѯһ��ֵ
	{
		int key=0;
		int data=0;
		cout<<"Please input the Phone number: ";
		cin>>key;
		Search(key,data);
		if (data >= 0) cout<<"The position is: "<<data<<endl;
		else cout<<"Data Not Found."<<endl;
	}

	void RangeSearchApp()  //��Χ��ѯ
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