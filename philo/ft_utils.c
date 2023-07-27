/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:39:59 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:42:00 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_not_number(char *nb)
{
	int	ct;

	ct = -1;
	if (nb == NULL || *nb == 0)
		return (1);
	while (nb[++ct])
		if (nb[ct] < 48 || nb[ct] > 57)
			return (1);
	return (0);
}

int	is_not_int(char *nb)
{
	unsigned long	tmp;
	unsigned int	ct;

	ct = 0;
	tmp = 0;
	while (nb[ct])
	{
		tmp = tmp * 10 + nb[ct] - 48;
		if (tmp > UINT_MAX)
			break ;
		ct++;
	}
	if (tmp == 0)
		return (1);
	if (tmp > UINT_MAX)
		return (1);
	return (0);
}

void	add_list(t_thread **t)
{
	t_thread		*cur;
	unsigned int	ct;

	if (*t == NULL)
		return ;
	cur = *t;
	ct = 1;
	while (cur->next != NULL)
	{
		ct++;
		cur = cur->next;
	}
	cur->next = malloc(sizeof(t_thread));
	if (cur->next == NULL)
	{
		free_threads(*t);
		p_error("t_thread failed to be malloc.");
		return ;
	}
	cur->next->numero = ct;
	cur->next->next = NULL;
	cur->next->philo = NULL;
}

int	ft_mlsleep(long time_mls, t_timer timer)
{
	struct timeval	actual;

	if (pthread_mutex_lock(timer.mutex_clock) != 0)
		return (p_error("Clock failed to be locked.\n"), -1);
	actual.tv_sec = timer.clock->tv_sec;
	actual.tv_usec = timer.clock->tv_usec;
	if (pthread_mutex_unlock(timer.mutex_clock) != 0)
		return (p_error("Clock failed to be unlocked.\n"), -1);
	time_mls = (actual.tv_sec * 1000 + actual.tv_usec / 1000) + time_mls;
	while (actual.tv_sec * 1000 + actual.tv_usec / 1000 < time_mls)
	{
		usleep(100);
		if (pthread_mutex_lock(timer.mutex_clock) != 0)
			return (p_error("Clock failed to be locked.\n"), -1);
		actual.tv_sec = timer.clock->tv_sec;
		actual.tv_usec = timer.clock->tv_usec;
		if (pthread_mutex_unlock(timer.mutex_clock) != 0)
			return (p_error("Clock failed to be unlocked.\n"), -1);
	}
	return (0);
}
