/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 16:28:57 by wiozsert          #+#    #+#             */
/*   Updated: 2021/03/16 09:36:07 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#include <stdio.h>
#define PRINTD(x) printf("%d\n", x);
#define PRINTS(x) printf("%s\n", x);
#define PRINTX(x) printf("%x\n", x);
#define DEBUG printf("DEBUG\n");

typedef struct		s_data
{
	int		width;
	int		precision;
	long	l_arg;
	char	conv;
	char	*c_str;
}					t_data;

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

static int	ft_strlen(const char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
		i++;
	return (i);
}

static int		ft_atoi(const char *src)
{
	int		i;
	long	res;
	
	i = 0;
	res = 0;
	while (src[i] >= '0' && src[i] <= '9')
	{
		res = res * 10 + src[i] - '0';
		i++;
	}
	return (res);
}

static t_data	get_width(const char *src, t_data data)
{
	data.width = ft_atoi(src);
	return (data);
}

static t_data	get_precision(const char *src, t_data data)
{
	int		i;

	i = 0;
	while (src[i] >= '0' && src[i] <= '9')
		i++;
	if (src[i] != '.')
	{
		data.precision = -1;
		return (data);
	}
	while (src[i] == '.')
		i++;
	data.precision = ft_atoi(src + i);
	return (data);
}

static t_data	get_conv(const char *src, t_data data)
{
	int		i;

	i = 0;
	while (src[i] != 's' && src[i] != 'd' && src[i] != 'x')
		i++;
	data.conv = src[i];
	return (data);
}

static char		*ft_itoa_base(long nbr, char *base)
{
	char	*dest;
	long	temp;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	if (nbr == 0)
	{
		if (!(dest = (char*)malloc(sizeof(char) * 2)))
			return (NULL);
		dest[0] = '0';
		dest[1] = '\0';
		return (dest);
	}
	if (nbr < 0)
	{
		sign = -1;
		nbr *= -1;
		i++;
	}
	temp = nbr;
	while (temp > 0)
	{
		temp /= ft_strlen(base);
		i++;
	}
	if (!(dest = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	dest[i] = '\0';
	i--;
	while (nbr > 0)
	{
		dest[i] = base[nbr % ft_strlen(base)];
		nbr /= ft_strlen(base);
		i--;
	}
	if (sign == -1)
		dest[i] = '-';
	return (dest);
}

static int	treat_dx(t_data data, int len)
{
	int		i;
	int		arg_len;

	arg_len = ft_strlen(data.c_str);
	i = 0;
	if (data.precision == 0 && data.l_arg == 0)
		arg_len = 0;
	while (data.width > arg_len && data.width > data.precision)
	{
		len++;
		ft_putchar(' ');
		data.width--;
	}
	if (data.l_arg < 0 && data.precision > arg_len)
	{
		i++;
		len++;
		ft_putchar('-');
		arg_len--;
	}
	while (data.precision > arg_len)
	{
		len++;
		ft_putchar('0');
		data.precision--;
	}
	while (arg_len > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		arg_len--;
		len++;
	}
	return (len);
}

static int	treat_s(t_data data, int len)
{
	int		i;
	int		arg_len;

	arg_len = ft_strlen(data.c_str);
	i = 0;
	if (data.precision < arg_len && data.precision != -1)
		arg_len = data.precision;
	while (data.width > arg_len)
	{
		ft_putchar(' ');
		len++;
		data.width--;
	}
	while (arg_len > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		arg_len--;
		len++;
	}
	return (len);
}

static int	get_content(const char *src, va_list list, int len)
{
	t_data	data;

	data.conv = '\0';
	data = get_width(src, data);
	data = get_precision(src, data);
	data = get_conv(src, data);
	
	if (data.conv == 's')
	{
		data.c_str = va_arg(list, char*);
		if (data.c_str == NULL)
			data.c_str = "(null)";
		len = treat_s(data, len);
	}
	else if (data.conv == 'x')
	{
		data.l_arg = (unsigned int)va_arg(list, unsigned int);
		data.c_str = ft_itoa_base(data.l_arg, "0123456789abcdef");
		len = treat_dx(data, len);
		free(data.c_str);
	}
	else
	{
		data.l_arg = va_arg(list, int);
		data.c_str = ft_itoa_base(data.l_arg, "0123456789");
		len = treat_dx(data, len);
		free(data.c_str);
	}
	return (len);
}


int		ft_printf(const char *src, ...)
{
	va_list	list;
	int		len;
	int		i;

	len = 0;
	i = 0;
	va_start(list, src);
	while (src[i] != '\0')
	{
		if (src[i] != '%')
		{
			ft_putchar(src[i]);
			len++;
			i++;
		}
		else
		{
			len = get_content(src + i + 1, list, len);
			while (src[i] != 's' && src[i] != 'd' && src[i] != 'x')
				i++;
			i++;
		}
	}
	va_end(list);
	return (len);
}
