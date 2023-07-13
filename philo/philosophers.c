/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/13 16:19:35 by ggiboury         ###   ########.fr       */
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

t_table	*set_table(t_philo *ref, pthread_mutex_t **forks, unsigned int ct)
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
	return (table);
}

int	lock_forks(t_table *table, pthread_mutex_t *left, pthread_mutex_t *right)
{
	(void) table;
	if (pthread_mutex_lock(left) != 0)
		return (1);
	if (print_status(table->philo_id, table->timer, "has taken a fork", table->turn) != 0)
		return (1);
	if (pthread_mutex_lock(right) != 0)
		return (1);
	// if (print_status(table->philo_id, table->timer, "has taken a fork", table->turn) != 0)
	// 	return (1);
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

int	philo_is_finished(t_table *table)
{
	if (table->menu->times == (unsigned int) -1)
		return (0);
	//table->menu->times comparer avec le tableau des nombre de fois mange
	return (0);
}

void	*live(void *table)
{
	t_table	*t;

	t = table;
	if (table == NULL)
		return (NULL);
	while (!philo_is_finished(table))
	{
		printf("Nouveau tour pour %u\n", t->philo_id);
		if (lock_forks(t, t->left_fork, t->right_fork) != 0)
			return (t);
		if (unlock_forks(t->left_fork, t->right_fork) != 0)
			return (t);
		break ;
	}
	free_table(t);
	return (NULL);
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
		// printf("Fin\n");
		if (pthread_join(cur->thread, returned_table) != 0)
			printf("TO IMPLEMENT");
		if (returned_table != NULL)
		{
			free_table(returned_table);
			err = 0;
		}
		cur = cur->next;
	}
	return ((int) err);
}

//SI un philo meurt, peut on voir d'autre messages apres ?
//Initialize each thread used for each philosopher.
void	philosopher(t_philo *p, t_thread *threads, pthread_mutex_t **forks)
{
	t_thread		*cur;
	t_table			*table;
	unsigned int	ct;
	pthread_mutex_t	*turn;

	cur = threads;
	ct = 0;
	turn = 
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
	printf("True ending\n");
}

int	main(int argc, char **argv)
{
	t_philo			*philo;
	t_thread		*threads;
	pthread_mutex_t	**forks;

	if (argc < 5 || argc > 6)
		print_error("Usage : ./philo <nb> <ttd> <tte> <tts> [nb_e].");
	philo = ft_parse(argc - 1, argv + 1);
	if (philo->nb_philos == 0)
	{
		free(philo);
		return (1);
	}
	// print_philo(philo);
	threads = init_threads(philo);
	forks = init_forks(philo->nb_philos, philo, threads);
	philosopher(philo, threads, forks);
	free_forks(forks, philo->nb_philos);
	free_threads(threads);
	free(philo);
}
