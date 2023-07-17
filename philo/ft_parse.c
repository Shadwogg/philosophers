/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:59:43 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/17 18:03:07 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	**init_forks(unsigned int nb, t_philo *p, t_thread *t)
{
	pthread_mutex_t	**forks;
	unsigned int	ct;

	forks = malloc(sizeof(pthread_mutex_t *) * nb);
	if (forks == NULL)
		fail_forks("Fork failed to be initialized in init_forks.", p, t);
	ct = 0;
	while (ct < nb)
	{
		forks[ct] = malloc(sizeof(pthread_mutex_t));
		if (forks[ct] == NULL)
		{
			free_forks(forks, ct);
			fail_forks("Fork failed to be initialized in init_forks.", p, t);
		}
		if (pthread_mutex_init(forks[ct], NULL) != 0)
			print_error("Implement");
		ct++;
	}
	return (forks);
}

t_thread	*init_threads(t_philo *philo)
{
	unsigned int	ct;
	t_thread		*threads;

	threads = malloc(sizeof(t_thread));
	if (threads == NULL)
	{
		free(philo);
		print_error("t_thread failed to be malloc.");
	}
	threads->next = NULL;
	threads->numero = 1;
	ct = 1;
	while (ct++ < philo->nb_philos)
		add_list(&threads, philo);
	return (threads);
}

int	input_is_invalid(int argc, char **argv)
{
	int	ct;

	ct = -1;
	while (++ct < argc)
	{
		if (is_not_number(argv[ct]))
			print_error("Should only have positive numbers");
		if (is_not_int(argv[ct]))
			print_error("Should have only int");
	}
	if (argc == 5 && ft_atoi(argv[4]) == 0)
		exit(EXIT_SUCCESS);
	return (0);
}

t_philo	*ft_parse(int argc, char **argv)
{
	int		ct;
	int		arr[5];
	t_philo	*philo;

	input_is_invalid(argc, argv);
	ct = -1;
	while (++ct < argc)
		arr[ct] = ft_atoi(argv[ct]);
	philo = malloc(sizeof(t_philo));
	if (philo == NULL)
		print_error("Malloc error in ft_parse()\n");
	philo->times = 0;
	philo->nb_philos = arr[0];
	philo->ttd = arr[1];
	philo->tte = arr[2];
	philo->tts = arr[3];
	if (argc == 5)
		philo->times = arr[4];
	return (philo);
}
