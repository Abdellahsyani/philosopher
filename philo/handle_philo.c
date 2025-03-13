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
	(void)arg;
	printf("start a thread\n");

	return (NULL);
}

void	philo_handler(t_philo *philo)
{
	int	i;
	pthread_t	*thread_id;

	i = 0;
	thread_id = malloc(sizeof(pthread_t));
	if (!thread_id)
		return ;
	while (i <= philo->philo_num)
	{
		pthread_create(&thread_id[i], NULL, thread_routine, NULL);
		i++;
	}
}
