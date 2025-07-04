/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/04/01 12:04:40 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken right fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken left fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo->table, philo->id, "has taken left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo->table, philo->id, "has taken right fork");
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
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*table;

	philo = (t_philosopher *)arg;
	table = philo->table;
	if (philo->id % 2 == 0)
		precise_sleep(1, table);
	while (1)
	{
		if (table->num_philosophers == 1)
		{
			print_status(table, 1, "has taken a fork");
			precise_sleep(table->time_to_die, table);
			break ;
		}
		pthread_mutex_lock(&table->print_mutex);
		if (table->simulation_stop)
		{
			pthread_mutex_unlock(&table->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->print_mutex);
		philo_think(philo);
		philo_eat(philo);
		if (table->must_eat_count > 0
			&& philo->times_eaten >= table->must_eat_count)
			break ;
		philo_sleep(philo);
	}
	return (NULL);
}

static void	*handle_must_eaten_time(t_table *table)
{
	size_t	i;

	i = 0;
	if (table->must_eat_count > 0 && table->num_philosophers != 1)
	{
		while (i < table->num_philosophers)
		{
			pthread_mutex_lock(&table->philosophers[i].times_eaten_mutex);
			if (table->philosophers[i].times_eaten < table->must_eat_count)
			{
				pthread_mutex_unlock(&table->philosophers[i].times_eaten_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&table->philosophers[i].times_eaten_mutex);
			i++;
		}
		table->finish_meals = true;
		pthread_mutex_lock(&table->print_mutex);
		table->simulation_stop = true;
		pthread_mutex_unlock(&table->print_mutex);
	}
	else
		table->finish_meals = true;
	return (NULL);
}

static void	check_died(t_table *table, size_t current_time, int i)
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

void	*monitor_routine(void *arg)
{
	t_table		*table;
	size_t			i;
	size_t	current_time;
	size_t	last_meal;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&table->print_mutex);
		if (table->simulation_stop)
		{
			pthread_mutex_unlock(&table->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->print_mutex);
		while (i < table->num_philosophers)
		{
			current_time = get_current_time();
			pthread_mutex_lock(&table->death_mutex);
			last_meal = table->philosophers[i].last_meal_time;
			pthread_mutex_unlock(&table->death_mutex);
			if (current_time - last_meal >= table->time_to_die && table->finish_meals)
			{
				check_died(table, current_time, i);
				return (NULL);
			}
			i++;
		}
		handle_must_eaten_time(table);
	}
	return (NULL);
}
