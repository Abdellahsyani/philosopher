/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:03:43 by asyani            #+#    #+#             */
/*   Updated: 2025/07/07 16:06:45 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	take_forks(t_philosopher *philo)
{
	size_t	total_time;

	total_time = philo->table->time_to_die - philo->table->time_to_eat
		- philo->table->time_to_sleep;
	if (philo->table->num_philosophers % 2 && philo->times_eaten > 0)
		precise_sleep(total_time / 2, philo->table);
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken right fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken right fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken left fork");
	}
}

void	philo_eat(t_philosopher *philo)
{
	take_forks(philo);
	print_status(philo->table, philo->id, "is eating");
	pthread_mutex_lock(&philo->table->death_mutex);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->table->death_mutex);
	pthread_mutex_lock(&philo->times_eaten_mutex);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->times_eaten_mutex);
	precise_sleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	check_died(t_table *table, size_t current_time, int i)
{
	pthread_mutex_lock(&table->print_mutex);
	if (!table->simulation_stop)
	{
		printf("%zu %zu died\n", current_time - table->start_time,
			table->philosophers[i].id);
		table->simulation_stop = true;
	}
	pthread_mutex_unlock(&table->print_mutex);
}
