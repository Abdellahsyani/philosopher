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

/*long	get_time_ms(void)*/
/*{*/
/*	struct timeval	time;*/
/**/
/*	gettimeofday(&time, NULL);*/
/*	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));*/
/*}*/

int get_time_ms(t_data *thread)
{
	static int start_time;
	struct timeval time;
	int	current_time;

	gettimeofday(&time, NULL);
	current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	pthread_mutex_lock(thread->mutex);
	if (start_time == 0)
		start_time = current_time;
	pthread_mutex_unlock(thread->mutex);
	return current_time - start_time;
}

void philosopher_eat(t_data *thread)
{
	int	time;

	time = get_time_ms(thread);
	pthread_mutex_lock(thread->mutex);
	printf("%d %d is eating\n", time, thread->philo.philo_list->philo_id);
	pthread_mutex_unlock(thread->mutex);
	usleep(thread->fi_info->time_to_sleep * 1000);
}

void philosopher_sleep(t_data *thread)
{
	int	time;

	time = get_time_ms(thread);
	pthread_mutex_lock(thread->mutex);
	printf("%d %d is sleeping\n", time, thread->philo.philo_list->philo_id);
	pthread_mutex_unlock(thread->mutex);
	usleep(thread->fi_info->time_to_sleep * 1000);
}

/*void philosopher_thinking(t_data *thread)*/
/*{*/
/*	int	time;*/
/**/
/*	time = get_time_ms();*/
/*	pthread_mutex_lock(thread->mutex);*/
/*	printf("%d %d is thinking\n", time, thread->philo.philo_list->philo_id);*/
/*	pthread_mutex_unlock(thread->mutex);*/
/*	usleep(thread->fi_info->time_);*/
/*}*/

void take_forks(t_data *thread)
{
	int	time;
	int	left_id;
	int	right_id;

	left_id = thread->philo.philo_list->philo_id - 1;
	right_id = thread->philo.philo_list->philo_id % thread->fi_info->philo_num;
	time = get_time_ms(thread);
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
	/*struct timeval	start_time;*/
	/*struct timeval	current_time;*/

	thread = (t_data *)arg;
	while (1)
	{
		take_forks(thread);
		philosopher_sleep(thread);
		/*phlosopher_thinking(thread);*/

	}
	return (NULL);
}

void	philo_handler(t_head *philo)
{
	int i;
	pthread_t *thread_id;
	t_data *thread_mutex;
	pthread_mutex_t mutex;

	i = 0;
	pthread_mutex_init(&mutex, NULL);

	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
	thread_mutex = malloc(sizeof(	t_data) * philo->philo_num);

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
		thread_mutex[i].mutex = &mutex;
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
