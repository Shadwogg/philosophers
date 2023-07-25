/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:43:24 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 00:35:28 by ggiboury         ###   ########.fr       */
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
		usleep(1);
		if (update_clock(ctler->philos[0]->timer) != 0)
			return (free_controller(ctler));
	}
	free_controller(ctler);
	return (souls);
}

//VAR to rename
int	init_clock_thread(pthread_t *clock_thread, t_info *info, t_thread *threads)
{
	t_controller	*clock;
	t_thread		*cur;
	unsigned int	ct;

	clock = malloc(sizeof(t_controller));
	if (clock == NULL)
		print_error("Controller null");
	clock->number_philo = info->nb_philos;
	clock->philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		clock->philos[ct] = cur->philo;
		ct++;
		cur = cur->next;
	}
	if (clock->philos == NULL)
		return (1);
	if (pthread_create(clock_thread, NULL, &set_timers, clock) != 0)
		return (1);
	if (pthread_detach(*clock_thread) != 0)
		return (1);
	return (0);
}
