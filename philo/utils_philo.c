/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:50:13 by asyani            #+#    #+#             */
/*   Updated: 2025/03/22 09:50:33 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * create_node _ function to create the node
 * @id: the data that node will take
 */
t_philo	*create_node(int id)
{
	t_philo	*new_node;

	new_node = malloc(sizeof(t_philo));
	if (!new_node)
		return (NULL);
	new_node->philo_id = id;
	new_node->next = NULL;
	new_node->time_to_die = 0;
	new_node->time_to_eat = 0;
	new_node->time_to_sleep = 0;
	new_node->eating_times = 0;
	new_node->num_of_forks = 0;
	return (new_node);
}

/**
 * add_to_stack _ function to add nodes to stack
 * @list: the philo list
 * @data: the philo id
 */
void	add_to_list(t_philo **list, int data)
{
	t_philo	*new_node;
	t_philo	*temp;

	new_node = create_node(data);
	if (!new_node)
	{
		/*free_stack(stack);*/
		return ;
	}
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	temp = *list;
	while (temp->next)
	{
		temp = temp->next;
	}
	temp->next = new_node;
}
