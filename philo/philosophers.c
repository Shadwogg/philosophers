/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/26 15:34:32 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	set_time(t_thread *threads)
{
	struct timeval	*start;

	if (gettimeofday(threads->philo->timer->clock, NULL) != 0)
		return (-1);
	start = threads->philo->timer->clock;
	while (threads != NULL)
	{
		threads->philo->timer->start.tv_sec = start->tv_sec;
		threads->philo->timer->start.tv_usec = start->tv_usec;
		threads->philo->timer->last_eaten.tv_sec = start->tv_sec;
		threads->philo->timer->last_eaten.tv_usec = start->tv_usec;
		threads = threads->next;
	}
	return (0);
}

//Initialize each thread used for each philosopher.
int	philosopher(t_info *p, t_thread *threads)
{
	pthread_t		end_thread;
	pthread_t		watch_thread;
	t_controller	*clock;

	if (set_time(threads) != 0)
		return (-1);
	if (init_death(&end_thread, p, threads) != 0)
		print_error("TO IMPPLLEEMMEENNT (philosopher)");
	if (init_clock_thread(&watch_thread, &clock, p, threads) != 0)
		print_error("TO implement");
	if (launch_threads(p, threads) != 0)
		return (1);
	if (verify_threads(threads, clock) == 0)
	{
		// free_print("One thread failed.", p, threads);
		return (-1);
	}
	// if (pthread_mutex_destroy(&turn) != 0)
	// 	free_print("Failed to destroy the turn mutex", p, threads, forks);
	return (0);
}

int	main(int argc, char **argv)
{
	t_info			*philo;
	t_thread		*threads;
	pthread_mutex_t	**forks;

	if (argc < 5 || argc > 6)
		return (print_error("Usage : ./philo <nb> <ttd> <tte> <tts> [nb_e]."));
	philo = ft_parse(argc - 1, argv + 1);
	if (philo == NULL)
		return (1);
	forks = init_forks(philo->nb_philos);
	if (forks == NULL)
		return (free(philo), 1);
	// All good
	threads = init_threads(philo, forks);
	if (threads == NULL)
	{
		free(philo);
		free_forks(forks, philo->nb_philos);
		print_error("Threads failed to be initialised.");
		return (1);
	}
	if (synchronize_philosophers(threads) != 0)
	{
		return (1);
	}
	if (philosopher(philo, threads) != 0)
		return (1);
	// free_forks(forks, philo->nb_philos);
	// free_threads(threads);
	free(philo);
}
