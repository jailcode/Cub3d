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

	if (!mem || !(*mem))
		return ;
	while ((*mem)->prev != NULL)
		(*mem) = (*mem)->prev;
	while ((*mem))
	{
		temp = (*mem)->next;
		if ((*mem)->address)
			free((*mem)->address);
		free(*mem);
		*mem = temp;
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

void    clean_exit(t_mem_list *memory, int code)
{
    clean_memory_list(&memory);
	printf("Error\n");
    exit(code);
}