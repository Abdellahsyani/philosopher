/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:48:32 by asyani            #+#    #+#             */
/*   Updated: 2025/03/22 09:51:20 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>

// Philosopher structure
typedef struct s_philosopher {
    int id;
    int times_eaten;
    long long last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t times_eaten_mutex;
    struct s_table *table;
} t_philosopher;

// Table structure to manage overall simulation
typedef struct s_table {
    int num_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int must_eat_count;
    
    // Synchronization primitives
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    
    // Simulation state
    bool simulation_stop;
    
    // Philosophers array
    t_philosopher *philosophers;
    long long start_time;
    
    // Threads
    pthread_t *philosopher_threads;
    pthread_t monitor_thread;
} t_table;

// Function prototypes
int init_table(t_table *table, int argc, char **argv);
void cleanup_table(t_table *table);
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);
long long get_current_time(void);
void precise_sleep(long long milliseconds);
void print_status(t_table *table, int philosopher_id, const char *status);

#endif // PHILOSOPHERS_H
