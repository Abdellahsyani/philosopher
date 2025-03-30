/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/03/29 15:35:32 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken a fork");
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
	precise_sleep(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philosopher *philo)
{
	print_status(philo->table, philo->id, "is sleeping");
	precise_sleep(philo->table->time_to_sleep);
}

void	philo_think(t_philosopher *philo)
{
	print_status(philo->table, philo->id, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philosopher *philo = (t_philosopher *)arg;
	t_table *table = philo->table;

	if (philo->id % 2 == 0)
		precise_sleep(1);

	while (1)
	{
		pthread_mutex_lock(&table->print_mutex);
		if (table->simulation_stop)
		{
			pthread_mutex_unlock(&table->print_mutex);
			break;
		}
		pthread_mutex_unlock(&table->print_mutex);
		philo_eat(philo);
		if (table->must_eat_count > 0 && 
			philo->times_eaten >= table->must_eat_count)
			break;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void *monitor_routine(void *arg)
{
	t_table	*table = (t_table *)arg;
	int	i;
	long long	current_time;

	while (1)
	{
		i = 0;
		while (i < table->num_philosophers)
		{
			current_time = get_current_time();
			pthread_mutex_lock(&table->death_mutex);
			long long last_meal = table->philosophers[i].last_meal_time;
			pthread_mutex_unlock(&table->death_mutex);
			if (current_time - last_meal > table->time_to_die)
			{
				pthread_mutex_lock(&table->print_mutex);
				if (!table->simulation_stop)
				{
					printf("%lld %d died\n", current_time - table->start_time, table->philosophers[i].id);
					table->simulation_stop = true;
				}
				pthread_mutex_unlock(&table->print_mutex);
				return NULL;
			}
			i++;
		}
		if (table->must_eat_count > 0)
		{
			bool all_satisfied = true;
			for (i = 0; i < table->num_philosophers; i++)
			{
				pthread_mutex_lock(&table->philosophers[i].times_eaten_mutex);
				if (table->philosophers[i].times_eaten < table->must_eat_count)
				{
					pthread_mutex_unlock(&table->philosophers[i].times_eaten_mutex);
					all_satisfied = false;
					break;
				}
				pthread_mutex_unlock(&table->philosophers[i].times_eaten_mutex);}
			if (all_satisfied)
			{
				pthread_mutex_lock(&table->print_mutex);
				table->simulation_stop = true;
				pthread_mutex_unlock(&table->print_mutex);
				return NULL;
			}
		}
		usleep(100);
	}
}
