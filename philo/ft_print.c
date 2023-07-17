/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 13:15:58 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/17 18:42:50 by ggiboury         ###   ########.fr       */
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

void	print_philo(t_philo *p)
{
	printf("Philo\nttd = %u\n", p->ttd);
	printf("tte = %u\ntts = %u\ntimes = %u\n", p->tte, p->tts, p->times);
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
	// printf("	%ld | %ld\n", (timer->tv.tv_sec * 1000 + timer->tv.tv_usec / 1000), (timer->start.tv_sec * 1000 + timer->start.tv_usec / 1000));
	if (philo_id == 2)
		printf("	%u %u %s\n", now, philo_id, str);
	else
		printf("%u %u %s\n", now, philo_id, str);
	if (pthread_mutex_unlock(turn) != 0)
		return (0);
	return (1);
}
