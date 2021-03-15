/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 16:06:17 by wiozsert          #+#    #+#             */
/*   Updated: 2021/03/15 16:21:45 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void		ft_putchar(char c)
{
	write(1, &c, 1);
}

static void		inter(char *s1, char *s2)
{
	int		i;
	int		tab[127];
	int		temp;

	i = -1;
	while (++i < 127)
		tab[i] = 0;
	i = 0;
	while (s2[i] != '\0')
	{
		temp = s2[i];
		tab[temp]++;
		i++;
	}
	i = 0;
	while (s1[i] != '\0')
	{
		temp = s1[i];
		if (tab[temp] > 0)
		{
			ft_putchar(s1[i]);
			tab[temp] = 0;
		}
		i++;
	}
	ft_putchar('\n');
}

int main(int ac, char **av)
{
	if (ac != 3)
		ft_putchar('\n');
	else
		inter(av[1], av[2]);
	return (0);
}
