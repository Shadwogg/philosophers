/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 00:24:01 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 22:20:04 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	lock_forks(t_philosopher *table, pthread_mutex_t *left,
	pthread_mutex_t *right)
{
	if (pthread_mutex_lock(left) != 0)
		return (1);
	if (print_status(table->philo_id, table->timer, "has taken a fork", table->turn) != 1)
		return (1);
	if (pthread_mutex_lock(right) != 0)
		return (1);
	if (print_status(table->philo_id, table->timer, "has taken a fork", table->turn) != 1)
		return (1);
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

int	philo_is_finished(t_philosopher *table)
{
	int	finished;

	finished = 0;
	if (pthread_mutex_lock(table->m_is_finished) != 0)
		;
	if (*(table->is_finished))
		finished = 1;
	if (pthread_mutex_unlock(table->m_is_finished) != 0)
		;
	return (finished);
}

void	philo_sleep(t_philosopher *t)
{
	print_status(t->philo_id, t->timer, "is sleeping", t->turn);
	ft_mlsleep(t->menu->tts);
	print_status(t->philo_id, t->timer, "is thinking", t->turn);
}

void	*live(void *table)
{
	t_philosopher	*t;

	if (table == NULL)
		return (NULL);
	t = (t_philosopher *) table;
	while (!philo_is_finished(t))
	{
		if (t->philo_id % 2 == 1)
			usleep((t->menu->tte / 2) * 1000);
		if (lock_forks(t, t->left_fork, t->right_fork) != 0)
			return (NULL);
		print_status(t->philo_id, t->timer, "is eating", t->turn);
		ft_mlsleep(t->menu->tte);
		if (unlock_forks(t->left_fork, t->right_fork) != 0)
			return (NULL);
		philo_sleep(t);
		break ;
	}
	return (t);
}
