#include "../../includes/cub.h"

void	dfs(t_parser *data, char **map, int row, int col)
{
	if (!data)
		clean_exit(data->parse_memory, 1, "no data");
	if (row < 0 || row >= data->map->rows || col < 0 || col >= data->map->col)
		return ;
	if (ft_strchr("0NSEW", map[row][col]))
		clean_exit(data->parse_memory, 1, "Map is not closed");
	if (map[row][col] != ' ')
		return ;
	map[row][col] = '1';
	dfs(data, map, row + 1, col);
	dfs(data, map, row, col + 1);
	dfs(data, map, row - 1, col);
	dfs(data, map, row, col - 1);
}

char	*ft_strtrim(t_mem_list **memory, char const *s1, char const *set)
{
	int start;
	int end;
	char *str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[start]) && start <= end)
		start++;
	if (start > end)
		return (ft_strdup(memory, s1 + end + 1));
	while (ft_strchr(set, s1[end]) && end >= 0)
		end--;
	str = x_malloc(memory, end - start + 2);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[start], end - start + 2);
	return (str);
}