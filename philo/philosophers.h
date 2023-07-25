/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:13 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 00:21:17 by ggiboury         ###   ########.fr       */
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
}	t_info;

// Chained list of thread.
typedef struct s_thread
{
	unsigned int			numero;
	struct s_thread			*next;
	struct s_philosopher	*philo;
	pthread_t				thread;
}	t_thread;

// Struct containing counters.
typedef struct s_timer
{
	unsigned int	time_eaten;
	struct timeval	start;
	struct timeval	last_eaten;
	struct timeval	*clock;
	pthread_mutex_t	*mutex_time_eaten;
	pthread_mutex_t	*mutex_clock;
}	t_timer;

// View of a philosopher.
typedef struct s_philosopher
{
	int				*is_finished;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*turn;
	pthread_mutex_t	*mutex_is_finished;
	t_info			*menu;
	t_timer			*timer;
}	t_philosopher;

typedef struct s_controller
{
	unsigned int			number_philo;
	struct s_philosopher	**philos;
}	t_controller;

int				print_error(char *str);
int				print_status(t_philosopher *philo, char *str, int death);

/*************************************PARSE************************************/

void			add_list(t_thread **t, t_info *ref);
int				input_is_invalid(int argc, char **argv);
t_info			*ft_parse(int argc, char **argv);
pthread_mutex_t	**init_forks(unsigned int nb);
t_thread		*init_threads(t_info *philo, pthread_mutex_t **forks);
t_philosopher	*init_philo_mutex(t_philosopher *philo, unsigned int nb_philo,
					pthread_mutex_t **forks);
t_philosopher	*init_philosopher(t_info *ref, pthread_mutex_t **forks,
					unsigned int ct);
int				init_timer(t_philosopher *philo);
t_info			*copy_menu(t_info *ref);

/*************************************UTILS************************************/

int				is_not_number(char *nb);
int				is_not_int(char *nb);
int				ft_atoi(const char *nptr);
char			*ft_itoa(unsigned int n);

int				ft_mlsleep(long time_mls, t_timer *t);

void			*free_controller(t_controller *ctler);
void			free_threads(t_thread *t);
void			free_forks(pthread_mutex_t **forks, unsigned int nb);
void			free_philo(t_philosopher *philo);
void			free_print(char *str, t_info *p, t_thread *t,
					pthread_mutex_t **f);
/***********************************PHILO_LIFE*********************************/

void			*live(void *philo);
int				lock_forks(t_philosopher *philo, pthread_mutex_t *left,
					pthread_mutex_t *right);
int				unlock_forks(pthread_mutex_t *left, pthread_mutex_t *right);
int				philo_is_finished(t_philosopher *philo);
int				philo_sleep(t_philosopher *p);

/***********************************PHILO_DEATH********************************/

int				terminate_all(t_philosopher **philos, unsigned int nb);
int				simulation_is_finished(t_controller *ctler);
int				all_philo_has_eaten(t_philosopher **philos, unsigned int nb);
void			*harvest(void *souls);

/*********************************TIME_OF_PHILO********************************/

int				update_clock(t_timer *clock);
void			*set_timers(void *souls);
int				init_clock_thread(pthread_t *clock_thread,
					t_info *info, t_thread *threads);

/************************************THREADS***********************************/

int				verify_threads(t_thread *threads);
t_thread		*init_threads(t_info *info, pthread_mutex_t **forks);
int				launch_threads(t_info *info, t_thread *threads);
int				synchronize_philosophers(t_thread *threads);

#endif