/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 13:15:58 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/13 16:08:51 by ggiboury         ###   ########.fr       */
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
	printf("Philo\n");
	printf("ttd = %u\ntte = %u\ntts = %u\n", p->ttd, p->tte, p->tts);
}

int	print_status(unsigned int philo_id, t_timer *timer,
	char *str, pthread_mutex_t *turn)
{
	(void) timer;
	if (pthread_mutex_lock(turn) != 0)
		return (1);
	printf("%u %u %s\n", 0, philo_id, str);
	return (pthread_mutex_unlock(turn) != 0);
}
