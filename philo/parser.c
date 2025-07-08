/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:07:42 by asyani            #+#    #+#             */
/*   Updated: 2025/07/07 16:18:24 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isalpha(argv[i][j]))
			{
				printf("Error: enter just number\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_atoi(char *str, t_table *table)
{
	int		i;
	int	sign;
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

void	cleanup_table(t_table *table)
{
	int	i;

	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->death_mutex);
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philosophers[i].times_eaten_mutex);
		i++;
	}
	free(table->philosophers);
	free(table->philosopher_threads);
	free(table->forks);
}

int	thread_fail(void)
{
	printf("Failed to create philosopher thread\n");
	return (1);
}
