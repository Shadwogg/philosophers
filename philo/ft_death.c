/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_death.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:59:42 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 15:15:53 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	terminate_all(t_philosopher **philos, unsigned int nb)
{
	while (nb-- > 0)
	{
		if (pthread_mutex_lock(philos[nb]->mutex_is_finished) != 0)
			return (-1);
		*((philos[nb])->is_finished) = 1;
		if (pthread_mutex_unlock(philos[nb]->mutex_is_finished) != 0)
			return (-1);
	}
	return (0);
}

int	simulation_is_finished(t_controller *ctler)
{
	int				ct;
	t_philosopher	*cur;
	time_t			start;
	time_t			actual;
	struct timeval	tv;

	ct = -1;
	while ((unsigned int)++ct < ctler->number_philo)
	{
		cur = ctler->philos[ct];
		start = cur->timer->last_eaten.tv_sec * 1000
			+ cur->timer->last_eaten.tv_usec / 1000;
		if (pthread_mutex_lock(cur->timer->mutex_clock) != 0)
			return (-1);
		tv.tv_sec = cur->timer->clock->tv_sec;
		tv.tv_usec = cur->timer->clock->tv_usec;
		if (pthread_mutex_unlock(cur->timer->mutex_clock) != 0)
			return (-1);
		actual = (tv.tv_sec * 1000 + tv.tv_usec / 1000)
			- cur->timer->time_eaten * (cur->menu->tte);
		if (actual - start >= cur->menu->ttd)
			return (ct + 1);
	}
	return (0);
}

int	all_philo_has_eaten(t_philosopher **philos, unsigned int nb)
{
	unsigned int	ct;
	t_timer			*cur;

	ct = 0;
	while (ct < nb)
	{
		cur = philos[ct]->timer;
		if (pthread_mutex_lock(cur->mutex_time_eaten) != 0)
			return (-1);
		if (philos[ct]->menu->times == 0
			|| cur->time_eaten < philos[ct]->menu->times)
		{
			if (pthread_mutex_unlock(cur->mutex_time_eaten) != 0)
				return (-1);
			return (0);
		}
		if (pthread_mutex_unlock(cur->mutex_time_eaten) != 0)
			return (-1);
		ct++;
	}
	return (1);
}

void	*harvest(void *souls)
{
	t_controller	*ctler;
	int				philo;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	philo = simulation_is_finished(ctler);
	while (philo == 0
		&& !all_philo_has_eaten(ctler->philos, ctler->number_philo))
	{
		ft_mlsleep(2, ctler->philos[0]->timer);
		philo = simulation_is_finished(ctler);
	}
	if (philo == -1)
		return (free_controller(ctler));
	if (terminate_all(ctler->philos, ctler->number_philo) == -1)
		return (free_controller(ctler));
	if (philo != 0)
		print_status(ctler->philos[philo - 1], "died", 1);
	free_controller(ctler);
	return (NULL);
}
