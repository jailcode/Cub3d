#include "../../includes/cub.h"

int	init_memory_list(t_mem_list **m)
{
	t_mem_list	*ret;

	ret = malloc(sizeof(*ret));
	if (!ret)
		return (1);
	ret->address = NULL;
	ret->next = NULL;
	ret->prev = NULL;
	*m = ret;
	return (0);
}

void	clean_memory_list(t_mem_list **mem)
{
	t_mem_list	*temp;
	int			count;
	
	if (!mem || !(*mem))
		return ;
	// Find the start of the list
	while ((*mem)->prev != NULL)
	{
		if ((*mem)->prev == *mem)  // Detect circular reference
			break ;
		(*mem) = (*mem)->prev;
	}

	// Free all nodes from start to end
	count = 0;
	while (*mem != NULL && count < 1000000)
	{
		temp = (*mem)->next;
		if ((*mem)->address != NULL)
			free((*mem)->address);
		free(*mem);
		*mem = temp;
		count++;
	}
}

void	*x_malloc(t_mem_list **mem, size_t size)
{
    t_mem_list	*curr;
    t_mem_list	*new_node;

    if (!mem || !*mem || size == 0)
        return (NULL);
    new_node = malloc(sizeof(*new_node));
    if (!new_node)
        return (NULL);
    new_node->address = malloc(size);
    if (!new_node->address)
    {
        free(new_node);
        return (NULL);
    }
    new_node->next = NULL;
    curr = *mem;
    while (curr->next)
        curr = curr->next;
    curr->next = new_node;
    new_node->prev = curr;
    return (new_node->address);
}

void	close_fds_in_range(int start, int end)
{
	while(start < end)
	{
		close(start);
		start++;
	}
}

void    clean_exit(t_mem_list *memory, int code, char *message)
{
    clean_memory_list(&memory);
	if (message)
		printf("Error\n%s\n", message);
	else
		printf("Error\n");
	close_fds_in_range(3, 1024);
    exit(code);
}