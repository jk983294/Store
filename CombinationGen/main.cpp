# include "ca.h"
# include <string.h>
int main(void)
{
	char s[] = "abcdefghijklmnopqrstuvwxyz";
	int n, k;
	char choice;
	printf("生成组合（C）或生成排列（A）\n");
	printf("C or A?");	
	scanf("%c", &choice);
	printf("n个元素中取出k个的组合（排列），请输入n和k,中间用空格分隔: ");
	scanf("%d%d", &n, &k);
	if (choice == 'a' || choice == 'A')
	{
		k = Combinations(s, n, k, Arrangements);
		printf("Arrangements: %d\n", k);
	}
	else
	{
		k = Combinations(s, n, k, output);
		printf("Combinatios: %d\n", k);
	}
	return 0;
}