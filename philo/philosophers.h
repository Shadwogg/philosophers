/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:13 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/17 17:04:46 by ggiboury         ###   ########.fr       */
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

// The struct containing parsed informations.
typedef struct s_philo
{
	unsigned int	nb_philos;
	unsigned int	ttd;
	unsigned int	tte;
	unsigned int	tts;
	unsigned int	times;
	struct timeval	start_time;
}	t_philo;

// Chained list of thread
typedef struct s_thread
{
	unsigned int	numero;
	struct s_thread	*next;
	pthread_t		thread;
}	t_thread;

// struct timeval | ->tv_usec
typedef struct s_timer
{
	unsigned int	*time_eaten;
	pthread_mutex_t	*mutex;
	struct timeval	start;
	struct timeval	tv;
}	t_timer;

// View of a philosopher
typedef struct s_table
{
	unsigned int	philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*turn;
	t_philo			*menu;
	t_timer			*timer;
}	t_table;

int				print_error(char *str);
void			print_philo(t_philo *p);
int				print_status(unsigned int philo_id, t_timer *start,
					char *str, pthread_mutex_t *turn);

/*************************************PARSE************************************/

void			add_list(t_thread **t, t_philo *ref);
int				input_is_invalid(int argc, char **argv);
t_philo			*ft_parse(int argc, char **argv);
pthread_mutex_t	**init_forks(unsigned int nb, t_philo *p, t_thread *t);
t_thread		*init_threads(t_philo *philo);

/*************************************UTILS************************************/

int				is_not_number(char *nb);
int				is_not_int(char *nb);
int				ft_atoi(const char *nptr);
char			*ft_itoa(unsigned int n);

void			free_threads(t_thread *t);
void			free_forks(pthread_mutex_t **f, unsigned int nb);
void			free_table(t_table *table);
void			fail_forks(char *str, t_philo *p, t_thread *t);
void			free_print(char *str, t_philo *p, t_thread *t,
					pthread_mutex_t **f);

#endif