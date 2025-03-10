/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:47:18 by asyani            #+#    #+#             */
/*   Updated: 2025/03/10 15:22:52 by asyani           ###   ########.fr       */
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


int	main(int ac, char **av)
{
	int	i;
	int	j;
	int	num;

	if (ac > 6)
		return (1);
	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
			{
				printf("Error:\n");
				return (1);
			}
			j++;
		}
		num = ft_atoi(av[i]);
		i++;
	}
	printf("%d ", num);
	return (0);
}
