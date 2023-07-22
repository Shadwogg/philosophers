/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:35:15 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/22 15:16:21 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_info	*copy_menu(t_info *info)
{
	t_info	*menu;

	menu = malloc(sizeof(t_info));
	if (menu == NULL)
		return (NULL);
	menu->nb_philos = info->nb_philos;
	menu->ttd = info->ttd;
	menu->tte = info->tte;
	menu->tts = info->tts;
	menu->times = info->times;
	return (menu);
}

int	init_timer(t_philosopher *philo, struct timeval start_time)
{
	philo->timer = malloc(sizeof(t_timer));
	if (philo->timer == NULL)
	{
		free(philo->menu);
		free(philo);
		return (1);
	}
	philo->timer->start = start_time;
	philo->timer->time_eaten = 0;
	philo->timer->tv.tv_usec = 0;
	philo->timer->tv.tv_sec = 0;
	return (0);
}

t_philosopher	*init_philo_mutex(t_philosopher *philo, unsigned int nb_philos,
	pthread_mutex_t **forks, pthread_mutex_t *turn)
{
	if (philo->id - 1 == 0)
		philo->left_fork = forks[nb_philos - 1];
	else
		philo->left_fork = forks[philo->id - 2];
	philo->right_fork = forks[philo->id - 1];
	philo->turn = turn;
	philo->m_is_finished = malloc(sizeof(pthread_mutex_t));
	if (philo->m_is_finished == NULL)
		return (NULL);
	if (pthread_mutex_init(philo->m_is_finished, NULL) != 0)
		return (NULL);
	return (philo);
}

t_philosopher	*set_philosopher(t_info *info, pthread_mutex_t **forks,
	unsigned int ct, pthread_mutex_t *turn)
{
	t_philosopher	*philo;

	philo = malloc(sizeof(t_philosopher));
	if (philo == NULL)
		return (NULL);
	philo->id = ct + 1;
	philo->menu = copy_menu(info);
	if (philo->menu == NULL)
	{
		free(philo);
		return (NULL);
	}
	if (init_philo_mutex(philo, info->nb_philos, forks, turn) == NULL)
	{
		free_philo(philo);
		return (NULL);
	}
	if (init_timer(philo, info->start_time) != 0)
		return (NULL);
	philo->is_finished = malloc(sizeof(int));
	if (philo->is_finished != NULL)
		*(philo->is_finished) = 0;
	else
	{
		free_philo(philo);
		return (NULL);
	}
	return (philo);
}
