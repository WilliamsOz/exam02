#include <libc.h>

char  *recursive(int fd, int i, int *ret)
{
	char buff[1];
	int rd;
	char *line;
	rd = read(fd, buff, 1);
	if (rd == -1)
		return (0);
	if (rd == 0)
		buff[0] = '\0';
	if (buff[0] == '\n' || buff[0] == '\0')
	{
		if ((line = malloc(sizeof(char) * (i + 1))) == NULL)
		{
			*ret = -1;
			return (0);
		}
		line[i] = '\0';
		*ret = 1;
		if (buff[0] == '\0')
			*ret = 0;
		return (line);
	}
	else
	{
		if (!(line = recursive(fd, i + 1, ret)))
		{
			*ret = -1;
			return (0);
		}
		line[i] = buff[0];
	}
	return (line);
}
int         get_next_line(int fd, char **line)
{
	int ret;
	*line = recursive(fd, 0, &ret);
	return (ret);
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"
#include <fcntl.h>

int
	main(void)
{
	int		r;
	char	*line;

	line = NULL;
	int fd = open("./cccns.txt", O_RDONLY);
	while ((r = get_next_line(fd, &line)) > 0)
	{
		printf("%s\n", line);
		free(line);
		line = NULL;
	}
	printf("%s", line);
	free(line);
	line = NULL;
}
