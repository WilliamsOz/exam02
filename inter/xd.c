#include <unistd.h>

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int main(int ac, char **av)
{
	int	tab[127];
	int	i;
	char	*str1;
	char	*str2;
	int	temp;


	if (ac != 3)
	{
		ft_putchar('\n');
		return (0);
	}
	i = -1;
	while (++i < 127)
		tab[i] = 0;
	i = 0;
	str1 = av[1];
	str2 = av[2];
	while (str2[i])
	{
		temp = str2[i];
		tab[temp] = 1;
		i++;
	}
	i = 0;
	while (str1[i])
	{
		temp = str1[i];
		if (tab[temp] == 1)
		{
			ft_putchar(str1[i]);
			tab[temp] = 42;
		}
		i++;
	}
	ft_putchar('\n');
	return (0);
}
