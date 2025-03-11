/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:48:36 by asyani            #+#    #+#             */
/*   Updated: 2025/03/11 16:05:20 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*thread_routine(void *arg)
{

}

void	philo_handler(t_philo *philo)
{
	int	i;
	pthread_t	thread_id;

	i = 0;
	while (i < philo->philo_num)
	{
		pthread_create(&thread_id, NULL, thread_routine, NULL);
		i++;
	}
}
