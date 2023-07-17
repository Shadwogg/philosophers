/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/17 18:42:00 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*copy_menu(t_philo *ref)
{
	t_philo	*menu;

	menu = malloc(sizeof(t_philo));
	if (menu == NULL)
		return (NULL);
	menu->nb_philos = ref->nb_philos;
	menu->ttd = ref->ttd;
	menu->tte = ref->tte;
	menu->tts = ref->tts;
	menu->times = ref->times;
	return (menu);
}

int	init_timer(t_table *table, struct timeval start_time)
{
	table->timer = malloc(sizeof(t_timer));
	if (table->timer == NULL)
	{
		free(table->menu);
		free(table);
		return (1);
	}
	table->timer->start = start_time;
	table->timer->time_eaten = 0;
	table->timer->tv.tv_usec = 0;
	table->timer->tv.tv_sec = 0;
	return (0);
}
t_table	*set_table(t_philo *ref, pthread_mutex_t **forks, unsigned int ct, pthread_mutex_t *turn)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (table == NULL)
		return (NULL);
	table->menu = copy_menu(ref);
	if (table->menu == NULL)
	{
		free(table);
		return (NULL);
	}
	if (ct == 0)
		table->left_fork = forks[ref->nb_philos - 1];
	else
		table->left_fork = forks[ct - 1];
	table->right_fork = forks[ct];
	table->philo_id = ct + 1;
	table->turn = turn;
	if (init_timer(table, ref->start_time) != 0)
		return (NULL);
	return (table);
}

int	lock_forks(t_table *table, pthread_mutex_t *left, pthread_mutex_t *right)
{
	if (pthread_mutex_lock(left) != 0)
		return (1);
	if (print_status(table->philo_id, table->timer, "has taken a fork", table->turn) != 1)
		return (1);
	if (pthread_mutex_lock(right) != 0)
		return (1);
	usleep(1000);
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

int	all_philo_has_eaten(void *null, unsigned int times)
{
	(void) null;
	(void) times;
	return (1);
}

int	philo_is_finished(t_table *table)
{
	int	has_eaten;

	if (table->menu->times == 0)
		return (0);
	has_eaten = 0;
	if (pthread_mutex_lock(table->timer->mutex) != 0)
		;
	if (all_philo_has_eaten(NULL, table->menu->times))
		has_eaten = 1;
	if (pthread_mutex_unlock(table->timer->mutex) != 0)
		;
	return (has_eaten);
}

void	philo_sleep(t_table *t)
{
	print_status(t->philo_id, t->timer, "is sleeping", t->turn);
	usleep(t->menu->tts * 1000);
	print_status(t->philo_id, t->timer, "is thinking", t->turn);
}

void	*live(void *table)
{
	t_table	*t;

	if (table == NULL)
		return (NULL);
	t = (t_table *) table;
	while (!philo_is_finished(t))
	{
		// printf("Nouveau tour pour %u\n", t->philo_id);
		if (t->philo_id % 2 == 1)
			usleep((t->menu->tte / 2) * 1000);
		if (lock_forks(t, t->left_fork, t->right_fork) != 0)
			return (NULL);
		print_status(t->philo_id, t->timer, "is eating", t->turn);
		usleep(t->menu->tte * 1000);
		if (unlock_forks(t->left_fork, t->right_fork) != 0)
			return (NULL);
		philo_sleep(t);
		break ;
	}
	// free_table(t);
	return (t);
}

// Verify that each thread ended correctly, if not return 0.
int	verify_threads(t_thread *threads)
{
	t_thread		*cur;
	void			*returned_table;
	char			err;

	cur = threads;
	err = 1;
	returned_table = NULL;
	while (cur != NULL)
	{
		if (pthread_join(cur->thread, returned_table) != 0)
			printf("TO IMPLEMENT");
		if (returned_table == NULL)
			err = 0;
		else
			free_table(returned_table);
		cur = cur->next;
	}
	return ((int) err);
}

//Initialize each thread used for each philosopher.
void	philosopher(t_philo *p, t_thread *threads, pthread_mutex_t **forks)
{
	t_thread		*cur;
	t_table			*table;
	unsigned int	ct;
	pthread_mutex_t	*turn;

	cur = threads;
	ct = 0;
	turn = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(turn, NULL) != 0)
		print_error("TO IMPLENT (philo)");
	while (cur != NULL)
	{
		table = set_table(p, forks, ct++, turn);
		if (table == NULL)
			free_print("Table failed to be initialized.", p, threads, forks);
		if (pthread_create(&cur->thread, NULL, &live, table) == -1)
		{
			free_table(table);
			free_print("Thread failed to be initialized.", p, threads, forks);
		}
		cur = cur->next;
	}
	if (verify_threads(threads) == 0)
		free_print("One thread failed.", p, threads, forks);
	pthread_mutex_destroy(turn);
	printf("sortie = %p\n", table);
	// free_table(table);
	printf("True ending\n");
}

int	main(int argc, char **argv)
{
	t_philo			*philo;
	t_thread		*threads;
	pthread_mutex_t	**forks;
	struct timeval	tv;

	if (argc < 5 || argc > 6)
		print_error("Usage : ./philo <nb> <ttd> <tte> <tts> [nb_e].");
	philo = ft_parse(argc - 1, argv + 1);
	if (philo->nb_philos == 0)
	{
		free(philo);
		return (1);
	}
	threads = init_threads(philo);
	forks = init_forks(philo->nb_philos, philo, threads);
	if (gettimeofday(&tv, NULL) != 0)
		free_print("Error while getting the time", philo, threads, forks);
	philo->start_time = tv;
	philosopher(philo, threads, forks);
	free_forks(forks, philo->nb_philos);
	free_threads(threads);
	free(philo);
}
