#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct	s_data
{
	long	l_arg;
	char	conv;
	int		width;
	int		precision;
	char	*c_str;
}		t_data;

static int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static void		ft_putchar(char c)
{
	write(1, &c, 1);
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

static t_data	get_conv(const char *src, t_data data)
{
	int		i;

	i = 0;
	while (src[i] != 's' && src[i] != 'd' && src[i] != 'x')
		i++;
	data.conv = src[i];
	return (data);
}

static t_data	get_width(const char *src, t_data data)
{
	int		i;

	i = 0;
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
		data.precision = -1;
	else
	{
		while (src[i] == '.')
			i++;
		data.precision = ft_atoi(src + i);
	}
	return (data);
}

static char		*ft_itoa_base(long nbr, char *base)
{
	long	temp;
	char	*dest;
	int		len;
	int		sign;

	len = 0;
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
		len++;
		nbr *= -1;
	}
	temp = nbr;
	while (temp > 0)
	{
		temp /= ft_strlen(base);
		len++;
	}
	if (!(dest = (char*)malloc(sizeof(char) * (len + 1))))
			return (NULL);
	dest[len] = '\0';
	len--;
		while (nbr > 0)
	{
		dest[len] = base[nbr % ft_strlen(base)];
		nbr /= ft_strlen(base);
		len--;
	}
	if (sign == -1)
		dest[len] = '-';
	return (dest);
}

static int		treat_dx(t_data data, int len)
{
	int		i;
	int		len_arg;

	len_arg = ft_strlen(data.c_str);
	i = 0;
	if (data.precision < len_arg && data.l_arg < 0)
		data.precision = len_arg;
	if (data.l_arg == 0 && data.precision == 0)
		len_arg = 0;
	while (data.width > len_arg && data.width > data.precision)
	{
		ft_putchar(' ');
		len++;
		data.width--;
	}
	if (data.l_arg < 0 && data.precision > len_arg)
	{
		ft_putchar('-');
		len++;
		len_arg--;
		i++;
	}
	while (data.precision > len_arg)
	{
		ft_putchar('0');
		data.precision--;
		len++;
	}
	while (len_arg > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		len_arg--;
		len++;
	}
	return (len);
}

static int		treat_s(t_data data, int len)
{
	int		i;
	int		len_arg;

	len_arg = ft_strlen(data.c_str);
	i = 0;
	if (data.precision < len_arg && data.precision != -1)
		len_arg = data.precision;
	while (data.width > len_arg)
	{
		ft_putchar(' ');
		data.width--;
		len++;
	}
	while (len_arg > 0)
	{
		ft_putchar(data.c_str[i]);
		i++;
		len++;
		len_arg--;
	}
	return (len);
}

static	int		get_content(const char *src, va_list list, int len)
{
	t_data	data;
	
	data.conv = '\0';
	data = get_conv(src, data);
	data = get_width(src, data);
	data = get_precision(src, data);

	if (data.conv == 'd')
	{
		data.l_arg = va_arg(list, int);
		data.c_str = ft_itoa_base(data.l_arg, "0123456789");
		len = treat_dx(data, len);
		free(data.c_str);
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
		data.c_str = va_arg(list, char*);
		if (data.c_str == NULL)
			data.c_str = "(null)";
		len = treat_s(data, len);
	}
	return (len);
}

int	ft_printf(const char *src, ...)
{
	va_list	list;
	int	i;
	int	len;

	len = 0;
	if (src[0] == '\0')
		return (len);
	i = 0;
	va_start(list, src);
	while (src[i] != '\0')
	{
		if (src[i] != '%')
		{
			ft_putchar(src[i]);
			i++;
			len++;
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

