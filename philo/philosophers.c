/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/10 15:44:42 by ggiboury         ###   ########.fr       */
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
	table->philo = ref->nb_philos;
	return (table);
}

void	*live(void *table)
{
	t_table	*t;

	t = table;
	if (table == NULL)
		return (NULL);
	pthread_mutex_lock(t->left_fork);
	pthread_mutex_lock(t->right_fork);
	sleep(2);
	printf("Coucou\n");
	pthread_mutex_unlock(t->left_fork);
	pthread_mutex_unlock(t->right_fork);
	free_table(t);
	return (NULL);
}

void	philosopher(t_philo *p, t_thread *t, pthread_mutex_t **forks)
{
	t_thread		*cur;
	t_table			*table;
	unsigned int	ct;

	cur = t;
	ct = 0;
	while (cur != NULL)
	{
		table = set_table(p, forks, ct++);
		if (table == NULL)
			free_print("Table failed to be initialized.", p, t, forks);
		if (pthread_create(&cur->thread, NULL, &live, table) == -1)
		{
			free_table(table);
			free_print("Thread failed to be initialized.", p, t, forks);
		}
		cur = cur->next;
	}
	cur = t;
	while (cur != NULL)
	{
		printf("FIn\n");
		pthread_join(cur->thread, NULL);
		cur = cur->next;
	}
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
	print_philo(philo);
	threads = init_threads(philo);
	forks = init_forks(philo->nb_philos, philo, threads);
	philosopher(philo, threads, forks);
	free_forks(forks, philo->nb_philos);
	free_threads(threads);
	free(philo);
}
