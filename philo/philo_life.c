/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 00:24:01 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:53:09 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	lock_forks(t_philosopher *philo, pthread_mutex_t *left,
	pthread_mutex_t *right)
{
	if (pthread_mutex_lock(left) != 0)
		return (p_error("Left fork failed to be locked.\n"), -1);
	if (print_status(philo, "has taken a fork", 0) != 0)
		return (-1);
	if (left == right)
	{
		if (ft_mlsleep(philo->menu.ttd + 1, philo->timer) == -1)
			return (-1);
		return (0);
	}
	if (pthread_mutex_lock(right) != 0)
		return (p_error("Right fork failed to be locked.\n"), -1);
	if (print_status(philo, "has taken a fork", 0) != 0)
		return (-1);
	return (0);
}

int	philo_eat(t_philosopher *philo, pthread_mutex_t *left,
	pthread_mutex_t *right)
{
	if (lock_forks(philo, left, right) != 0)
		return (-1);
	if (philo_is_finished(philo) == 1)
		return (0);
	if (pthread_mutex_lock(philo->timer.mutex_timer) != 0)
		return (p_error("m_timer failed to be locked.\n"), -1);
	if (pthread_mutex_lock(philo->timer.mutex_clock) != 0)
		return (p_error("m_clock failed to be locked.\n"), -1);
	philo->timer.last_eaten.tv_sec = philo->timer.clock->tv_sec;
	philo->timer.last_eaten.tv_usec = philo->timer.clock->tv_usec;
	if (pthread_mutex_unlock(philo->timer.mutex_clock) != 0)
		return (p_error("m_clock failed to be unlocked.\n"), -1);
	philo->timer.time_eaten++;
	if (pthread_mutex_unlock(philo->timer.mutex_timer) != 0)
		return (p_error("m_timer failed to be unlocked.\n"), -1);
	if (philo_is_finished(philo))
		return (0);
	if (print_status(philo, "is eating", 0) != 0)
		return (-1);
	return (ft_mlsleep(philo->menu.tte, philo->timer));
}

int	philo_is_finished(t_philosopher *philo)
{
	int	finished;

	finished = 0;
	if (pthread_mutex_lock(philo->mutex_is_finished) != 0)
		return (p_error("m_is_finished failed to be locked.\n"), -1);
	if (*(philo->is_finished))
		finished = 1;
	if (pthread_mutex_unlock(philo->mutex_is_finished) != 0)
		return (p_error("m_is_finished failed to be unlocked.\n"), -1);
	return (finished);
}

int	philo_sleep(t_philosopher *t)
{
	if (philo_is_finished(t))
		return (0);
	if (print_status(t, "is sleeping", 0) == -1)
		return (-1);
	if (ft_mlsleep(t->menu.tts, t->timer) == -1)
		return (-1);
	if (philo_is_finished(t))
		return (0);
	return (print_status(t, "is thinking", 0));
}

void	*live(void *philo)
{
	t_philosopher	*t;

	if (philo == NULL)
		return (NULL);
	t = (t_philosopher *) philo;
	while (!philo_is_finished(t))
	{
		if (t->timer.time_eaten == 0 && t->id % 2 == 1)
			ft_mlsleep((t->menu.tte / 2) + 1, t->timer);
		if (philo_is_finished(t))
			return (t);
		if (philo_eat(t, t->left_fork, t->right_fork) != 0)
			return (NULL);
		if (pthread_mutex_unlock(t->left_fork) != 0)
			return (p_error("Left fork failed be unlocked.\n"), NULL);
		if (pthread_mutex_unlock(t->right_fork) != 0)
			return (p_error("Right fork failed be unlocked.\n"), NULL);
		if (philo_sleep(t) == -1)
			return (NULL);
	}
	return (t);
}
