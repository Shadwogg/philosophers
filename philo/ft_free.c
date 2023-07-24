/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:03:35 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/24 19:04:55 by ggiboury         ###   ########.fr       */
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
	if (forks[nb] != NULL)
		free(forks[nb]);
	free(forks);
}

void	free_threads(t_thread *t)
{
	t_thread	*next;

	while (t->next != NULL)
	{
		next = t->next;
		free(t);
		t = next;
	}
	free(t);
}

void	free_philo(t_philosopher *philo)
{
	free(philo->menu);
	free(philo->timer);
	free(philo->is_finished);
	pthread_mutex_destroy(philo->m_is_finished);
	free(philo->m_is_finished);
	free(philo);
}

void	free_print(char *str, t_info *p, t_thread *t, pthread_mutex_t **f)
{
	free_forks(f, p->nb_philos);
	free_threads(t);
	free(p);
	print_error(str);
}

void	free_controller(t_controller *ctler)
{
	free(ctler);
}
