/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:03:35 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/10 15:37:16 by ggiboury         ###   ########.fr       */
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

	printf("To implement\n");
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

void	fail_forks(char *str, t_philo *p, t_thread *t)
{
	free(p);
	free_threads(t);
	print_error(str);
}

void	free_print(char *str, t_philo *p, t_thread *t, pthread_mutex_t **f)
{
	free_forks(f, p->nb_philos);
	free_threads(t);
	free(p);
	print_error(str);
}
