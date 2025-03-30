/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:05:26 by asyani            #+#    #+#             */
/*   Updated: 2025/03/30 12:17:03 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int init_table(t_table *table, int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: number_of_philos time_to_die time_to_eat time_to_sleep must_eaten\n");
		return 0;
	}
	table->num_philosophers = atoi(argv[1]);
	table->time_to_die = atoi(argv[2]);
	table->time_to_eat = atoi(argv[3]);
	table->time_to_sleep = atoi(argv[4]);
	if (argv[5])
		table->must_eat_count = atoi(argv[5]);
	if (table->num_philosophers <= 0 || table->time_to_die < 0 || 
		table->time_to_eat < 0 || table->time_to_sleep < 0)
	{
		printf("Invalid input parameters\n");
		return 0;
	}
	table->philosophers = malloc(sizeof(t_philosopher) * table->num_philosophers);
	table->philosopher_threads = malloc(sizeof(pthread_t) * table->num_philosophers);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
	if (!table->philosophers || !table->philosopher_threads || !table->forks)
	{
		printf("Memory allocation failed\n");
		return 0;
	}
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->death_mutex, NULL);
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
		table->philosophers[i].last_meal_time = get_current_time();
		table->philosophers[i].table = table;
		table->philosophers[i].left_fork = &table->forks[i];
		table->philosophers[i].right_fork = &table->forks[(i + 1) % table->num_philosophers];
		i++;
	}
	table->simulation_stop = false;
	table->start_time = get_current_time();
	return (1);
}

void cleanup_table(t_table *table)
{
	int	i;

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

int main(int argc, char **argv)
{
	t_table table;
	int i;

	if (!init_table(&table, argc, argv))
	{
		return 1;
	}
	i = 0;
	while (i < table.num_philosophers)
	{
		if (pthread_create(&table.philosopher_threads[i], NULL, 
		     philosopher_routine, &table.philosophers[i]) != 0)
		{
			printf("Failed to create philosopher thread\n");
			return 1;
		}
		i++;
	}
	if (pthread_create(&table.monitor_thread, NULL, 
		    monitor_routine, &table) != 0)
	{
		printf("Failed to create monitor thread\n");
		return 1;
	}
	i = 0;
	while (i < table.num_philosophers)
	{
		pthread_join(table.philosopher_threads[i], NULL);
		i++;
	}
	pthread_join(table.monitor_thread, NULL);
	cleanup_table(&table);
	return 0;
}
