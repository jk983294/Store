/***********************************
*Combinations And Arrangements
*
*Xiaojun_0237 Original Author 2007-2008
*E-mail:xiaojun_0237@yahoo.com.cn
*	
************************************/
/********************************************************************************************************
*组合与排列生成算法
*
*小钧(赵政宇) 原始作者 2007-2008 
*
*这个算法的复制和分发遵循GPL通用公共许可，你可以对其进行复制，修改再分发，但请不要删除这段文字，并欢迎
*你将你的改动再发到互联网上，与原作者及更多的人交流。
*
*作者邮箱：xiaojun_0237@yahoo.com.cn
*********************************************************************************************************/
# ifndef _CA_H
# define _CA_H
# include <stdio.h>
# include <string.h>

/******************************************************************************************************************
*Function Name : Combinations																					  
*Parameters:	char s[]							用于生成组合的元素序列
				int n								元素总数																				   
				int k								从n个元素中取出的元素个数 , k <= n
				void (*output)(char s[], int C[], unsigned l)	输出每一组生成的组合序列的函数,其中s[]为要输出内容的序列,
																C[]为参考序列,s[]中元素的组合方式由C来确定, l为C序列的长度
*Return Value:	int ERRORCODE	C(n,k)	正常生成所有组合,返回输出的所有组合的数目,(注：C(n,k) 不得超过 2147483648)
								-1		元素总数目为负
								-2		所取元素数目非法
								-3		n 大于s序列中所含元素个数
*Discriptions:	执行结果是在屏幕上显示 从n个元素中取出k个元素的所有组合,输出格式由函数指针output指向的回调函数确定
				特殊情况: k = 0 或 k > n 时 函数自动退出,屏幕上什么都不显示
******************************************************************************************************************/
int Combinations(char s[], int n, int k, int (*output)(char s[], int C[], int l))		//输出 n个元素中取k个 的所有组合
{
	if (n <= 0) return -1;
	if (k <= 0 || k > n) return -2;
	if (n > (int)strlen(s)) return -3;
	//当输入的 n, k组合没有意义时函数直接退出,并返回错误码

	int *C = new int[k];
	//C为输出组合的参考序列, 算法中C被当作堆栈使用
	int top = -1, tmp = 1;
	//堆栈的栈顶指针top, tmp临时保存栈顶弹出的数据
	int counter = 0, unit;

	//***********算法开始***************
	//这个循环产生第一个组合:1234...k
	do
	{
		C[++top] = tmp;
		tmp++; 
	}while(top < k - 1);
	//输出这个组合
	if (top == k - 1)
	{
		unit = output(s, C, k);
		//输出一个组合，计数器计一次数
		counter +=unit;
	}
	//下面这个do循环中产生并输出剩下的所有组合
	do
	{
		if (top == k - 1)
			do
			{
				tmp = C[top--];
			}while(tmp > n - (k - (top + 1))  && top > -1);
			//上面这个if语句的意思是  
			//如果栈满的话不断弹栈,直到 弹出来的元素小于等于该位置上允许的最大元素
			//当所有的组合都生成完时这一句会将栈弹空
			//just follow it step by step
		if (tmp <= n - (k - (top + 1))) //如果位于栈顶的元素还可以再大
			do
			{
				tmp++;
				C[++top] = tmp;
			}while(top < k - 1);  //当栈未满
			//这一句是将上面弹的半空的栈重新压满,压满的新栈就是新生成的一个组合
		if (top == k - 1)
		{
			output(s, C, k);
			//输出一个组合，计数器计一次数
			counter += unit;
		}
		//如果栈满则输出这个组合,当输出完所有组合后栈会弹空
	}while(top > -1); //当所有组合还未生成完
	delete []C;
	return counter;
}

int output(char s[], int C[], int l)
{ 
	int i;
	printf("%c", s[C[0] - 1]);
	for (i = 1; i < l; i++) 
	{
		printf(" %c", s[C[i] - 1]);
	}
	printf("\n");
	return 1;
}
////////////////////////////////////////////////////////////////////
int output(char s[], int C[], int A[], int l)
{
	register int mapper1, mapper2; 
	mapper1 = A[0] - 1;
	mapper2 = C[mapper1] - 1;
	printf("%c", s[mapper2]);
	for (int i = 1; i < l; i++)
	{
		mapper1 = A[i] - 1;
		mapper2 = C[mapper1] -1;
		printf(" %c", s[mapper2]);
	}
	printf("\n");
	return 1;
}

/******************************************************************************************************************
*Function Name: Arrangements																					  
*Parameters:	char s[]							用于生成排列的元素序列
				int C[]								用于参考的组合序列																				   
				int n								从所有元素中取出的元素个数
*Return Value:	int ERRORCODE	A(n,n)	正常生成所有排列, 返回生成的所有的数目(注：A(n,k)不得超过 2147483648)
*Discriptions:	执行结果是在屏幕上显示 从n个元素中取出k个元素的所有排列
******************************************************************************************************************/
int Arrangements(char s[], int C[], int n)
{		
	int *A = new int[n], top = -1;
	//A为输出排列的参考序列，算法中A被当作栈使用，top为栈顶指针 
	int *oc = new int[n], ltop = n, curElem = ltop, Elements = 0;
	//oc(Ordered Container)，有序容器，也当作栈使用，但为保证栈中元素总是按从小到大的顺序弹出及充分利用空间,比栈要多一个“交换”和“追加”运算
	int tmp, transit;
	//算法中用到的临时变量
	int counter = 0;
	//返回生成的排列总数的计数器
	//************算法开始********************
	//这个循环产生第一组排列123...n
	tmp = 1;  
	do
	{
		A[++top] = tmp;
		tmp++;
	}while(top < n - 1);
	//输出生成的组合并计数
	if (top >= n - 1)
	{	
		output(s, C, A, n); 
		counter++;
	}
	//下面这个do循环中生产并输出剩下的所有组合
	tmp = -1, transit = -1;
	do
	{
		//这个的if语句将刚刚已生成的一个排列的末几个已完全呈逆序的元素弹出，
		// 压入“有序容器”，压入时做调整，保证有序容器从栈顶到栈底元素的顺序是从小到大
		if (top >= n - 1)
		{
			tmp = A[top--];
			if (Elements == 0 || curElem != ltop)
			{
				do
				{
					//如果有序容器为空则直接将弹出的元素压入oc
					if (Elements == 0)
					{
						ltop = n;curElem = ltop;	//必要指针归位
						//压栈动作
						ltop--;					//向下生长的堆栈
						ltop = (ltop + n) % n;	//空间循环利用
						oc[ltop] = tmp;
						curElem = ltop;
						Elements++;
						//弹出元素已被操作的标记
						tmp = -1;
					}
					//否则将弹出的元素追加到栈底（因为这时弹出无素是最大的）
					else
					{
						//追加动作
						curElem++;
						curElem %= n;	//空间循环利用
						oc[curElem] = tmp;
						Elements++;
						//弹出元素已被操作的标记
						tmp = -1;
					}
					//继续弹出A中元素
					if (top > -1) tmp = A[top--];
				}while(tmp != -1 && tmp > oc[curElem]);	//循环,将"完全逆序"元素全部送入有序容器
				curElem = ltop;	//动作结束后指针归位
			}
		}
		//下面的if语句开始将有序容器中的元素按一定规则重新压入栈A,生成新的排列
		if (tmp != -1)	//如果栈中元素不是完全按从大到小的顺序排列(如果是说明已生成所有排列)则生成新的排列
		{
			//下面的两分支都是将刚从A栈顶元素与oc中比它大的最小元素交换
			if (tmp < oc[ltop])	//如果比它大的最小元素就在oc栈顶
			{
				//弹栈动作
				transit = oc[ltop];
				ltop++;		//向下生长的堆栈
				ltop %= n;	//空间循环利用  
				Elements--;
				curElem = ltop; //必要指针归位
				//将弹出的元素压出A
				A[++top] = transit;
				//将A中刚刚弹出的元素压入oc
				ltop--;		//向下生长的堆栈
				ltop = (ltop + n) % n;	//空间循环利用
				oc[ltop] = tmp;	
				Elements++;
				//如果A栈未满则做下面的循环将A栈压满
				while (top < n - 1)
				{
					//弹oc动作
					transit = oc[ltop];
					ltop++;		//向下生长的堆栈
					ltop %= n;	//空间循环利用
					Elements--;
					curElem = ltop;	//必要指针归位
					A[++top] = transit;	//oc中弹出的元素压入栈中
				}
			}
			//如果oc中的比tmp大的最小元素不是栈顶
			else
			//The codes below, especially that is in the circular block is difficult to understand.
			{
				//顺序查找oc中的元素直到找到比A栈顶大的元素
				do
				{
					curElem++;	//顺序查找下一个
					curElem %= n;	//空间循环利用
					if (tmp < oc[curElem])	//如果找到
					{
						//进行交换
						transit = oc[curElem];
						oc[curElem] = tmp;
						tmp = transit;
						A[++top] = tmp;
						//A中刚弹出的元素已被操作的标记
						tmp = -1;
						//如果A栈未满则循环,将A栈压满
						while (top < n - 1)
						{
							//弹栈动作
							transit = oc[ltop];
							ltop++;	//向下生长的堆栈
							ltop %= n;	//空间循环利用
							Elements--;
							//弹出元素压入A栈
							A[++top] = transit;
						}
					}
				}while((curElem + 1) % n != ltop && tmp > oc[curElem]);	//当目前未找到且未找完oc中所有元素(这个判断条件不是很稳定,但一般不会出错)
			}
			//至此生成新排列的工作结束
			//如果新排列生成则输出并计数
			if (top >= n - 1)
			{
				output(s, C, A, n);
				counter++;
			}
		}
	}while(top > -1);

	delete []A;
	delete []oc;
	return counter;
}


# endif