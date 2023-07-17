/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:35:15 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 00:21:55 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_info	*copy_menu(t_info *ref)
{
	t_info	*menu;

	menu = malloc(sizeof(t_info));
	if (menu == NULL)
		return (NULL);
	menu->nb_philos = ref->nb_philos;
	menu->ttd = ref->ttd;
	menu->tte = ref->tte;
	menu->tts = ref->tts;
	menu->times = ref->times;
	return (menu);
}

int	init_timer(t_philosopher *table, struct timeval start_time)
{
	table->timer = malloc(sizeof(t_timer));
	if (table->timer == NULL)
	{
		free(table->menu);
		free(table);
		return (1);
	}
	table->timer->start = start_time;
	table->timer->time_eaten = 0;
	table->timer->tv.tv_usec = 0;
	table->timer->tv.tv_sec = 0;
	return (0);
}

t_philosopher	*init_philo_mutex(t_philosopher *table, unsigned int nb_philos,
	pthread_mutex_t **forks, pthread_mutex_t *turn)
{
	if (table->philo_id - 1 == 0)
		table->left_fork = forks[nb_philos - 1];
	else
		table->left_fork = forks[table->philo_id - 2];
	table->right_fork = forks[table->philo_id - 1];
	table->turn = turn;
	table->m_is_finished = malloc(sizeof(pthread_mutex_t));
	if (table->m_is_finished == NULL)
		return (NULL);
	return (table);
}

t_philosopher	*set_philosopher(t_info *ref, pthread_mutex_t **forks, unsigned int ct, pthread_mutex_t *turn)
{
	t_philosopher	*table;

	table = malloc(sizeof(t_philosopher));
	if (table == NULL)
		return (NULL);
	table->philo_id = ct + 1;
	table->menu = copy_menu(ref);
	if (table->menu == NULL)
	{
		free(table);
		return (NULL);
	}
	if (init_philo_mutex(table, ref->nb_philos, forks, turn) == NULL)
	{
		free_table(table);
		return (NULL);
	}
	if (init_timer(table, ref->start_time) != 0)
		return (NULL);
	table->is_finished = 0;
	return (table);
}
