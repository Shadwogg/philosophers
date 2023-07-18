/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:03:35 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 21:52:42 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	free_forks(pthread_mutex_t **f, unsigned int nb)
{
	unsigned int	ct;

	ct = 0;
	while (ct < nb)
	{
		pthread_mutex_destroy(f[ct]);
		free(f[ct++]);
	}
	free(f);
}

void	free_table(t_philosopher *philo)
{
	free(philo->menu);
	free(philo->timer);
	free(philo->is_finished);
	pthread_mutex_destroy(philo->turn);
	pthread_mutex_destroy(philo->m_is_finished);
	free(philo->m_is_finished);
	free(philo);
}

void	fail_forks(char *str, t_info *p, t_thread *t)
{
	free(p);
	free_threads(t);
	print_error(str);
}

void	free_print(char *str, t_info *p, t_thread *t, pthread_mutex_t **f)
{
	free_forks(f, p->nb_philos);
	free_threads(t);
	free(p);
	print_error(str);
}
