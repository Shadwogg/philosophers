/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 22:08:47 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:45:14 by ggiboury         ###   ########.fr       */
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
		return (p_error("Threads failed to be allocated.\n"), NULL);
	threads->next = NULL;
	threads->numero = 1;
	threads->philo = NULL;
	ct = 1;
	while (ct++ < info->nb_philos)
		add_list(&threads);
	if (threads == NULL)
		return (p_error("Threads failed to be allocated.\n"), NULL);
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

static int	free_print(pthread_mutex_t *m_turn, pthread_mutex_t *m_clock, int c)
{
	if (c == 0)
		return (free(m_turn), p_error("M_turn failed to be initialised.\n"));
	else if (c == 1)
	{
		pthread_mutex_destroy(m_turn);
		free(m_turn);
		p_error("M_clock failed to be allocated.\n");
	}
	else if (c == 2)
	{
		pthread_mutex_destroy(m_turn);
		free(m_turn);
		free(m_clock);
		p_error("M_clock failed to be initialised.");
	}
	else if (c == 3)
	{
		pthread_mutex_destroy(m_turn);
		free(m_turn);
		pthread_mutex_destroy(m_clock);
		free(m_clock);
		p_error("M_clock failed to be initialised.");
	}
	return (-1);
}

int	synchronize_philosophers(t_thread *threads)
{
	pthread_mutex_t	*m_turn;
	pthread_mutex_t	*m_clock;
	struct timeval	*clock;

	m_turn = malloc(sizeof(pthread_mutex_t));
	if (m_turn == NULL)
		return (p_error("M_turn failed to be allocated.\n"), -1);
	if (pthread_mutex_init(m_turn, NULL) != 0)
		return (free_print(m_turn, NULL, 0));
	m_clock = malloc(sizeof(pthread_mutex_t));
	if (m_clock == NULL)
		return (free_print(m_turn, NULL, 1));
	if (pthread_mutex_init(m_clock, NULL) != 0)
		return (free_print(m_turn, m_clock, 2));
	clock = malloc(sizeof(struct timeval));
	if (clock == NULL)
		return (free_print(m_turn, m_clock, 3));
	while (threads != NULL)
	{
		threads->philo->timer.mutex_clock = m_clock;
		threads->philo->timer.clock = clock;
		threads->philo->turn = m_turn;
		threads = threads->next;
	}
	return (0);
}

// Verify that each thread ended correctly, if not return 0.
int	verify_threads(t_thread *threads, t_controller *clock)
{
	t_thread		*cur;
	t_thread		*to_free;
	t_philosopher	*returned_philo;

	cur = threads;
	returned_philo = NULL;
	while (cur != NULL)
	{
		if (pthread_join(cur->thread, (void *) &returned_philo) != 0)
			return (p_error("Failed to join one philosopher.\n"), -1);
		if (returned_philo == NULL)
			return (p_error("An error occured\n"), -1);
		if (pthread_mutex_lock(clock->mutex_philo_dead) != 0)
			return (p_error("m_philo_dead failed to be locked.\n"), -1);
		clock->philo_dead++;
		if (clock->philo_dead == clock->number_philo)
			end_shared_ressources(returned_philo);
		free_philo(returned_philo);
		if (pthread_mutex_unlock(clock->mutex_philo_dead) != 0)
			return (p_error("m_philo_dead failed to be unlocked.\n"), -1);
		to_free = cur;
		cur = cur->next;
		free(to_free);
	}
	return (1);
}

int	launch_threads(t_thread *threads)
{
	unsigned int	ct;
	t_thread		*cur;

	ct = 0;
	cur = threads;
	while (cur != NULL)
	{
		if (pthread_create(&cur->thread, NULL, &live, cur->philo) == -1)
			return (p_error("Thread (philo) failed to be created.\n"), -1);
		cur = cur->next;
	}
	return (0);
}
