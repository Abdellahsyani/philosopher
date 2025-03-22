/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/03/22 10:10:51 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	take_right_fork(int philo_id)
{
	int	left_fork;
	int	right_fork;

	if ()
}

void	take_left_fork(int philo_id)
{
}

void	*thread_routine(void *arg)
{
	t_data	*thread;
	struct timeval	start_time;
	struct timeval	current_time;
	int	time;

	thread = (t_data *)arg;
	gettimeofday(&start_time, NULL);
	while (1)
	{
		pthread_mutex_lock(thread->fi_info->died_mutex);
		if (thread->fi_info->died_flag == 1)
		{
			pthread_mutex_unlock(thread->fi_info->died_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(thread->fi_info->died_mutex);

		// Start counting the time
		gettimeofday(&current_time, NULL);
		time = get_time();
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
		pthread_mutex_lock(thread->mutex); 
		thread->philo.philo_list->num_of_forks++;
		pthread_mutex_unlock(thread->mutex); 

		if (thread->philo.philo_list->num_of_forks == 2)
		{
			// Update the meal time
			gettimeofday(&current_time, NULL);
			elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1000 + 
				(current_time.tv_usec - start_time.tv_usec) / 1000;

			printf("%ld %d is eating\n", elapsed_time, thread->philo.philo_list->philo_id);
			usleep(thread->philo.philo_list->time_to_eat * 1000);
			pthread_mutex_lock(thread->mutex); 
			thread->philo.philo_list->num_of_forks = 0;
			pthread_mutex_unlock(thread->mutex); 

			gettimeofday(&thread->philo.philo_list->last_meal, NULL);
			elapsed = (thread->philo.philo_list->last_meal.tv_sec - start_time.tv_sec) * 1000 + 
				(thread->philo.philo_list->last_meal.tv_usec - start_time.tv_usec) / 1000;	
			printf("The last meal_time %ld of philo %d\n", elapsed, thread->philo.philo_list->philo_id);
			printf("%ld %d is sleeping\n", elapsed_time, thread->philo.philo_list->philo_id);
			usleep(thread->philo.philo_list->time_to_sleep * 1000);
		}

		gettimeofday(&current_time, NULL);
		pthread_mutex_lock(thread->mutex); 
		time_since_last_meal = (current_time.tv_sec - thread->philo.philo_list->last_meal.tv_sec) * 1000 + 
			(current_time.tv_usec - thread->philo.philo_list->last_meal.tv_usec) / 1000;
		pthread_mutex_unlock(thread->mutex); 

		if (time_since_last_meal > thread->philo.philo_list->time_to_die)
		{
			gettimeofday(&current_time, NULL);
			died = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
			printf("%ld %d is died\n", died, thread->philo.philo_list->philo_id);
			pthread_mutex_lock(thread->fi_info->died_mutex);
			thread->fi_info->died_flag = 1;
			pthread_mutex_unlock(thread->fi_info->died_mutex);
			return (NULL);
		}
	}

	return (NULL);
}

void	philo_handler(t_head *philo)
{
	int	i;
	pthread_t	*thread_id;
	t_data	*thread_mutex;
	pthread_mutex_t	mutex;
	/*int	died_flag = 0;*/

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	philo->died_mutex = malloc(sizeof(pthread_mutex_t));
	if (!philo->died_mutex)
	{
		fprintf(stderr, "Error: Failed to allocate memory for died_mutex\n");
		exit(1);
	}
	pthread_mutex_init(philo->died_mutex, NULL);
	pthread_mutex_init(philo->died_mutex, NULL);
	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
	thread_mutex = malloc(sizeof(t_data) * philo->philo_num);
	if (!thread_id || !thread_mutex)
		return ;
	philo->died_flag = 0;	
	while (i < philo->philo_num)
	{
		thread_mutex[i].id = i + 1;
		thread_mutex[i].mutex = &mutex;
		thread_mutex[i].fi_info = philo;

		// Initialize the philo_list to NULL before adding to it
		thread_mutex[i].philo.philo_list = NULL;

		// Add the philosopher to the list
		pthread_mutex_lock(thread_mutex[i].mutex); 
		add_to_list(&thread_mutex[i].philo.philo_list, thread_mutex[i].id);
		pthread_mutex_unlock(thread_mutex[i].mutex); 

		// Copy timing parameters from the main philo structure
		thread_mutex[i].philo.philo_list->time_to_die = philo->time_to_die;
		thread_mutex[i].philo.philo_list->time_to_eat = philo->time_to_eat;
		thread_mutex[i].philo.philo_list->time_to_sleep = philo->time_to_sleep;
		thread_mutex[i].philo.philo_list->eating_times = philo->eating_times;

		pthread_create(&thread_id[i], NULL, thread_routine, &thread_mutex[i]);
		i++;
	}

	i = 0;
	while (i < philo->philo_num)
	{
		pthread_join(thread_id[i], NULL);
		i++;
	}

	// Free all allocated memory for philosophers
	i = 0;
	while (i < philo->philo_num)
	{
		// Free linked list for each philosopher
		t_philo *current = thread_mutex[i].philo.philo_list;
		while (current)
		{
			t_philo *temp = current;
			current = current->next;
			free(temp);
		}
		i++;
	}

	pthread_mutex_destroy(&mutex);
	free(thread_id);
	free(thread_mutex);
}
