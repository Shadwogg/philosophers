/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:35:15 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 00:21:04 by ggiboury         ###   ########.fr       */
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

int	init_timer(t_philosopher *philo)
{
	philo->timer = malloc(sizeof(t_timer));
	if (philo->timer == NULL)
	{
		free(philo->menu);
		free(philo);
		return (1);
	}
	philo->timer->time_eaten = 0;
	philo->timer->mutex_time_eaten = malloc(sizeof(pthread_mutex_t));
	if (philo->timer->mutex_time_eaten == NULL)
		return (1);
	if (pthread_mutex_init(philo->timer->mutex_time_eaten, NULL) != 0)
	{
		free(philo->menu);
		free(philo->timer->mutex_time_eaten);
		free(philo);
		return (1);
	}
	return (0);
}

t_philosopher	*init_philo_mutex(t_philosopher *philo, unsigned int nb_philos,
	pthread_mutex_t **forks)
{
	if (philo->id - 1 == 0)
		philo->left_fork = forks[nb_philos - 1];
	else
		philo->left_fork = forks[philo->id - 2];
	philo->right_fork = forks[philo->id - 1];
	philo->mutex_is_finished = malloc(sizeof(pthread_mutex_t));
	if (philo->mutex_is_finished == NULL)
		return (NULL);
	if (pthread_mutex_init(philo->mutex_is_finished, NULL) != 0)
	{
		free(philo->mutex_is_finished);
		return (NULL);
	}
	return (philo);
}

t_philosopher	*init_philosopher(t_info *info, pthread_mutex_t **forks,
	unsigned int ct)
{
	t_philosopher	*philo;

	philo = malloc(sizeof(t_philosopher));
	if (philo == NULL)
		return (print_error("A philosopher failed to be allocated\n"), NULL);
	philo->id = ct + 1;
	philo->menu = copy_menu(info);
	if (philo->menu == NULL)
		return (free(philo->menu), free(philo), NULL);
	if (init_philo_mutex(philo, info->nb_philos, forks) == NULL)
		return (free(philo->menu), free(philo), NULL);
	if (init_timer(philo) != 0)
		return (free_philo(philo), NULL);
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
