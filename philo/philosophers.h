/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:00:35 by asyani            #+#    #+#             */
/*   Updated: 2025/07/07 16:18:21 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher
{
	size_t			id;
	size_t			times_eaten;
	size_t			last_meal_time;
	bool			must_eat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	times_eaten_mutex;
	struct s_table	*table;
}					t_philosopher;

typedef struct s_table
{
	size_t			num_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			must_eat_count;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	start_time_mutex;

	bool			simulation_stop;
	bool			finish_meals;
	bool			stop_atoi;

	t_philosopher	*philosophers;
	size_t			start_time;

	pthread_t		*philosopher_threads;
	pthread_t		monitor_thread;
}					t_table;

int					init_table(t_table *table, int argc, char **argv);
void				cleanup_table(t_table *table);
void				*philosopher_routine(void *arg);
void				*monitor_routine(void *arg);
size_t				get_current_time(void);
void				precise_sleep(size_t milliseconds, t_table *table);
void				print_status(t_table *table, size_t philosopher_id,
						const char *status);
void				philo_sleep(t_philosopher *philo);
void				philo_think(t_philosopher *philo);
void				philo_eat(t_philosopher *philo);
void				check_died(t_table *table, size_t current_time, int i);
int					ft_atoi(char *str, t_table *table);
int					check_args(int argc, char **argv);
void				cleanup_table(t_table *table);
int					thread_fail(void);

#endif
