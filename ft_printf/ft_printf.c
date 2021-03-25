/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 14:47:15 by wiozsert          #+#    #+#             */
/*   Updated: 2021/03/25 11:49:44 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct	s_data
{
	int			width;
	int			precision;
	long		l_arg;
	char		*c_str;
	char		conv;
}				t_data;

static int		ft_strlen(const char *str)
{
	int		i;

	i = 0;
	if (str == NULL)
		return (i);
	while (str[i] != '\0')
		i++;
	return (i);
}

static void		ft_putchar(char c)
{
	write(1, &c, 1);
}

static int		ft_atoi(const char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res);
}

static t_data	get_conv(const char *str, t_data data)
{
	int		i;

	i = 0;
	while (str[i] != 's' && str[i] != 'd' && str[i] != 'x')
		i++;
	data.conv = str[i];
	return (data);
}

static t_data	get_width(const char *str, t_data data)
{
	data.width = ft_atoi(str);
	return (data);
}

static t_data	get_precision(const char *str, t_data data)
{
	int		i;

	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != '.')	
		data.precision = -1;
	else
	{
		while (str[i] == '.')
			i++;
		data.precision = ft_atoi(str + i);
	}
	return (data);
}

static char		*ft_itoa_base(long nbr, char *base)
{
	int		sign;
	char	*dest;
	long	temp;
	int		len;

	if (nbr == 0)
	{
		if (!(dest = (char*)malloc(sizeof(char) * 2)))
			return (NULL);
		dest[0] = '0';
		dest[1] = '\0';
		return (dest);
	}
	sign = 1;
	len = 0;
	if (nbr < 0)
	{
		sign = -1;
		nbr *= -1;
		len++;
	}
	temp = nbr;
	while (temp > 0)
	{
		len++;
		temp /= ft_strlen(base);
	}
	if (!(dest = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	dest[len--] = '\0';
	while (nbr > 0)
	{
		dest[len] = base[nbr % ft_strlen(base)];
		len--;
		nbr /= ft_strlen(base);
	}
	if (sign == -1)
		dest[len] = '-';
	return (dest);
}

static int		treat_s(t_data data, int len)
{
	int		c_len;
	int		i;

	c_len = ft_strlen(data.c_str);
	if (data.precision < c_len && data.precision != -1)
		c_len = data.precision;
	while (data.width > c_len)
	{
		ft_putchar(' ');
		len++;
		data.width--;
	}
	i = 0;
	while (c_len > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		len++;
		c_len--;
	}
	return (len);
}

static int		treat_dx(t_data data, int len)
{
	int		c_len;
	int		i;

	c_len = ft_strlen(data.c_str);
	if (data.l_arg == 0 && data.precision == 0)
		c_len = 0;
	if (data.l_arg < 0 && data.precision > c_len)
	{	
		data.width--;
		c_len--;
	}
	while (data.width > c_len && data.width > data.precision)
	{
		ft_putchar(' ');
		data.width--;
		len++;
	}
	i = 0;
	if (data.l_arg < 0 && data.precision > c_len)
	{
		ft_putchar('-');
		i++;
	}
	while (data.precision > c_len)
	{
		ft_putchar('0');
		len++;
		data.precision--;
	}
	while (c_len > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		len++;
		c_len--;
	}
	return (len);
}

static int		get_content(const char *str, va_list list, int len)
{
	t_data	data;

	data.conv = '\0';
	data = get_conv(str, data);
	data = get_width(str, data);
	data = get_precision(str, data);

	if (data.conv == 's')
	{
		data.c_str = va_arg(list, char *);
		if (data.c_str == NULL)
			data.c_str = "(null)";
		len = treat_s(data, len);
	}
	else
	{
		if (data.conv == 'd')
		{
			data.l_arg = va_arg(list, int);
			data.c_str = ft_itoa_base(data.l_arg, "0123456789");
		}
		else
		{
			data.l_arg = (unsigned int)va_arg(list, unsigned int);
			data.c_str = ft_itoa_base(data.l_arg, "0123456789abcdef");
		}
		len = treat_dx(data, len);
		free(data.c_str);
	}
	return (len);
}

int				ft_printf(const char *str, ...)
{
	int		i;
	int		len;
	va_list	list;

	i = 0;
	len = 0;
	va_start(list, str);
	while (str[i] != '\0')
	{
		if (str[i] != '%')
		{
			ft_putchar(str[i]);
			len++;
			i++;
		}
		else
		{
			len = get_content(str + i + 1, list, len);
			while (str[i] != 's' && str[i] != 'd' && str[i] != 'x')
				i++;
			i++;
		}
	}
	va_end(list);
	return (len);
}

