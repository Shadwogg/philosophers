/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:59:43 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/27 17:17:39 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	**init_forks(unsigned int nb)
{
	pthread_mutex_t	**forks;
	unsigned int	ct;

	forks = malloc(sizeof(pthread_mutex_t *) * nb);
	if (forks == NULL)
		return (p_error("Forks failed to be allocated."), NULL);
	ct = 0;
	while (ct < nb)
	{
		forks[ct] = malloc(sizeof(pthread_mutex_t));
		if (forks[ct] == NULL)
		{
			free_forks(forks, ct);
			return (p_error("One fork failed to be allocated."), NULL);
		}
		if (pthread_mutex_init(forks[ct], NULL) != 0)
		{
			free_forks(forks, ct);
			return (p_error("One fork failed to be initialised."), NULL);
		}
		ct++;
	}
	return (forks);
}

int	input_is_invalid(int argc, char **argv)
{
	int	ct;

	ct = -1;
	while (++ct < argc)
	{
		if (is_not_number(argv[ct]))
			return (p_error("Only positive numbers are allowed.\n"));
		if (is_not_int(argv[ct]))
			return (p_error("Only strict positive int are allowed.\n"));
	}
	return (0);
}

t_info	*ft_parse(int argc, char **argv)
{
	int		ct;
	int		arr[5];
	t_info	*philo;

	if (input_is_invalid(argc, argv) != 0)
		return (NULL);
	ct = -1;
	while (++ct < argc)
		arr[ct] = ft_atoi(argv[ct]);
	philo = malloc(sizeof(t_info));
	if (philo == NULL)
		return (p_error("Malloc error in ft_parse()\n"), NULL);
	philo->times = 0;
	philo->nb_philos = arr[0];
	philo->ttd = arr[1];
	philo->tte = arr[2];
	philo->tts = arr[3];
	if (argc == 5)
		philo->times = arr[4];
	return (philo);
}
