/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:59:43 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/24 18:58:50 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	**init_forks(unsigned int nb)
{
	pthread_mutex_t	**forks;
	unsigned int	ct;

	forks = malloc(sizeof(pthread_mutex_t *) * nb);
	if (forks == NULL)
		return (NULL);
	ct = 0;
	while (ct < nb)
	{
		forks[ct] = malloc(sizeof(pthread_mutex_t));
		if (forks[ct] == NULL)
		{
			free_forks(forks, ct);
			return (NULL);
		}
		if (pthread_mutex_init(forks[ct], NULL) != 0)
		{
			free_forks(forks, ct);
			return (NULL);
		}
		ct++;
	}
	return (forks);
}

t_thread	*init_threads(t_info *info, pthread_mutex_t **forks)
{
	unsigned int	ct;
	t_thread		*threads;
	t_thread		*cur;
	pthread_mutex_t	*turn;

	threads = malloc(sizeof(t_thread));
	if (threads == NULL)
	{
		free(info);
		print_error("t_thread failed to be malloc.");
	}
	threads->next = NULL;
	threads->numero = 1;
	threads->philo = NULL;
	ct = 1;
	while (ct++ < info->nb_philos)
		add_list(&threads, info);
	turn = malloc(sizeof(pthread_mutex_t));
	if (turn == NULL)
		print_error("A implementer");
	if (pthread_mutex_init(turn, NULL) != 0)
		print_error("TO IMPLENT (philo)");
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		cur->philo = set_philosopher(info, forks, ct++, turn);
		if (cur->philo == NULL)
		{
			free_print("Philo failed to be initialized.", info, threads, forks);
			return (NULL);
			// return (1);
		}
		cur = cur->next;
	}
	return (threads);
}

int	input_is_invalid(int argc, char **argv)
{
	int	ct;

	ct = -1;
	while (++ct < argc)
	{
		if (is_not_number(argv[ct]))
			print_error("Only positive numbers are allowed");
		if (is_not_int(argv[ct]))
			print_error("Only strict positive int are allowed");
	}
	if (argc == 5 && ft_atoi(argv[4]) == 0)
		exit(EXIT_SUCCESS);
	return (0);
}

t_info	*ft_parse(int argc, char **argv)
{
	int		ct;
	int		arr[5];
	t_info	*philo;

	input_is_invalid(argc, argv);
	ct = -1;
	while (++ct < argc)
		arr[ct] = ft_atoi(argv[ct]);
	philo = malloc(sizeof(t_info));
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
