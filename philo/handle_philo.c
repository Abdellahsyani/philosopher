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

void	*thread_routine(void *arg)
{
	t_data	*thread;
	
	thread = (t_data *)arg;

	pthread_mutex_lock(thread->mutex);
	printf("start a thread......[%d]\n", thread->id);
	pthread_mutex_unlock(thread->mutex);
	printf("thread [%d] has finished\n", thread->id);
	return (NULL);
}

void	philo_handler(t_philo *philo)
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
