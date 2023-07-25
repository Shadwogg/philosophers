/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 13:15:58 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/25 22:32:04 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_error(char *str)
{
	size_t	len;

	if (str == NULL)
		return (1);
	len = 0;
	while (str[len] != 0)
		len++;
	write(2, str, len);
	return (1);
}

// default color = "\033[00m"
void	color_print(unsigned int id)
{
	if (id % 8 == 0)
		printf("\033[0;30m");
	else if (id % 8 == 1)
		printf("\033[0;31m");
	else if (id % 8 == 2)
		printf("\033[0;32m");
	else if (id % 8 == 3)
		printf("\033[0;33m");
	else if (id % 8 == 4)
		printf("\033[0;34m");
	else if (id % 8 == 5)
		printf("\033[0;35m");
	else if (id % 8 == 6)
		printf("\033[0;36m");
	else
		printf("\033[0;37m");
}

int	print_status(t_philosopher *philo, char *str, int has_died)
{
	static int	someone_has_died;
	time_t		sec;
	time_t		start_sec;
	suseconds_t	usec;
	suseconds_t	start_usec;

	if (pthread_mutex_lock(philo->timer->mutex_clock) != 0)
		return (-1);
	sec = philo->timer->clock->tv_sec;
	usec = philo->timer->clock->tv_usec;
	start_sec = philo->timer->start.tv_sec;
	start_usec = philo->timer->start.tv_usec;
	if (pthread_mutex_unlock(philo->timer->mutex_clock) != 0)
		return (-1);
	if (pthread_mutex_lock(philo->turn) != 0)
		return (-1);
	if (someone_has_died != 1)
		printf("%ld %u %s\n", (sec * 1000 + usec / 1000)
			- (start_sec * 1000 + start_usec / 1000), philo->id, str);
	if (pthread_mutex_unlock(philo->turn) != 0)
		return (-1);
	someone_has_died |= has_died;
	return (0);
}
