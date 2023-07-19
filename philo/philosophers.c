/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/19 19:31:03 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_philo_has_eaten(t_philosopher **philos, unsigned int nb)
{
	(void) philos;
	(void) nb;
	return (0);
}

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

//To end
int	terminate_all(t_philosopher **philos, unsigned int nb)
{
	while (nb-- > 0)
	{
		if (pthread_mutex_lock(philos[nb]->m_is_finished) != 0)
			return (1);
		*((philos[nb])->is_finished) = 1;
		if (pthread_mutex_unlock(philos[nb]->m_is_finished) != 0)
			return (1);
	}
	return (0);
}

int	simulation_is_finished(t_controller *ctler)
{
	unsigned int	ct;
	t_philosopher	*cur;
	time_t			start;
	time_t			actual;
	struct timeval	tv;

	ct = 0;
	ft_mlsleep(30);
	while (ct < ctler->number_philo)
	{
		//lock
		printf("DEBUT\n");
		cur = ctler->philos[ct];
		printf("START\n");
		start = cur->timer->start.tv_sec * 1000
			+ cur->timer->start.tv_usec / 1000;
		if (gettimeofday(&tv, NULL) != 0)
			return (-1);
		printf("HERE\n");
		actual = (tv.tv_sec * 1000 + tv.tv_usec / 1000)
			- cur->timer->time_eaten * (cur->menu->tte);
		//unlock
		printf("LA\n");
		if (actual - start >= cur->menu->ttd)
			return (1);
		ct++;
	}
	return (0);
}

void	*harvest(void *souls)
{
	t_controller	*ctler;

	ctler = souls;
	//Wait every thread to begin ?
	// while (0)
	// {

	// }
	while (!simulation_is_finished(ctler)
		&& !all_philo_has_eaten(ctler->philos, ctler->number_philo))
	{
		ft_mlsleep(2);
	}
	terminate_all(ctler->philos, ctler->number_philo);
	return (souls);
}

int	init_death(pthread_t *ending_thread, t_info *info, t_thread *threads)
{
	t_controller	controller;
	t_thread		*cur;
	unsigned int	ct;

	controller.number_philo = info->nb_philos;
	controller.philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		controller.philos[ct++] = threads->philo;
		cur = cur->next;
	}
	if (controller.philos == NULL)
		return (1);
	if (pthread_create(ending_thread, NULL, &harvest, &controller) != 0)
		return (1);
	if (pthread_detach(*ending_thread) != 0)
		return (1);
	return (0);
}

//Initialize each thread used for each philosopher.
void	philosopher(t_info *p, t_thread *threads, pthread_mutex_t **forks)
{
	pthread_t		end_thread;

	if (init_death(&end_thread, p, threads) != 0)
		print_error("TO IMPPLLEEMMEENNT (philosopher)");
	printf("Death initialized\n");
	if (launch_threads(p, threads, forks) != 0)
		return ;
	printf("Threads launched\n");
	if (verify_threads(threads) == 0)
	{
		free_print("One thread failed.", p, threads, forks);
	}
	// if (pthread_mutex_destroy(&turn) != 0)
	// 	free_print("Failed to destroy the turn mutex", p, threads, forks);
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
	forks = init_forks(philo->nb_philos, philo);
	if (gettimeofday(&tv, NULL) != 0)
		free_print("Error while getting the time", philo, NULL, forks);
	philo->start_time = tv;
	threads = init_threads(philo, forks);
	philosopher(philo, threads, forks);
	free_forks(forks, philo->nb_philos);
	free_threads(threads);
	free(philo);
}
