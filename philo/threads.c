/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 22:08:47 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 19:03:28 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_thread	*init_threads(t_info *info, pthread_mutex_t **forks)
{
	unsigned int	ct;
	t_thread		*threads;
	t_thread		*cur;

	threads = malloc(sizeof(t_thread));
	if (threads == NULL)
		return (print_error("Threads failed to be allocated.\n"), NULL);
	threads->next = NULL;
	threads->numero = 1;
	threads->philo = NULL;
	ct = 1;
	while (ct++ < info->nb_philos)
		add_list(&threads);
	if (threads == NULL)
		return (print_error("Threads failed to be allocated.\n"), NULL);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		cur->philo = init_philosopher(info, forks, ct++);
		if (cur->philo == NULL)
			return (free_threads(threads), NULL);
		cur = cur->next;
	}
	return (threads);
}

int	synchronize_philosophers(t_thread *threads)
{
	pthread_mutex_t	*mutex_turn;
	pthread_mutex_t	*mutex_clock;
	struct timeval	*clock;

	mutex_turn = malloc(sizeof(pthread_mutex_t));
	if (mutex_turn == NULL)
		return (print_error("Mutex turn failed to be allocated."), -1);
	if (pthread_mutex_init(mutex_turn, NULL) != 0)
		return (print_error("Mutex turn failed to be initialised."), -1);
	mutex_clock = malloc(sizeof(pthread_mutex_t));
	if (mutex_clock == NULL)
		return (print_error("Mutex clock failed to be allocated."), -1);
	if (pthread_mutex_init(mutex_clock, NULL) != 0)
		return (print_error("Mutex clock failed to be initialised."), -1);
	clock = malloc(sizeof(struct timeval));
	if (clock == NULL)
		return (-1);
	while (threads != NULL)
	{
		threads->philo->timer.mutex_clock = mutex_clock;
		threads->philo->timer.clock = clock;
		threads->philo->turn = mutex_turn;
		threads = threads->next;
	}
	return (0);
}

// Verify that each thread ended correctly, if not return 0.
int	verify_threads(t_thread *threads, t_controller *clock)
{
	t_thread		*cur;
	void			*returned_philo;
	char			err;

	cur = threads;
	err = 1;
	returned_philo = NULL;
	while (cur != NULL)
	{
		if (pthread_join(cur->thread, &returned_philo) != 0)
			printf("TO IMPLEMENT");
		if (returned_philo == NULL)
			err = 0;
		else
			free_philo(returned_philo);
		if (pthread_mutex_lock(clock->mutex_philo_dead) != 0)
			return (-1);
		clock->philo_dead++;
		if (pthread_mutex_unlock(clock->mutex_philo_dead) != 0)
			return (-1);
		cur = cur->next;
	}
	return (err);
}

int	launch_threads(t_info *info, t_thread *threads)
{
	unsigned int	ct;
	t_thread		*cur;

	ct = 0;
	cur = threads;
	while (cur != NULL)
	{
		if (pthread_create(&cur->thread, NULL, &live, cur->philo) == -1)
		{
			free_print("Thread failed to be initialized.", info, threads, NULL);
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}
