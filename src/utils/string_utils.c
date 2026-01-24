#include "../../includes/cub.h"


size_t  ft_strlen(const char *s)
{
    int i;

    i = 0;
    while(s[i])
        i++;
    return (i);
}

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
        return (1);
    return (0);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	count;

	count = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[count] != '\0' && count < (size - 1))
	{
		dest[count] = src[count];
		count++;
	}
	dest[count] = '\0';
	return (ft_strlen(src));
}

int ft_isdigit(char c)
{
    if (c <= '9' && c >= '0')
        return (1);
    return (0);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    if (!s1 || !s2)
        return (-1);

    i = 0;
    while (i < n && s1[i] && s2[i])
    {
        if ((unsigned char)s1[i] != (unsigned char)s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    if (i == n)
        return 0;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char    *ft_strdup(t_mem_list **memory, const char *s)
{
    char *dup;
    size_t  len;
    size_t  i;

    if (!s)
        return (NULL);
    len = 0;
    while (s[len])
        len++;
    dup = x_malloc(memory, len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while(i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}