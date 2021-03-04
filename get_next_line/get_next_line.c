#include "./get_next_line.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static char	*ft_strsjoin_buffer_in_line(char *line, char *buffer)
{
	char	*dest;
	int	i;

	if (line == NULL)
	{
		if (!(line = (char*)malloc(sizeof(char) * 2)))
			return (NULL);
		line[0] = buffer[0];
		line[1] = '\0';
	}
	else
	{
		if (!(dest = (char*)malloc(sizeof(char) * (ft_strlen(line) + 2))))
			return (NULL);
		i = 0;
		while (line[i] != '\0')
		{
			dest[i] = line[i];
			i++;
		}
		dest[i] = buffer[0];
		dest[i + 1] = '\0';
		free(line);
		line = dest;
	}
	return (line);
}

static char	*get_line(char *line, char *buffer, size_t *ptr_is_eof)
{
	*ptr_is_eof = read(0, buffer, 1);
	if (line == NULL && (buffer[0] == '\n' || buffer[0] == '\0'))
	{
		if (!(line = (char*)malloc(sizeof(char) * 1)))
			return (NULL);
		line[0] = '\0';
		return (line);
	} 
	else if (buffer[0] == '\n')
		return (line);
	else
	{
		line = ft_strsjoin_buffer_in_line(line, buffer);
		buffer[0] = '\0';
		*ptr_is_eof = read(0, buffer, 1);
		if (*ptr_is_eof == 0)
			return (line);
		else
			line = ft_strsjoin_buffer_in_line(line, buffer);
		buffer[0] = '\0';
		line = get_line(line, buffer, ptr_is_eof);
	}
	return (line);
}

int	get_next_line(char **line)
{
	char	buffer[1];
	size_t	is_eof;

	buffer[0] = '\0';
	if ((*line = get_line(*line, buffer, &is_eof)) == NULL)
		return (-1);
	if (is_eof == 1)
		return (1);
	return (0);
}
