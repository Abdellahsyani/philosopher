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
	int	*i;
	pthread_mutex_t	*mutex;

	pthread_mutex_init(&mutex, NULL);
	i = (int *)arg;

	printf("start a thread......[%d]\n", *i);
	return (NULL);
}

void	philo_handler(t_philo *philo)
{
	int	i;
	int	*ids;
	pthread_t	*thread_id;

	i = 0;
	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
	ids = malloc(sizeof(int) * philo->philo_num);
	if (!thread_id || !ids)
		return ;
	while (i <= philo->philo_num)
	{
		ids[i] = i + 1;
		pthread_create(&thread_id[i], NULL, thread_routine, &ids[i]);
		i++;
	}
	i = 0;
	while (i < philo->philo_num)
	{
		pthread_join(thread_id[i], NULL);
		i++;
	}
}
