/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/25 16:43:48 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Verify that each thread ended correctly, if not return 0.
int	verify_threads(t_thread *threads)
{
	t_thread		*cur;
	void			*returned_philo;
	char			err;

	cur = threads;
	err = 1;
	returned_philo = NULL;
	while (cur != NULL)
	{
		if (pthread_join(cur->thread, &returned_philo) != 0)
			printf("TO IMPLEMENT");
		if (returned_philo == NULL)
			err = 0;
		else
			free_philo(returned_philo);
		cur = cur->next;
	}
	return ((int) err);
}

int	launch_threads(t_info *info, t_thread *threads, pthread_mutex_t **forks)
{
	unsigned int	ct;
	t_thread		*cur;

	ct = 0;
	cur = threads;
	while (cur != NULL)
	{
		if (pthread_create(&cur->thread, NULL, &live, cur->philo) == -1)
		{
			free_print("Thread failed to be initialized.", info, threads, forks);
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

int	init_death(pthread_t *ending_thread, t_info *info, t_thread *threads)
{
	t_controller	*controller;
	t_thread		*cur;
	unsigned int	ct;

	controller = malloc(sizeof(t_controller));
	if (controller == NULL)
		print_error("Controller null");
	controller->number_philo = info->nb_philos;
	controller->philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		controller->philos[ct] = cur->philo;
		ct++;
		cur = cur->next;
	}
	if (controller->philos == NULL)
		return (1);
	if (pthread_create(ending_thread, NULL, &harvest, controller) != 0)
		return (1);
	if (pthread_detach(*ending_thread) != 0)
		return (1);
	return (0);
}

//Initialize each thread used for each philosopher.
void	philosopher(t_info *p, t_thread *threads, pthread_mutex_t **forks)
{
	pthread_t		end_thread;
	pthread_t		watch_thread;

	if (init_death(&end_thread, p, threads) != 0)
		print_error("TO IMPPLLEEMMEENNT (philosopher)");
	if (init_watch(&watch_thread, p, threads) != 0)
		print_error("TO implement");
	if (launch_threads(p, threads, forks) != 0)
		return ;
	if (verify_threads(threads) == 0)
	{
		free_print("One thread failed.", p, threads, forks);
	}
	// if (pthread_mutex_destroy(&turn) != 0)
	// 	free_print("Failed to destroy the turn mutex", p, threads, forks);
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
	printf("SF normal, didnt implement yet le malloc de l'horloge commune\n");
	forks = init_forks(philo->nb_philos);
	if (forks == NULL)
	{
		free(philo);
		print_error("Forks failed to be initialized.");
	}
	// All good
	if (gettimeofday(&tv, NULL) != 0)
		free_print("Error while getting the time", philo, NULL, forks);
	philo->start_time = tv;
	threads = init_threads(philo, forks);
	philosopher(philo, threads, forks);
	free_forks(forks, philo->nb_philos);
	free_threads(threads);
	free(philo);
}
