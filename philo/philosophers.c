/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 19:55:55 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_controller(t_controller *ctler, t_info *info, t_thread *threads)
{
	t_thread		*cur;
	unsigned int	ct;

	ctler->number_philo = info->nb_philos;
	ctler->philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	if (ctler->philos == NULL)
		return (p_error("Philos failed to be allocated.\n"), free(ctler), -1);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		ctler->philos[ct++] = cur->philo;
		cur = cur->next;
	}
	return (0);
}

void	end_shared_ressources(t_philosopher *philo)
{
	pthread_mutex_destroy(philo->turn);
	free(philo->turn);
}

int	set_time(t_thread *threads)
{
	struct timeval	*start;

	if (gettimeofday(threads->philo->timer.clock, NULL) != 0)
		return (p_error("Can't get time.\n"), -1);
	start = threads->philo->timer.clock;
	while (threads != NULL)
	{
		threads->philo->timer.start.tv_sec = start->tv_sec;
		threads->philo->timer.start.tv_usec = start->tv_usec;
		threads->philo->timer.last_eaten.tv_sec = start->tv_sec;
		threads->philo->timer.last_eaten.tv_usec = start->tv_usec;
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
		return (-1);
	if (init_clock_thread(&watch_thread, &clock, p, threads) != 0)
		return (-1);
	if (launch_threads(threads) != 0)
		return (-1);
	if (verify_threads(threads, clock) == 0)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_info			*philo;
	t_thread		*threads;
	pthread_mutex_t	**forks;

	if (argc < 5 || argc > 6)
		return (p_error("Usage : ./philo <nb> <ttd> <tte> <tts> [nb_e]."));
	philo = ft_parse(argc - 1, argv + 1);
	if (philo == NULL)
		return (1);
	forks = init_forks(philo->nb_philos);
	if (forks == NULL)
		return (free(philo), 1);
	threads = init_threads(philo, forks);
	if (threads == NULL)
		return (free(philo), free_forks(forks, philo->nb_philos),
			p_error("Threads failed to be initialised."));
	if (synchronize_philosophers(threads) != 0)
		return (free_threads(threads), free(philo),
			free_forks(forks, ft_atoi(argv[1])), 1);
	if (philosopher(philo, threads) != 0)
		return (free_threads(threads), free(philo),
			free_forks(forks, ft_atoi(argv[1])), 1);
	free_forks(forks, ft_atoi(argv[1]));
	free(philo);
}
