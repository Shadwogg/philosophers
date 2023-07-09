/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:35:52 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/06 14:41:55 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * Convert a string representing a number to the number.
*/
int	ft_atoi(const char *nptr)
{
	unsigned int	nb;
	int				ct;
	long			res;

	nb = 0;
	ct = 0;
	while (nptr[ct] == ' ' || nptr[ct] == '\f' || nptr[ct] == '\n'
		|| nptr[ct] == '\r' || nptr[ct] == '\t' || nptr[ct] == '\v')
		ct++;
	if (nptr[ct] == '-')
		res = -1;
	else
		res = 1;
	if (nptr[ct] == '+' || nptr[ct] == '-')
		ct++;
	ct--;
	while (nptr[++ct] >= '0' && nptr[ct] <= '9')
		nb = 10 * nb + nptr[ct] - 48;
	res *= nb;
	return ((int)(res));
}
