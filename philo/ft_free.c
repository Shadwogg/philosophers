/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:03:35 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:34:41 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_forks(pthread_mutex_t **forks, unsigned int nb)
{
	unsigned int	ct;

	ct = 0;
	while (ct < nb)
	{
		pthread_mutex_destroy(forks[ct]);
		free(forks[ct++]);
	}
	free(forks);
}

void	free_threads(t_thread *t)
{
	t_thread	*next;

	while (t->next != NULL)
	{
		next = t->next;
		free_philo(t->philo);
		free(t);
		t = next;
	}
	free(t);
}

void	free_timer(t_timer timer)
{
	if (timer.mutex_timer != NULL)
		pthread_mutex_destroy(timer.mutex_timer);
	free(timer.mutex_timer);
}

void	free_philo(t_philosopher *philo)
{
	free_timer(philo->timer);
	free(philo->is_finished);
	if (philo->mutex_is_finished != NULL)
		pthread_mutex_destroy(philo->mutex_is_finished);
	free(philo->mutex_is_finished);
	free(philo);
}

void	*free_controller(t_controller *ctler)
{
	free(ctler->philos);
	free(ctler);
	return (ctler);
}
