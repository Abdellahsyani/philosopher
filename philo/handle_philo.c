/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/07/07 16:16:19 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	s_similation(t_table *table, t_philosopher *philo)
{
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
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*table;

	philo = (t_philosopher *)arg;
	table = philo->table;
	if (philo->id % 2 == 0)
		precise_sleep(1, table);
	s_similation(table, philo);
	return (NULL);
}

static void	*handle_must_eaten_time(t_table *table)
{
	int	i;

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
	}
	else
		table->finish_meals = false;
	return (NULL);
}

static int	start_checking(t_table *table)
{
	int	i;
	int	current_time;
	int	last_meal;

	i = 0;
	while (i < table->num_philosophers)
	{
		current_time = get_current_time();
		pthread_mutex_lock(&table->death_mutex);
		last_meal = table->philosophers[i].last_meal_time;
		pthread_mutex_unlock(&table->death_mutex);
		if (current_time - last_meal >= table->time_to_die)
		{
			check_died(table, current_time, i);
			return (0);
		}
		if (table->finish_meals)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (start_checking(table) == 0)
			return (NULL);
		handle_must_eaten_time(table);
		usleep(100);
	}
	return (NULL);
}
