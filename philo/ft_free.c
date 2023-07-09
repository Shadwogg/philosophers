/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:03:35 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/09 17:23:43 by ggiboury         ###   ########.fr       */
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

void	free_forks(char	**f, unsigned int nb)
{
	unsigned int	ct;

	ct = 0;
	while (ct < nb)
		free(f[ct++]);
	free(f);
}

void	free_table(t_table *table)
{
	free(table->menu);
	free(table);
}
