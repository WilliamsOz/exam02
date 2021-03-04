#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct	s_data
{
	long	l_arg;
	char	conv;
	int	width;
	int	precision;
	char	*c_str;
}		t_data;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

static int	ft_atoi(const char *src)
{
	int	i;
	int	res;
	
	i = 0;
	res = 0;
	while (src[i] >= '0' && src[i] <= '9')
	{
		res = res * 10 + src[i] - '0';
		i++;
	}
	return (res);
}

static char	*ft_itoa_base(long nbr, char *base, t_data data)
{
	char	*str;
	long	nb;
	int	len;
	int	sign;

	len = 0;
	if (nbr == 0 && data.precision == 0)
	{
		if (!(str= (char*)malloc(sizeof(char))))
			return (0);
		str[0] = '\0';
		return (str);
	}
	else if (nbr == 0)
	{
		if (!(str = (char*)malloc(sizeof(char) * 2)))
			return (NULL);
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	sign = 1;
	if (nbr < 0)
	{
		len++;
		nbr *= -1;
		sign = -1;
	}
	nb = nbr;
	while (nb > 0)
	{
		nb /= ft_strlen(base);
		len++;
	}
	if (!(str = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	str[len] = '\0';
	len--;
	while (nbr > 0)
	{
		str[len] = base[nbr % ft_strlen(base)]; 
		len--;
		nbr /= ft_strlen(base);
	}
	if (sign == -1)
		str[len] = '-';
	return (str);
}

static int	get_width(const char *src)
{
	int	i;

	i = 0;
	if (src[i] == '.' || src[i] == 's' || src[i] == 'd' || src[i] == 'x')
		return (0);
	return (ft_atoi(src));
}

static int	get_precision(const char *src)
{
	int	i;

	i = 0;
	while (src[i] >= '0' && src[i] <= '9')
		i++;
	if (src[i] == '.')
		return (ft_atoi(src + i + 1));
	return (-1);
}

static int	get_conv(const char *src)
{
	int	i;

	i = 0;
	while (src[i] != 's' && src[i] != 'd' && src[i] != 'x')
		i++;
	return (src[i]);
}

static int	treat_s(t_data data, int len, int len_content)
{
	int	i;

	i = 0;
	if (data.precision == 0)
		len_content = 0;
	if (data.precision > len_content)
		data.precision = len_content;
	else if (data.precision > 0 && data.precision < len_content)
		len_content = data.precision;
	while (data.width > len_content && data.width > data.precision)
	{
		ft_putchar(' ');
		data.width--;
		len++;
	}
	while(data.precision > 0)
	{
		ft_putchar(data.c_str[i]);
		data.precision--;
		len++;
		i++;
	}
	return (len);
}


static int	treat_d_x(t_data data, int len)
{
	int	i;

	i = 0;
	if (data.conv == 'x')
		data.c_str = ft_itoa_base(data.l_arg, "0123456789abcdef", data);
	else if (data.conv == 'd')
		data.c_str = ft_itoa_base(data.l_arg, "0123456789", data);
	while (data.width > ft_strlen(data.c_str) && data.width > data.precision)
	{
		ft_putchar(' ');
		data.width--;
		len++;
	}
	if (data.l_arg < 0 )
	{
		len++;
		ft_putchar('-');
	}
	while (data.precision > ft_strlen(data.c_str))
	{
		ft_putchar('0');
		data.precision--;
		len++;
	}
	while (data.c_str[i] != '\0')
	{
		ft_putchar(data.c_str[i]);
		i++;
		len++;
	}
	free(data.c_str);
	return (len);
}

static int	get_content(const char *src, va_list list, int len)
{
	t_data	data;
	int	i;
	int	len_content;

	i = 0;
	data.width = get_width(src);
	data.precision = get_precision(src);
	data.conv = get_conv(src);
	if (data.conv == 's')
	{
		data.c_str = va_arg(list, char*);
		len_content = ft_strlen(data.c_str);
		treat_s(data, len, len_content);
	}
	else
	{
		if (data.conv == 'd')
			data.l_arg = va_arg(list, int);
		else
			data.l_arg = (unsigned int)va_arg(list, unsigned int);
		treat_d_x(data, len);
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

