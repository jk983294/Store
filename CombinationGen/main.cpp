# include "ca.h"
# include <string.h>
int main(void)
{
	char s[] = "abcdefghijklmnopqrstuvwxyz";
	int n, k;
	char choice;
	printf("������ϣ�C�����������У�A��\n");
	printf("C or A?");	
	scanf("%c", &choice);
	printf("n��Ԫ����ȡ��k������ϣ����У���������n��k,�м��ÿո�ָ�: ");
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