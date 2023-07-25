/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:43:24 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/25 19:00:54 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	update_clock(t_timer *clock)
{
	if (pthread_mutex_lock(clock->mutex_clock) != 0)
		return (-1);
	if (gettimeofday(clock->clock, NULL) != 0)
		return (-1);
	if (pthread_mutex_unlock(clock->mutex_clock) != 0)
		return (-1);
	return (0);
}

void	*set_timers(void *souls)
{
	t_controller	*ctler;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	while (!simulation_is_finished(ctler))
	{
		ft_mlsleep(1, ctler->philos[0]->timer);
		if (update_clock(ctler->philos[0]->timer) != 0)
			return (free_controller(ctler));
	}
	free_controller(ctler);
	return (souls);
}

//VAR to rename
int	init_clock_thread(pthread_t *watch_thread, t_info *info, t_thread *threads)
{
	t_controller	*watch;
	t_thread		*cur;
	unsigned int	ct;

	watch = malloc(sizeof(t_controller));
	if (watch == NULL)
		print_error("Controller null");
	watch->number_philo = info->nb_philos;
	watch->philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		watch->philos[ct] = cur->philo;
		ct++;
		cur = cur->next;
	}
	if (watch->philos == NULL)
		return (1);
	if (pthread_create(watch_thread, NULL, &set_timers, watch) != 0)
		return (1);
	if (pthread_detach(*watch_thread) != 0)
		return (1);
	return (0);
}
