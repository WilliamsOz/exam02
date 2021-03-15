
#include <stdio.h>

int	ft_printf(const char *src, ...);

int main(void)
{
	int mine = 0;
	int real = 0;
	mine = ft_printf("|%5.1s|", "toto     ");
	printf("\n");
	real = printf("|%5.1s|", "toto     ");
	printf("\n");
	printf("mine = %d\n", mine);
	printf("real : %d\n", real);
	return (0);
}
