/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/03/22 10:30:03 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void take_forks(t_data *thread)
{
	int	time;
	int	left_id;
	int	right_id;

	left_id = thread->philo.philo_list->philo_id - 1;
	right_id = thread->philo.philo_list->philo_id % thread->fi_info->philo_num;
	time = get_time_ms();
	if (left_id < right_id)
	{
		pthread_mutex_lock(thread->left_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
		pthread_mutex_lock(thread->right_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
	}
	else
	{
		pthread_mutex_lock(thread->right_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
		pthread_mutex_lock(thread->left_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
	}
	philosopher_eat(thread);
	pthread_mutex_unlock(thread->left_fork);
	pthread_mutex_unlock(thread->right_fork);
}

void	*thread_routine(void *arg)
{
	t_data	*thread;
	struct timeval	start_time;
	struct timeval	current_time;
	int	time;

	thread = (t_data *)arg;
	gettimeofday(&start_time, NULL);
	while (1)
	{
		take_forks(thread);
		philosopher_sleep(thread);
		phlosopher_thinking(thread);

	}

	return (NULL);
}

void	philo_handler(t_head *philo)
{
	int i;
	pthread_t *thread_id;
	t_data *thread_mutex;
	pthread_mutex_t table_mutex;

	i = 0;
	pthread_mutex_init(&table_mutex, NULL);

	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
	thread_mutex = malloc(sizeof(t_data) * philo->philo_num);

	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * philo->philo_num);

	if (!thread_id || !thread_mutex || !forks)
		return;
	int j = 0;
	while (j++ < philo->philo_num)
		pthread_mutex_init(&forks[j], NULL);
	philo->died_flag = 0;    
	while (i < philo->philo_num)
	{
		thread_mutex[i].id = i + 1;
		thread_mutex[i].mutex = &table_mutex;
		thread_mutex[i].fi_info = philo;

		thread_mutex[i].left_fork = &forks[i];
		thread_mutex[i].right_fork = &forks[(i + 1) % philo->philo_num]; 

		thread_mutex[i].philo.philo_list = NULL;
		pthread_mutex_lock(thread_mutex[i].mutex); 
		add_to_list(&thread_mutex[i].philo.philo_list, thread_mutex[i].id);
		pthread_mutex_unlock(thread_mutex[i].mutex); 
		thread_mutex[i].philo.philo_list->time_to_die = philo->time_to_die;
		thread_mutex[i].philo.philo_list->time_to_eat = philo->time_to_eat;
		thread_mutex[i].philo.philo_list->time_to_sleep = philo->time_to_sleep;
		thread_mutex[i].philo.philo_list->eating_times = philo->eating_times;

		pthread_create(&thread_id[i], NULL, thread_routine, &thread_mutex[i]);
		i++;
	} 
	i = 0;
	while (i < philo->philo_num)
	{
		pthread_join(thread_id[i], NULL);
		i++;
	}

	i = 0;
	while (i < philo->philo_num)
	{
		t_philo *current = thread_mutex[i].philo.philo_list;
		while (current)
		{
			t_philo *temp = current;
			current = current->next;
			free(temp);
		}
		i++;
	}

	pthread_mutex_destroy(&mutex);
	free(thread_id);
	free(thread_mutex);
}
