/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:50:13 by asyani            #+#    #+#             */
/*   Updated: 2025/04/01 12:04:11 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (gettimeofday(&tv, NULL) == -1)
	{
		write(2, "gettimeofday() error\n", 22);
		return (0);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(int milliseconds, t_table *table)
{
	int	start;

	(void)table;
	start = get_current_time();
	while (get_current_time() - start < milliseconds)
	{
		usleep(100);
	}
}

void	print_status(t_table *table, int philosopher_id, const char *status)
{
	int	current_time;

	pthread_mutex_lock(&table->print_mutex);
	if (!table->simulation_stop)
	{
		current_time = get_current_time();
		printf("%d %d %s\n", current_time - table->start_time, philosopher_id,
			status);
	}
	pthread_mutex_unlock(&table->print_mutex);
}

void	philo_sleep(t_philosopher *philo)
{
	print_status(philo->table, philo->id, "is sleeping");
	precise_sleep(philo->table->time_to_sleep, philo->table);
}

void	philo_think(t_philosopher *philo)
{
	print_status(philo->table, philo->id, "is thinking");
}
