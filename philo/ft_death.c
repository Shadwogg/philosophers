/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_death.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:59:42 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:08:56 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_death(pthread_t *ending_thread, t_info *info, t_thread *threads)
{
	t_controller	*ctler;

	ctler = malloc(sizeof(t_controller));
	if (ctler == NULL)
		return (p_error("Death failed to be allocated"), -1);
	if (init_controller(ctler, info, threads) != 0)
		return (-1);
	ctler->mutex_philo_dead = NULL;
	if (pthread_create(ending_thread, NULL, &harvest, ctler) != 0)
		return (p_error("Death thread failed to be created.\n"), -1);
	if (pthread_detach(*ending_thread) != 0)
		return (p_error("Death thread failed to be detached.\n"), -1);
	return (0);
}

int	terminate_all(t_philosopher **philos, unsigned int nb)
{
	while (nb-- > 0)
	{
		if (pthread_mutex_lock(philos[nb]->mutex_is_finished) != 0)
			return (p_error("m_is_finished failed to be locked.\n"), -1);
		*((philos[nb])->is_finished) = 1;
		if (pthread_mutex_unlock(philos[nb]->mutex_is_finished) != 0)
			return (p_error("m_is_finished failed to be unlocked.\n"), -1);
	}
	return (0);
}

int	simulation_is_finished(t_controller *ctler)
{
	int				ct;
	t_philosopher	*cur;
	time_t			start;
	time_t			actual;

	ct = -1;
	while ((unsigned int)++ct < ctler->number_philo)
	{
		cur = ctler->philos[ct];
		if (pthread_mutex_lock(cur->timer.mutex_timer) != 0)
			return (p_error("M_timer failed to be locked.\n"), -1);
		start = cur->timer.last_eaten.tv_sec * 1000
			+ cur->timer.last_eaten.tv_usec / 1000;
		if (pthread_mutex_lock(cur->timer.mutex_clock) != 0)
			return (p_error("M_clock failed to be locked.\n"), -1);
		actual = (cur->timer.clock->tv_sec * 1000 + cur->timer.clock->tv_usec
				/ 1000);
		if (pthread_mutex_unlock(cur->timer.mutex_clock) != 0)
			return (p_error("M_clock failed to be unlocked.\n"), -1);
		if (pthread_mutex_unlock(cur->timer.mutex_timer) != 0)
			return (p_error("M_timer failed to be locked.\n"), -1);
		if (actual - start >= cur->menu.ttd)
			return (ct + 1);
	}
	return (0);
}

int	all_philo_has_eaten(t_philosopher **philos, unsigned int nb)
{
	unsigned int	ct;
	t_timer			cur;

	ct = 0;
	while (ct < nb)
	{
		cur = philos[ct]->timer;
		if (pthread_mutex_lock(cur.mutex_timer) != 0)
			return (p_error("m_timer failed to be locked.\n"), -1);
		if (philos[ct]->menu.times == 0
			|| cur.time_eaten < philos[ct]->menu.times)
		{
			if (pthread_mutex_unlock(cur.mutex_timer) != 0)
				return (p_error("m_timer failed to be unlocked.\n"), -1);
			return (0);
		}
		if (pthread_mutex_unlock(cur.mutex_timer) != 0)
			return (p_error("m_timer failed to be locked.\n"), -1);
		ct++;
	}
	return (1);
}

void	*harvest(void *souls)
{
	t_controller	*ctler;
	int				philo;
	int				has_eaten;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	philo = simulation_is_finished(ctler);
	has_eaten = all_philo_has_eaten(ctler->philos, ctler->number_philo);
	while (philo == 0 && has_eaten == 0)
	{
		if (ft_mlsleep(2, ctler->philos[0]->timer) == -1)
			return (free_controller(ctler), NULL);
		philo = simulation_is_finished(ctler);
		has_eaten = all_philo_has_eaten(ctler->philos, ctler->number_philo);
	}
	if (philo == -1 || has_eaten == -1)
		return (free_controller(ctler), NULL);
	if (terminate_all(ctler->philos, ctler->number_philo) != 0)
		return (free_controller(ctler), NULL);
	if (philo != 0)
		if (print_status(ctler->philos[philo - 1], "died", 1) != 0)
			return (NULL);
	free_controller(ctler);
	return (NULL);
}
