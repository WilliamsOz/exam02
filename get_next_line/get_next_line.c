#include "./get_next_line.h"

static int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	if (str == NULL)
		return (i);
	while (str[i] != '\0')
		i++;
	return (i);
}

static char		*ft_strsjoin(char *line, char *buffer)
{
	char	*dest;
	int		i;

	i = ft_strlen(line) + 1;
	if (!(dest = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	dest[i] = '\0';
	i = 0;
	if (line != NULL)
	{
		while (line[i] != '\0')
		{
			dest[i] = line[i];
			i++;
		}
		free(line);
	}
	dest[i] = buffer[0];
	line = dest;
	return (line);
}

static char		*get_line(char *line, int *eof)
{
	char	buffer[1];

	*eof = read(0, buffer, 1);
	if (*eof == -1)
		return (NULL);
	if (buffer[0] == '\n' && line != NULL)
		return (line);
	if (buffer[0] == '\n' || *eof == 0)
	{
		if (!(line = (char*)malloc(sizeof(char) * 1)))
			return (NULL);
		line[0] = '\0';
		return (line);
	}
	else
	{
		line = ft_strsjoin(line, buffer);
		if (line == NULL)
		{
			*eof = -1;
			return (NULL);
		}
		line = get_line(line, eof);
	}
	return (line);
}

int				get_next_line(char **line)
{
	int		eof;
	
	*line = get_line(*line, &eof);
	return (eof);
}

