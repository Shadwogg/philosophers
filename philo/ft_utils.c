/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:39:59 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/12 18:06:38 by ggiboury         ###   ########.fr       */
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

void	add_list(t_thread **t, t_philo *ref)
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
	cur->next->next = NULL;
	cur->next->numero = ct;
}
