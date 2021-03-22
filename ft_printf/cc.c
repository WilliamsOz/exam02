
#include <stdio.h>

int	ft_printf(const char *src, ...);

#include <libc.h>

int main(void)
{
	int mine = 0;
	int real = 0;
	mine = ft_printf("|%1.0x|\n", (int)-2147483648);
	real = printf("|%1.0x|\n", (int)-2147483648);
	printf("mine = %d\n", mine);
	printf("real : %d\n", real);
	return (0);
}
