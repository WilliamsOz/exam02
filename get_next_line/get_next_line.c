#include "./get_next_line.h"
#include <libc.h>

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

static char		*get_line(char *line, int *eof, int i)
{
	char	buffer[1];
	int		is_eof;

	is_eof = read(0, buffer, 1);
	*eof = is_eof;
	if (is_eof == -1)
		return (NULL);
	if (line == NULL && (buffer[0] == '\n' || is_eof == 0))
	{
		if (!(line = (char*)malloc(sizeof(char) * 1)))
			return (NULL);
		line[0] = '\0';
		return (line);
	}
	else if (buffer[0] == '\n')
		return (line);
	line = ft_strsjoin(line, buffer);
	if (line == NULL)
	{
		*eof = -1;
		return (NULL);
	}
	*eof = is_eof;
	line = get_line(line, eof, i + 1);
	return (line);
}

#include <libc.h>

int				get_next_line(char **line)
{
	int		eof;
	
	*line = get_line(*line, &eof, 0);
	return (eof);
}

