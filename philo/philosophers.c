/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/24 19:06:07 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_philo_has_eaten(t_philosopher **philos, unsigned int nb)
{
	unsigned int	ct;
	t_timer			*cur;

	ct = 0;
	while (ct < nb)
	{
		cur = philos[ct]->timer;
		if (pthread_mutex_lock(cur->mutex) != 0)
			return (-1);
		if (philos[ct]->menu->times == 0 ||
			cur->time_eaten < philos[ct]->menu->times)
		{
			// printf("time to eat %d\n", philos[ct]->menu->times);
			// printf("time eaten %d\n", cur->time_eaten);
			if (pthread_mutex_unlock(cur->mutex) != 0)
				return (-1);
			return (0);
		}
		if (pthread_mutex_unlock(cur->mutex) != 0)
			return (-1);
		ct++;
	}
	return (1);
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
	int				ct;
	t_philosopher	*cur;
	time_t			start;
	time_t			actual;
	struct timeval	tv;

	ct = -1;
	while ((unsigned int) ++ct < ctler->number_philo)
	{
		cur = ctler->philos[ct];
		start = cur->timer->last_eaten.tv_sec * 1000
			+ cur->timer->last_eaten.tv_usec / 1000;
		if (pthread_mutex_lock(cur->timer->mutex) != 0)
			return (-1);
		tv.tv_sec = cur->timer->tv->tv_sec;
		tv.tv_usec = cur->timer->tv->tv_usec;
		if (pthread_mutex_unlock(cur->timer->mutex) != 0)
			return (-1);
		actual = (tv.tv_sec * 1000 + tv.tv_usec / 1000)
			- cur->timer->time_eaten * (cur->menu->tte);
		if (actual - start >= cur->menu->ttd)
			return (ct + 1);
	}
	return (0);
}

void	*harvest(void *souls)
{
	t_controller	*ctler;
	int				philo;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	philo = simulation_is_finished(ctler);
	while (philo == 0
		&& !all_philo_has_eaten(ctler->philos, ctler->number_philo))
	{
		ft_mlsleep(2);
		philo = simulation_is_finished(ctler);
	}
	terminate_all(ctler->philos, ctler->number_philo);
	if (philo != 0)
		print_status(ctler->philos[philo - 1], "died", 1);
	return (souls);
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
// Chaue Philo a un pointeur vers timer qui a une ressoure partagé, tv
int	set_timers_philosophers(t_philosopher **philos, unsigned int nb)
{
	unsigned int	ct;

	ct = 0;
	while (ct < nb)
	{
		// philos[ct]->timer->
		if (pthread_mutex_lock(philos[ct]->timer->mutex) != 0)
			return (-1);
		;
		if (pthread_mutex_unlock(philos[ct]->timer->mutex) != 0)
			return (-1);
		ct++;
	}
	return (0);
}

void	*set_timers(void *souls)
{
	t_controller	*ctler;

	if (souls == NULL)
		return (NULL);
	ctler = souls;
	while (!simulation_is_finished(ctler))
	{
		ft_mlsleep(1);
		set_timers_philosophers(ctler->philos, ctler->number_philo);
	}
	free_controller(ctler);
	return (souls);
}

int	init_watch(pthread_t *watch_thread, t_info *info, t_thread *threads)
{
	t_controller	*watch;
	t_thread		*cur;
	unsigned int	ct;

	watch = malloc(sizeof(t_controller));
	if (watch == NULL)
		print_error("Controller null");
	watch->number_philo = info->nb_philos;
	watch->philos = malloc(sizeof(t_philosopher *) * info->nb_philos);
	cur = threads;
	ct = 0;
	while (cur != NULL)
	{
		watch->philos[ct] = cur->philo;
		ct++;
		cur = cur->next;
	}
	if (watch->philos == NULL)
		return (1);
	if (pthread_create(watch_thread, NULL, &set_timers, watch) != 0)
		return (1);
	if (pthread_detach(*watch_thread) != 0)
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
