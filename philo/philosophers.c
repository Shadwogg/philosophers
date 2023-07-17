/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/18 00:28:44 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_philo_has_eaten(void *null, unsigned int times)
{
	(void) null;
	(void) times;
	return (1);
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
void	philosopher(t_info *p, t_thread *threads, pthread_mutex_t **forks)
{
	t_thread		*cur;
	t_philosopher			*table;
	unsigned int	ct;
	pthread_mutex_t	*turn;

	cur = threads;
	ct = 0;
	turn = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(turn, NULL) != 0)
		print_error("TO IMPLENT (philo)");
	while (cur != NULL)
	{
		table = set_philosopher(p, forks, ct++, turn);
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
	t_info			*philo;
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
