#include "../../includes/cub.h"

static char	*fr_free(t_mem_list *memory, char *buffer, char *buf)
{
	char	*temp;

	temp = ft_strjoin(memory, buffer, buf);
	//free(buffer);
	return (temp);
}

static char	*ft_next(t_mem_list *memory, char *buffer)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		return (NULL);
	}
	line = ft_calloc(memory, (ft_strlen(buffer) - i + 1), sizeof(char));
	i++;
	j = 0;
	while (buffer[i])
		line[j++] = buffer[i++];
	return (line);
}

static char	*ft_line(t_mem_list *memory, char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(memory, i + 2, sizeof(char));
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] && buffer[i] == '\n')
		line[i++] = '\n';
	return (line);
}

static char	*read_file(t_mem_list *memory, int fd, char *res)
{
	char	*buffer;
	int		byte_read;

	if (!res)
		res = ft_calloc(memory, 1, 1);
	buffer = ft_calloc(memory, BUFFER_SIZE + 1, sizeof(char));
	byte_read = 1;
	while (byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			return (NULL);
		}
		buffer[byte_read] = 0;
		res = fr_free(memory, res, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (res);
}

char	*get_next_line(t_mem_list *memory, int fd)
{
	static char	*buffer = NULL;
	char		*line;

	if (fd == -1)
	{
		buffer = NULL;
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		buffer = NULL;
		return (NULL);
	}
	buffer = read_file(memory, fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_line(memory, buffer);
	buffer = ft_next(memory, buffer);
	return (line);
}