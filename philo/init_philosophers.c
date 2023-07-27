/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:35:15 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 17:17:39 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	copy_menu(t_info *dst, t_info *src)
{
	dst->nb_philos = src->nb_philos;
	dst->ttd = src->ttd;
	dst->tte = src->tte;
	dst->tts = src->tts;
	dst->times = src->times;
}

int	init_timer(t_philosopher *philo)
{
	philo->timer.mutex_clock = NULL;
	philo->timer.clock = NULL;
	philo->timer.mutex_timer = NULL;
	philo->timer.time_eaten = 0;
	philo->timer.start.tv_sec = 0;
	philo->timer.start.tv_usec = 0;
	philo->timer.last_eaten.tv_sec = 0;
	philo->timer.last_eaten.tv_usec = 0;
	philo->timer.mutex_timer = malloc(sizeof(pthread_mutex_t));
	if (philo->timer.mutex_timer == NULL)
		return (p_error("M_timer failed to be allocated.\n"), -1);
	if (pthread_mutex_init(philo->timer.mutex_timer, NULL) != 0)
		return (p_error("M_timer failed to be initialised.\n"),
			free(philo->timer.mutex_timer), -1);
	return (0);
}

int	init_philo_mutex(t_philosopher *philo, unsigned int nb_philos,
	pthread_mutex_t **forks)
{
	if (philo->id - 1 == 0)
		philo->left_fork = forks[nb_philos - 1];
	else
		philo->left_fork = forks[philo->id - 2];
	philo->right_fork = forks[philo->id - 1];
	philo->mutex_is_finished = malloc(sizeof(pthread_mutex_t));
	if (philo->mutex_is_finished == NULL)
		return (p_error("M_is_finished failed to be allocated.\n"), -1);
	if (pthread_mutex_init(philo->mutex_is_finished, NULL) != 0)
		return (p_error("M_is_finished failed to be initialised.\n"),
			free(philo->mutex_is_finished), -1);
	return (0);
}

t_philosopher	*init_philosopher(t_info *info, pthread_mutex_t **forks,
	unsigned int ct)
{
	t_philosopher	*philo;

	philo = malloc(sizeof(t_philosopher));
	if (philo == NULL)
		return (p_error("A philosopher failed to be allocated.\n"), NULL);
	philo->id = ct + 1;
	copy_menu(&philo->menu, info);
	philo->is_finished = malloc(sizeof(int));
	if (philo->is_finished == NULL)
		return (p_error("philo->is_finished failed to be allocated"),
			free(philo), NULL);
	*(philo->is_finished) = 0;
	philo->mutex_is_finished = NULL;
	if (init_timer(philo) != 0)
		return (free_philo(philo), NULL);
	if (init_philo_mutex(philo, info->nb_philos, forks) != 0)
		return (free_philo(philo), NULL);
	return (philo);
}
