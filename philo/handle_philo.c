/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:33:11 by asyani            #+#    #+#             */
/*   Updated: 2025/03/22 10:30:03 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int get_time_ms(t_data *thread)
{
	static int start_time;
	struct timeval time;
	int	current_time;

	gettimeofday(&time, NULL);
	current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	pthread_mutex_lock(thread->mutex);
	if (start_time == 0)
		start_time = current_time;
	pthread_mutex_unlock(thread->mutex);
	return current_time - start_time;
}

void philosopher_eat(t_data *thread)
{
	int	time;
	/*int	last_meal;*/

	time = get_time_ms(thread);
	pthread_mutex_lock(thread->mutex);
	printf("%d %d is eating\n", time, thread->philo.philo_list->philo_id);
	pthread_mutex_unlock(thread->mutex);
	thread->last_meal = get_time_ms(thread);
	usleep(thread->fi_info->time_to_eat * 1000);
}

void philosopher_sleep(t_data *thread)
{
	int	time;

	time = get_time_ms(thread);
	pthread_mutex_lock(thread->mutex);
	printf("%d %d is sleeping\n", time, thread->philo.philo_list->philo_id);
	pthread_mutex_unlock(thread->mutex);
	usleep(thread->fi_info->time_to_sleep * 1000);
}

void take_forks(t_data *thread)
{
	int	time;
	int	left_id;
	int	right_id;

	left_id = thread->philo.philo_list->philo_id - 1;
	right_id = thread->philo.philo_list->philo_id % thread->fi_info->philo_num;
	time = get_time_ms(thread);
	if (left_id < right_id)
	{
		pthread_mutex_lock(thread->left_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
		pthread_mutex_lock(thread->right_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
	}
	else
{
		pthread_mutex_lock(thread->right_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
		pthread_mutex_lock(thread->left_fork);
		printf("%d %d has taken a fork\n", time, thread->philo.philo_list->philo_id);
	}
	philosopher_eat(thread);
	pthread_mutex_unlock(thread->left_fork);
	pthread_mutex_unlock(thread->right_fork);
}

void philosopher_think(t_data *thread)
{
    int time = get_time_ms(thread);
    
    pthread_mutex_lock(thread->mutex);
    printf("%d %d is thinking\n", time, thread->philo.philo_list->philo_id);
    pthread_mutex_unlock(thread->mutex);
}

int check_death(t_data *thread)
{
    int current_time = get_time_ms(thread);
    
    pthread_mutex_lock(&thread->fi_info->died_mutex);
    
    // Check if philosopher has exceeded time to die
    if (current_time - thread->last_meal >= thread->fi_info->time_to_die)
    {
        if (!thread->fi_info->died_flag) {
            printf("%d %d died\n", current_time, thread->philo.philo_list->philo_id);
            thread->fi_info->died_flag = 1;
        }
        pthread_mutex_unlock(&thread->fi_info->died_mutex);
        return 0;  // Death detected
    }
    
    pthread_mutex_unlock(&thread->fi_info->died_mutex);
    return 1;  // Philosopher is alive
}

void *thread_routine(void *arg)
{
    t_data *thread = (t_data *)arg;
    int eat_count = 0;
    
    // Initialize last_meal to the start time
    thread->last_meal = get_time_ms(NULL);
    
    while (1)
    {
        // Check if simulation has ended
        pthread_mutex_lock(&thread->fi_info->died_mutex);
        if (thread->fi_info->died_flag) {
            pthread_mutex_unlock(&thread->fi_info->died_mutex);
            break;
        }
        pthread_mutex_unlock(&thread->fi_info->died_mutex);
        
        // Stagger even and odd philosophers to reduce deadlock
        if (thread->philo.philo_list->philo_id % 2 == 0)
            usleep(1000);
        
        // Take forks and eat
        take_forks(thread);
        
        // Update last meal time
        thread->last_meal = get_time_ms(NULL);
        
        eat_count++;
        
        // Check if max eating times is reached
        if (thread->fi_info->eating_times > 0 && 
            eat_count >= thread->fi_info->eating_times)
        {
            break;
        }
        
        // Sleep
        philosopher_sleep(thread);
        
        // Think
        philosopher_think(thread);
    }
    
    return NULL;
}

void *monitoring_thread(void *arg)
{
    t_head *philo = (t_head *)arg;
    t_data *thread_mutex = philo->thread_mutex;
    int i;

    while (1)
    {
        pthread_mutex_lock(&philo->died_mutex);
        if (philo->died_flag) {
            pthread_mutex_unlock(&philo->died_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->died_mutex);

        for (i = 0; i < philo->philo_num; i++)
        {
            int current_time = get_time_ms(NULL);

            pthread_mutex_lock(&philo->died_mutex);
            
            // Check if philosopher has exceeded time to die
            if (current_time - thread_mutex[i].last_meal >= philo->time_to_die)
            {
                if (!philo->died_flag) {
                    printf("%d %d died\n", current_time, thread_mutex[i].philo.philo_list->philo_id);
                    philo->died_flag = 1;
                }
                pthread_mutex_unlock(&philo->died_mutex);
                return NULL;
            }
            
            pthread_mutex_unlock(&philo->died_mutex);
        }

        // Small sleep to prevent busy waiting
        usleep(1000);
    }

    return NULL;
}

void philo_handler(t_head *philo)
{
    int i;
    pthread_t *thread_id;
    pthread_t monitoring_thread_id;
    pthread_mutex_t mutex;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&philo->died_mutex, NULL);
    
    thread_id = malloc(sizeof(pthread_t) * philo->philo_num);
    philo->thread_mutex = malloc(sizeof(t_data) * philo->philo_num);
    pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * philo->philo_num);
    
    if (!thread_id || !philo->thread_mutex || !forks)
        return;
    
    // Initialize forks
    for (int j = 0; j < philo->philo_num; j++)
    {
        pthread_mutex_init(&forks[j], NULL);
    }
    
    philo->died_flag = 0;    
    
    // Create threads
    for (i = 0; i < philo->philo_num; i++)
    {
        philo->thread_mutex[i].id = i + 1;
        philo->thread_mutex[i].mutex = &mutex;
        philo->thread_mutex[i].fi_info = philo;
        philo->thread_mutex[i].left_fork = &forks[i];
        philo->thread_mutex[i].right_fork = &forks[(i + 1) % philo->philo_num]; 
        philo->thread_mutex[i].philo.philo_list = NULL;
        
        pthread_mutex_lock(philo->thread_mutex[i].mutex); 
        add_to_list(&philo->thread_mutex[i].philo.philo_list, philo->thread_mutex[i].id);
        pthread_mutex_unlock(philo->thread_mutex[i].mutex); 
        
        philo->thread_mutex[i].philo.philo_list->time_to_die = philo->time_to_die;
        philo->thread_mutex[i].philo.philo_list->time_to_eat = philo->time_to_eat;
        philo->thread_mutex[i].philo.philo_list->time_to_sleep = philo->time_to_sleep;
        philo->thread_mutex[i].philo.philo_list->eating_times = philo->eating_times;
        
        pthread_create(&thread_id[i], NULL, thread_routine, &philo->thread_mutex[i]);
    } 
    
    // Create monitoring thread
    pthread_create(&monitoring_thread_id, NULL, monitoring_thread, philo);
    
    // Wait for all threads to complete
    for (i = 0; i < philo->philo_num; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    
    // Wait for monitoring thread
    pthread_join(monitoring_thread_id, NULL);
    
    // Cleanup
    for (i = 0; i < philo->philo_num; i++)
    {
        t_philo *current = philo->thread_mutex[i].philo.philo_list;
        while (current)
        {
            t_philo *temp = current;
            current = current->next;
            free(temp);
        }
        pthread_mutex_destroy(&forks[i]);
    }
    
    // Destroy mutexes and free memory
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&philo->died_mutex);
    
    free(thread_id);
    free(philo->thread_mutex);
    free(forks);
}
/*void *thread_routine(void *arg)*/
/*{*/
/*    t_data *thread = (t_data *)arg;*/
/*    int eat_count = 0;*/
/**/
/*    // Initialize last_meal to the start time*/
/*    thread->last_meal = get_time_ms(thread);*/
/**/
/*    while (1)*/
/*    {*/
/*        // Check if simulation has ended*/
/*        pthread_mutex_lock(&thread->fi_info->died_mutex);*/
/*        if (thread->fi_info->died_flag) {*/
/*            pthread_mutex_unlock(&thread->fi_info->died_mutex);*/
/*            break;*/
/*        }*/
/*        pthread_mutex_unlock(&thread->fi_info->died_mutex);*/
/**/
/*        // Stagger even and odd philosophers to reduce deadlock*/
/*        if (thread->philo.philo_list->philo_id % 2 == 0)*/
/*            usleep(1000);*/
/**/
/*        // Take forks and eat*/
/*        take_forks(thread);*/
/*        eat_count++;*/
/**/
/*        // Check if max eating times is reached*/
/*        if (thread->fi_info->eating_times > 0 && */
/*            eat_count >= thread->fi_info->eating_times)*/
/*        {*/
/*            break;*/
/*        }*/
/**/
/*        // Check for death after eating*/
/*        if (!check_death(thread))*/
/*            break;*/
/**/
/*        // Sleep*/
/*        philosopher_sleep(thread);*/
/**/
/*        // Check for death after sleeping*/
/*        if (!check_death(thread))*/
/*            break;*/
/**/
/*        // Think*/
/*        philosopher_think(thread);*/
/**/
/*        // Final death check*/
/*        if (!check_death(thread))*/
/*            break;*/
/*    }*/
/**/
/*    return NULL;*/
/*}*/
/**/
/*void	philo_handler(t_head *philo)*/
/*{*/
/*	int i;*/
/*	pthread_t *thread_id;*/
/*	t_data *thread_mutex;*/
/*	pthread_mutex_t mutex;*/
/**/
/*	i = 0;*/
/*	pthread_mutex_init(&mutex, NULL);*/
/*	pthread_mutex_init(&philo->died_mutex, NULL);*/
/*	thread_id = malloc(sizeof(pthread_t) * philo->philo_num);*/
/*	thread_mutex = malloc(sizeof(	t_data) * philo->philo_num);*/
/**/
/*	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * philo->philo_num);*/
/**/
/*	if (!thread_id || !thread_mutex || !forks)*/
/*		return;*/
/*	int j = 0;*/
/*	while (j < philo->philo_num)*/
/*	{*/
/*		pthread_mutex_init(&forks[j], NULL);*/
/*		j++;*/
/*	}*/
/*	philo->died_flag = 0;    */
/*	while (i < philo->philo_num)*/
/*	{*/
/*		thread_mutex[i].id = i + 1;*/
/*		thread_mutex[i].mutex = &mutex;*/
/*		thread_mutex[i].fi_info = philo;*/
/**/
/*		thread_mutex[i].left_fork = &forks[i];*/
/*		thread_mutex[i].right_fork = &forks[(i + 1) % philo->philo_num]; */
/**/
/*		thread_mutex[i].philo.philo_list = NULL;*/
/*		pthread_mutex_lock(thread_mutex[i].mutex); */
/*		add_to_list(&thread_mutex[i].philo.philo_list, thread_mutex[i].id);*/
/*		pthread_mutex_unlock(thread_mutex[i].mutex); */
/*		thread_mutex[i].philo.philo_list->time_to_die = philo->time_to_die;*/
/*		thread_mutex[i].philo.philo_list->time_to_eat = philo->time_to_eat;*/
/*		thread_mutex[i].philo.philo_list->time_to_sleep = philo->time_to_sleep;*/
/*		thread_mutex[i].philo.philo_list->eating_times = philo->eating_times;*/
/**/
/*		pthread_create(&thread_id[i], NULL, thread_routine, &thread_mutex[i]);*/
/*		i++;*/
/*	} */
/*	i = 0;*/
/*	while (i < philo->philo_num)*/
/*	{*/
/*		pthread_join(thread_id[i], NULL);*/
/*		i++;*/
/*	}*/
/**/
/*	i = 0;*/
/*	while (i < philo->philo_num)*/
/*	{*/
/*		t_philo *current = thread_mutex[i].philo.philo_list;*/
/*		while (current)*/
/*		{*/
/*			t_philo *temp = current;*/
/*			current = current->next;*/
/*			free(temp);*/
/*		}*/
/*		i++;*/
/*	}*/
/**/
/*	pthread_mutex_destroy(&mutex);*/
/*	free(thread_id);*/
/*	free(thread_mutex);*/
/*	free(forks);*/
/*}*/
