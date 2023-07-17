/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 13:15:58 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 00:05:11 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_error(char *str)
{
	size_t	len;

	if (str == NULL)
		exit(EXIT_FAILURE);
	len = 0;
	while (str[len] != 0)
		len++;
	write(2, str, len);
	exit(EXIT_FAILURE);
}

void	print_philo(t_info *p)
{
	printf("Philo\nttd = %u\n", p->ttd);
	printf("tte = %u\ntts = %u\ntimes = %u\n", p->tte, p->tts, p->times);
}

void	color_print(unsigned int philo_id)
{
	if (philo_id % 8 == 0)
		printf("\033[0;30m");
	else if (philo_id % 8 == 1)
		printf("\033[0;31m");
	else if (philo_id % 8 == 2)
		printf("\033[0;32m");
	else if (philo_id % 8 == 3)
		printf("\033[0;33m");
	else if (philo_id % 8 == 4)
		printf("\033[0;34m");
	else if (philo_id % 8 == 5)
		printf("\033[0;35m");
	else if (philo_id % 8 == 6)
		printf("\033[0;36m");
	else
		printf("\033[0;37m");
}

int	print_status(unsigned int philo_id, t_timer *timer,
	char *str, pthread_mutex_t *turn)
{
	useconds_t	now;

	if (pthread_mutex_lock(turn) != 0)
		return (0);
	if (gettimeofday(&(timer->tv), NULL) != 0)
		return (0);
	now = (timer->tv.tv_sec * 1000 + timer->tv.tv_usec / 1000)
		- (timer->start.tv_sec * 1000 + timer->start.tv_usec / 1000);
	color_print(philo_id);
	printf("%u %u %s\n", now, philo_id, str);
	printf("\033[00m");
	if (pthread_mutex_unlock(turn) != 0)
		return (0);
	return (1);
}
