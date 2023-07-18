/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:13 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 23:02:45 by ggiboury         ###   ########.fr       */
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
typedef struct s_info
{
	unsigned int	nb_philos;
	unsigned int	ttd;
	unsigned int	tte;
	unsigned int	tts;
	unsigned int	times;
	struct timeval	start_time;
}	t_info;

// Chained list of thread.
typedef struct s_thread
{
	unsigned int	numero;
	struct s_thread	*next;
	pthread_t		thread;
}	t_thread;

// Struct containing counters.
typedef struct s_timer
{
	unsigned int	*time_eaten;
	pthread_mutex_t	*mutex;
	struct timeval	start;
	struct timeval	tv;
}	t_timer;

// View of a philosopher.
typedef struct s_philosopher
{
	int				*is_finished;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*turn;
	t_info			*menu;
	t_timer			*timer;
	pthread_mutex_t	*m_is_finished;
}	t_philosopher;

int				print_error(char *str);
void			print_info(t_info *p);
int				print_status(unsigned int id, t_timer *start,
					char *str, pthread_mutex_t *turn);

/*************************************PARSE************************************/

void			add_list(t_thread **t, t_info *ref);
int				input_is_invalid(int argc, char **argv);
t_info			*ft_parse(int argc, char **argv);
pthread_mutex_t	**init_forks(unsigned int nb, t_info *p, t_thread *t);
t_thread		*init_threads(t_info *philo);
t_philosopher	*init_philo_mutex(t_philosopher *table, unsigned int nb_philo,
					pthread_mutex_t **forks, pthread_mutex_t *turn);
t_philosopher	*set_philosopher(t_info *ref, pthread_mutex_t **forks,
					unsigned int ct, pthread_mutex_t *turn);
int				init_timer(t_philosopher *table, struct timeval start_time);
t_info			*copy_menu(t_info *ref);

/*************************************UTILS************************************/

int				is_not_number(char *nb);
int				is_not_int(char *nb);
int				ft_atoi(const char *nptr);
char			*ft_itoa(unsigned int n);

int				ft_mlsleep(long time_mls);

void			free_threads(t_thread *t);
void			free_forks(pthread_mutex_t **f, unsigned int nb);
void			free_table(t_philosopher *table);
void			fail_forks(char *str, t_info *p, t_thread *t);
void			free_print(char *str, t_info *p, t_thread *t,
					pthread_mutex_t **f);
/***********************************PHILO_LIFE*********************************/

void			*live(void *table);
int				lock_forks(t_philosopher *table, pthread_mutex_t *left,
					pthread_mutex_t *right);
int				unlock_forks(pthread_mutex_t *left, pthread_mutex_t *right);
int				philo_is_finished(t_philosopher *table);
void			philo_sleep(t_philosopher *t);

#endif