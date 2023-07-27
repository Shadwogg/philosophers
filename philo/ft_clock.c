/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:43:24 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:59:11 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	update_clock(pthread_mutex_t *m_clock, struct timeval *clock)
{
	if (pthread_mutex_lock(m_clock) != 0)
		return (p_error("M_clock failed to be locked.\n"), -1);
	if (gettimeofday(clock, NULL) != 0)
		return (p_error("Can't get time.\n"), -1);
	if (pthread_mutex_unlock(m_clock) != 0)
		return (p_error("M_clock failed to be unlocked.\n"), -1);
	return (0);
}

int	clock_is_on(t_controller *ctler)
{
	int	is_on;

	is_on = 1;
	if (pthread_mutex_lock(ctler->mutex_philo_dead) != 0)
		return (p_error("M_philo_dead failed to be locked.\n"), -1);
	if (ctler->philo_dead == ctler->number_philo)
		is_on = 0;
	if (pthread_mutex_unlock(ctler->mutex_philo_dead) != 0)
		return (p_error("M_clock failed to be unlocked.\n"), -1);
	return (is_on);
}

// I need to keep track of clock and m_clock to free them at the end.
void	*set_timers(void *souls)
{
	t_controller	*ctler;
	pthread_mutex_t	*m_clock;
	struct timeval	*clock;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	m_clock = ctler->philos[ctler->number_philo - 1]->timer.mutex_clock;
	if (pthread_mutex_lock(m_clock) != 0)
		return (p_error("Failed to lock m_clock.\n"), NULL);
	clock = ctler->philos[ctler->number_philo - 1]->timer.clock;
	if (pthread_mutex_unlock(m_clock) != 0)
		return (p_error("Failed to unlock m_clock.\n"), NULL);
	while (clock_is_on(ctler))
	{
		usleep(1);
		if (!clock_is_on(ctler))
			break ;
		if (update_clock(m_clock, clock) != 0)
			return (free_controller(ctler));
	}
	end_clock(ctler->mutex_philo_dead, m_clock, clock);
	free_controller(ctler);
	return (souls);
}

int	init_clock_thread(pthread_t *clock_thread, t_controller **ref,
	t_info *info, t_thread *threads)
{
	t_controller	*clock;

	clock = malloc(sizeof(t_controller));
	if (clock == NULL)
		return (p_error("Clock failed to be allocated.\n"), -1);
	if (init_controller(clock, info, threads) != 0)
		return (-1);
	clock->philo_dead = 0;
	clock->mutex_philo_dead = malloc(sizeof(pthread_mutex_t));
	if (clock->mutex_philo_dead == NULL)
		return (p_error("M_philo_dead failed to be allocated.\n"), -1);
	if (pthread_mutex_init(clock->mutex_philo_dead, NULL) != 0)
		return (p_error("M_philo_dead failed to be initialised.\n"), -1);
	if (pthread_create(clock_thread, NULL, &set_timers, clock) != 0)
		return (p_error("Clock thread failed to be create.\n"), -1);
	if (pthread_detach(*clock_thread) != 0)
		return (p_error("Clock thread failed to be detached.\n"), -1);
	*ref = clock;
	return (0);
}

void	end_clock(pthread_mutex_t *m_philo_dead, pthread_mutex_t *m_clock,
	struct timeval *clock)
{
	pthread_mutex_destroy(m_philo_dead);
	free(m_philo_dead);
	pthread_mutex_destroy(m_clock);
	free(m_clock);
	free(clock);
}
