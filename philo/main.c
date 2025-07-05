/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:05:26 by asyani            #+#    #+#             */
/*   Updated: 2025/04/01 12:03:25 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_atoi(char *str, t_table *table)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res >= INT_MAX)
			return (table->stop_atoi = true, 0);
		i++;
	}
	return (res * sign);
}

static void	support_init(t_table *table)
{
	size_t	i;

	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->waiter, NULL);
	table->start_time = get_current_time();
	i = 0;
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
		//table->philosophers[i].last_meal_time = table->start_time;
		table->philosophers[i].table = table;
		table->philosophers[i].right_fork = &table->forks[i];
		table->philosophers[i].left_fork = &table->forks[(i + 1) % table->num_philosophers];
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
	{
		printf("Usage of argumenets is wrong\n");
		return (0);
	}
	table->stop_atoi = false;
	table->num_philosophers = ft_atoi(argv[1], table);
	table->time_to_die = ft_atoi(argv[2], table);
	table->time_to_eat = ft_atoi(argv[3], table);
	table->time_to_sleep = ft_atoi(argv[4], table);
	if (argv[5])
	{
		table->must_eat_count = ft_atoi(argv[5], table);
		if (table->must_eat_count <= 0)
			return (0);
	}
	else
		table->must_eat_count = 0;
	if (table->stop_atoi)
		return (0);
	if (table->num_philosophers <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0)
	{
		printf("Invalid input parameters\n");
		return (0);
	}
	allocation(table);
	support_init(table);
	return (1);
}

void	cleanup_table(t_table *table)
{
	size_t	i;

	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->death_mutex);
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->philosophers);
	free(table->philosopher_threads);
	free(table->forks);
}

static int	thread_fail(void)
{
	printf("Failed to create philosopher thread\n");
	//don't forget to destroy and free here 
	return (1);
}

int	main(int argc, char **argv)
{
	t_table	table;
	size_t		i;

	if (!init_table(&table, argc, argv))
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
		if (table.num_philosophers > 1)
			usleep(3);
		i++;
	}
	if (pthread_create(&table.monitor_thread, NULL, monitor_routine,
			&table) != 0)
		thread_fail();
	i = 0;
	while (i < table.num_philosophers)
	{
		pthread_join(table.philosopher_threads[i], NULL);
		i++;
	}
	pthread_join(table.monitor_thread, NULL);
	cleanup_table(&table);
	return (0);
}
