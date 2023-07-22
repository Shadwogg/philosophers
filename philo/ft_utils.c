/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:39:59 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/22 14:23:49 by ggiboury         ###   ########.fr       */
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
	if (tmp > UINT_MAX)
		return (1);
	return (0);
}

void	add_list(t_thread **t, t_info *ref)
{
	t_thread		*cur;
	unsigned int	ct;

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
		free(ref);
		free_threads(*t);
		print_error("t_thread failed to be malloc.");
	}
	cur->next->numero = ct;
	cur->next->next = NULL;
	cur->next->philo = NULL;
}

int	ft_mlsleep(long time_mls)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL) != 0)
		return (1);
	time_mls = t.tv_sec * 1000 + t.tv_usec / 1000 + time_mls;
	while (t.tv_sec * 1000 + t.tv_usec / 1000 < time_mls)
	{
		if (gettimeofday(&t, NULL) != 0)
			return (1);
		usleep(10);
	}
	return (0);
}
