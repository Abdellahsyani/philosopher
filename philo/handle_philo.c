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

/*struct timeval	convert_time(int num)*/
/*{*/
/*	struct timeval	time;*/
/**/
/**/
/*}*/

void	*thread_routine(void *arg)
{
	t_data	*thread;
	struct timeval	start_time;
	struct timeval	current_time;
	thread = (t_data *)arg;

	/*while (thread->philo.philo_list)*/
	/*{*/
	/*	printf("\"%d %d %d %d %d\"-->", thread->philo.philo_list->philo_id, thread->philo.philo_list->time_to_die, thread->philo.philo_list->time_to_eat, thread->philo.philo_list->time_to_sleep, thread->philo.philo_list->eating_times);*/
	/*	thread->philo.philo_list = thread->philo.philo_list->next;*/
	/*}*/
	long elapsed;
	gettimeofday(&start_time, NULL);
	while (1)
	{
		if (thread->philo.philo_list->philo_id > 0)
		{
			//start counting the time
			gettimeofday(&current_time, NULL);
			long elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
			printf("%ld %d has taken a fork\n", elapsed_time,thread->philo.philo_list->philo_id);
			thread->philo.philo_list->num_of_forks++;
		}
		if (thread->philo.philo_list->num_of_forks == 2)
		{
			//update the meal time
			gettimeofday(&current_time, NULL);
			long elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
			printf("%ld %d is eating\n", elapsed_time,thread->philo.philo_list->philo_id);
			usleep(thread->philo.time_to_eat * 1000);
			thread->philo.philo_list->num_of_forks = 0;
			gettimeofday(&thread->philo.philo_list->last_meal, NULL);
			elapsed = (thread->philo.philo_list->last_meal.tv_sec - start_time.tv_sec) * 1000 + (thread->philo.philo_list->last_meal.tv_usec - start_time.tv_usec) / 1000;
			printf("The last meal_time %ld of philo %d\n", elapsed, thread->philo.philo_list->philo_id);
			printf("%ld %d is sleeping\n", elapsed_time, thread->philo.philo_list->philo_id);
			usleep(thread->fi_info.time_to_sleep * 1000);
		}
		if (elapsed > thread->philo.philo_list->time_to_die)
		{
			gettimeofday(&current_time, NULL);
			long died = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
			printf("%ld %d is died\n", died,thread->philo.philo_list->philo_id);
			exit(1);
		}
	}
	/*printf("%d %d is thinking\n", kj, thread->id);*/
	/*printf("%d %d is died\n", kj, thread->id);*/
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
		add_to_list(&thread_mutex[i].philo.philo_list, thread_mutex[i].id);
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
