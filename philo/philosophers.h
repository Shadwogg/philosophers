/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:13 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/09 17:23:54 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_philo
{
	unsigned int	nb_philos;
	unsigned int	ttd;
	unsigned int	tte;
	unsigned int	tts;
	unsigned int	times;
}	t_philo;

typedef struct s_thread
{
	unsigned int	numero;
	struct s_thread	*next;
	pthread_t		thread;
}	t_thread;

typedef struct s_table
{
	unsigned int	philo;
	char			*left_fork;
	char			*right_fork;
	t_philo			*menu;
}	t_table;

int		print_error(char *str);

/*************************************UTILS************************************/

int		is_not_number(char *nb);
int		is_not_int(char *nb);
int		ft_atoi(const char *nptr);

void	free_threads(t_thread *t);
void	free_forks(char	**f, unsigned int nb);
void	free_table(t_table *table);

#endif