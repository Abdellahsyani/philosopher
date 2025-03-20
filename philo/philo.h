/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:48:32 by asyani            #+#    #+#             */
/*   Updated: 2025/03/09 13:50:15 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H


# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>


typedef struct s_philo
{
	int	philo_id;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	eating_times;
	int	num_of_forks;
	struct s_philo *next;
	struct s_philo	*philo_list;
	struct	timeval	last_meal;
} t_philo;

typedef struct s_head
{
	int	philo_num;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	eating_times;
} t_head;

typedef struct s_data
{
	int	id;
	pthread_mutex_t	*mutex;
	t_philo	philo;
	t_head	fi_info;
} t_data;

void	philo_handler(t_head *philo);
void	*thread_routine(void *arg);

#endif
