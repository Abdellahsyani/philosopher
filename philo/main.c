/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:05:26 by asyani            #+#    #+#             */
/*   Updated: 2025/07/07 16:17:09 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	support_init(t_table *table, int i)
{
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->death_mutex, NULL);
	table->start_time = get_current_time();
	while (i < table->num_philosophers)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		pthread_mutex_init(&table->philosophers[i].times_eaten_mutex, NULL);
		i++;
	}
	i = 0;
	while (i < table->num_philosophers)
	{
		table->philosophers[i].id = i + 1;
		table->philosophers[i].times_eaten = 0;
		table->philosophers[i].table = table;
		table->philosophers[i].must_eat = false;
		table->philosophers[i].right_fork = &table->forks[i];
		table->philosophers[i].left_fork = &table->forks[(i + 1)
			% table->num_philosophers];
		i++;
	}
	table->simulation_stop = false;
	table->finish_meals = false;
}

void	allocation(t_table *table)
{
	table->philosophers = malloc(sizeof(t_philosopher)
			* table->num_philosophers);
	table->philosopher_threads = malloc(sizeof(pthread_t)
			* table->num_philosophers);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
	if (!table->philosophers || !table->philosopher_threads || !table->forks)
	{
		printf("Memory allocation failed\n");
		return ;
	}
}

int	init_table(t_table *table, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf("\033[1;31mUsage of args is wrong\033[0m\n"), 0);
	table->stop_atoi = false;
	table->num_philosophers = ft_atoi(argv[1], table);
	table->time_to_die = ft_atoi(argv[2], table);
	table->time_to_eat = ft_atoi(argv[3], table);
	table->time_to_sleep = ft_atoi(argv[4], table);
	if (argv[5])
	{
		table->must_eat_count = ft_atoi(argv[5], table);
		if (table->must_eat_count <= 0)
			return (printf("\033[1;31mError: wrong number\033[0m\n"), 0);
	}
	else
		table->must_eat_count = 0;
	if (table->stop_atoi)
		return (printf("\033[1;31mError: wrong number\033[0m\n"), 0);
	if (table->num_philosophers <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0)
		return (printf("\033[1;31mError: wrong number\033[0m\n"), 0);
	allocation(table);
	support_init(table, 0);
	return (1);
}

void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_join(table->philosopher_threads[i], NULL);
		i++;
	}
	pthread_join(table->monitor_thread, NULL);
	cleanup_table(table);
}

int	main(int argc, char **argv)
{
	t_table	table;
	int		i;

	if (!check_args(argc, argv) || !init_table(&table, argc, argv))
		return (1);
	i = 0;
	while (i < table.num_philosophers)
	{
		if (pthread_create(&table.philosopher_threads[i], NULL,
				philosopher_routine, &table.philosophers[i]) != 0)
			thread_fail();
		pthread_mutex_lock(&table.death_mutex);
		table.philosophers[i].last_meal_time = table.start_time;
		pthread_mutex_unlock(&table.death_mutex);
		i++;
	}
	if (pthread_create(&table.monitor_thread, NULL, monitor_routine,
			&table) != 0)
		thread_fail();
	join_threads(&table);
	return (0);
}
