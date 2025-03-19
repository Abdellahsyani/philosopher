/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/03/13 16:33:26 by asyani           ###   ########.fr       */
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

void	*thread_routine(void *arg)
{
	t_data	*thread;
	struct timeval	start_time;
	t_philo	*philo;

	philo = NULL;
	thread = (t_data *)arg;

	add_to_list(&philo, thread->id);
	/*while (philo)*/
	/*{*/
	/*	printf("\"%d %d %d %d %d\"-->", philo->philo_id, philo->time_to_die, philo->time_to_eat, philo->time_to_sleep, philo->eating_times);*/
	/*	philo = philo->next;*/
	/*}*/
	while (1)
	{
		pthread_mutex_lock(thread->mutex);
		while (philo)
		{
			if (philo->philo_id)
			{
				//start counting the time
				gettimeofday(&start_time, NULL);
				printf("%ld %d has taken a fork\n", start_time.tv_sec ,philo->philo_id);
				philo->num_of_forks++;
			}
			if (philo->num_of_forks == 2)
			{
				//update the meal time
				gettimeofday(&start_time, NULL);
				printf("%ld %d is eating\n", start_time.tv_sec ,philo->philo_id);
				usleep(thread->philo.time_to_eat * 1000);
			}
			/*printf("%d %d is sleeping\n", kj, thread->id);*/
			/*printf("%d %d is thinking\n", kj, thread->id);*/
			/*printf("%d %d is died\n", kj, thread->id);*/
			philo = philo->next;
		}
		pthread_mutex_unlock(thread->mutex);
	}
	return (NULL);
}

void	philo_handler(t_head *philo)
{
	int	i;
	pthread_t	*thread_id;
	t_data	*thread_mutex;
	pthread_mutex_t	mutex;

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
	thread_mutex = malloc(sizeof(t_data) * philo->philo_num);
	if (!thread_id || !thread_mutex)
		return ;
	while (i < philo->philo_num)
	{
		thread_mutex[i].id = i + 1;
		thread_mutex[i].mutex = &mutex;
		pthread_create(&thread_id[i], NULL, thread_routine, &thread_mutex[i]);
		i++;
	}
	i = 0;
	while (i < philo->philo_num)
	{
		pthread_join(thread_id[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	free(thread_id);
	free(thread_mutex);
}
