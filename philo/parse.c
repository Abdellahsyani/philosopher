/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:51 by asyani            #+#    #+#             */
/*   Updated: 2025/03/13 16:33:07 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(char *str)
{
	int		i;
	int		sign;
	int	res;

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
		/*if (res >= INT_MAX)*/
		/*	exit_failure("Invalid PID\n");*/
		i++;
	}
	return (res * sign);
}

static void	fill_args(char **av)
{
	t_head	all_arg;

	all_arg.philo_num = 0;
	all_arg.time_to_die = 0;
	all_arg.time_to_eat = 0;
	all_arg.time_to_sleep = 0;
	if (av[4])
	{
		all_arg.philo_num = ft_atoi(av[1]);
		all_arg.time_to_die = ft_atoi(av[2]);
		all_arg.time_to_eat = ft_atoi(av[3]);
		all_arg.time_to_sleep = ft_atoi(av[4]);
	}
	else
		printf("Error: You must enter at least 4 args\n");
	printf("philo_num: %d\n", all_arg.philo_num);
	printf("time_to_die: %d\n", all_arg.time_to_die);
	printf("time_to_eat: %d\n", all_arg.time_to_eat);
	printf("time_to_sleep: %d\n", all_arg.time_to_sleep);
	if (av[5])
	{
		all_arg.eating_times = ft_atoi(av[5]);
		printf("eating_times: %d\n", all_arg.eating_times);
	}
	philo_handler(&all_arg);
}

int	main(int ac, char **av)
{
	int	i;
	int	j;

	if (ac > 6)
	{
		printf("Error: Enter just 5 arguments\n");
		return (1);
	}
	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
			{
				printf("Error: some of args contain non integer\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	fill_args(av);
	return (0);
}
