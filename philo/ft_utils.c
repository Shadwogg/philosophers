/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:39:59 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/06 18:08:29 by ggiboury         ###   ########.fr       */
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
