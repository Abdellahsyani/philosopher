/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyani <asyani@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 07:11:24 by asyani            #+#    #+#             */
/*   Updated: 2025/03/06 07:33:54 by asyani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#define TIME 21000

typedef struct s_counter
{
	pthread_mutex_t counter_mutex;
	unsigned int count;
} t_counter;

void	*thread_routine(void *data)
{
	pthread_t tid;
	t_counter *counter = (t_counter *)data;
	unsigned int	i;

	tid = pthread_self();
	pthread_mutex_lock(&counter->counter_mutex);
	printf("The start of thread here: [%d]\n", counter->count);
	pthread_mutex_unlock(&counter->counter_mutex);
	i = 0;
	while (i < TIME)
	{
		pthread_mutex_lock(&counter->counter_mutex);
		counter->count++;
		pthread_mutex_unlock(&counter->counter_mutex);
		i++;
	}

	pthread_mutex_lock(&counter->counter_mutex);
	printf("Thread final count: [%d]\n", counter->count);
	pthread_mutex_unlock(&counter->counter_mutex);
	return (NULL);
}

int	main(void)
{
	pthread_t th_1;
	pthread_t th_2;

	t_counter counter;

	counter.count = 0;

	pthread_mutex_init(&counter.counter_mutex, NULL);

	printf("Main: exepected count is : %d\n", 2 * TIME);

	pthread_create(&th_1, NULL, thread_routine, &counter);
	printf("create the first thread: [%ld]\n", th_1);

	pthread_create(&th_2, NULL, thread_routine, &counter);
	printf("create the first thread: [%ld]\n", th_2);

	pthread_join(th_1, NULL);
	printf("the thread joined %ld\n", th_1);

	pthread_join(th_2, NULL);
	printf("the thread joined %ld\n", th_2);

	if (counter.count != (2 * TIME))
		printf("Error: total count is: %u\n", counter.count);
	else
		printf("Main: ok total count is: %u\n", counter.count);
	pthread_mutex_destroy(&counter.counter_mutex);

	return(0);
}
