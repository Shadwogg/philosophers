/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 00:24:01 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/23 15:02:12 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	lock_forks(t_philosopher *philo, pthread_mutex_t *left,
	pthread_mutex_t *right)
{
	if (pthread_mutex_lock(left) != 0)
		return (1);
	if (print_status(philo->id, philo->timer, "has taken a fork", philo->turn) != 1)
		return (1);
	if (pthread_mutex_lock(right) != 0)
		return (1);
	if (print_status(philo->id, philo->timer, "has taken a fork", philo->turn) != 1)
		return (1);
	if (pthread_mutex_lock(philo->timer->mutex) != 0)
		return (1);
	if (gettimeofday(&(philo->timer->last_eaten), NULL) != 0)
		return (1);
	philo->timer->time_eaten++;
	if (print_status(philo->id, philo->timer, "is eating", philo->turn) != 1)
		return (1);
	if (pthread_mutex_unlock(philo->timer->mutex) != 0)
		return (1);
	ft_mlsleep(philo->menu->tte);
	return (0);
}

int	unlock_forks(pthread_mutex_t *left, pthread_mutex_t *right)
{
	if (pthread_mutex_unlock(left) != 0)
		return (1);
	if (pthread_mutex_unlock(right) != 0)
		return (1);
	return (0);
}

int	philo_is_finished(t_philosopher *philo)
{
	int	finished;

	finished = 0;
	if (pthread_mutex_lock(philo->m_is_finished) != 0)
		printf("ERROR IS FINISHED\n");
	if (*(philo->is_finished))
		finished = 1;
	if (pthread_mutex_unlock(philo->m_is_finished) != 0)
		printf("ERROR IS FINISHED end\n");
	return (finished);
}

void	philo_sleep(t_philosopher *t)
{
	if (philo_is_finished(t))
		return ;
	print_status(t->id, t->timer, "is sleeping", t->turn);
	ft_mlsleep(t->menu->tts);
	print_status(t->id, t->timer, "is thinking", t->turn);
}

void	*live(void *philo)
{
	t_philosopher	*t;

	if (philo == NULL)
		return (NULL);
	t = (t_philosopher *) philo;
	while (!philo_is_finished(t))
	{
		if (t->timer->time_eaten == 0 && t->id % 2 == 1)
			ft_mlsleep(10);
		if (philo_is_finished(t))
			return (t);
		if (lock_forks(t, t->left_fork, t->right_fork) != 0)
			return (NULL);
		if (unlock_forks(t->left_fork, t->right_fork) != 0)
			return (NULL);
		philo_sleep(t);
	}
	return (t);
}
