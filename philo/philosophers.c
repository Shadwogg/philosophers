/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:10 by ggiboury          #+#    #+#             */
/*   Updated: 2023/07/09 17:50:29 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_error(char *str)
{
	size_t	len;

	if (str == NULL)
		exit(EXIT_FAILURE);
	len = 0;
	while (str[len] != 0)
		len++;
	write(2, str, len);
	exit(EXIT_FAILURE);
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
	philo->nb_philos = arr[0];
	philo->ttd = arr[1];
	philo->tte = arr[2];
	philo->tts = arr[3];
	if (argc == 4)
		philo->times = arr[4];
	else
		philo->times = 0;
	return (philo);
}

void	add_list(t_thread **t, t_philo *ref)
{
	t_thread		*cur;
	unsigned int	ct;

	cur = *t;
	ct = 1;
	while (cur->next != NULL)
	{
		ct++;
		cur = cur->next;
	}
	cur->next = malloc(sizeof(t_thread));
	if (cur->next == NULL)
	{
		free(ref);
		free_threads(*t);
		print_error("t_thread failed to be malloc.");
	}
	cur->next->next = NULL;
	cur->next->numero = ct;
	cur->next->thread = NULL;
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
	threads->thread = NULL;
	ct = 1;
	while (ct++ < philo->nb_philos)
		add_list(&threads, philo);
	return (threads);
}

void	print_philo(t_philo *p)
{
	printf("Philo\n");
	printf("ttd = %u\ntte = %u\ntts = %u\n", p->ttd, p->tte, p->tts);
}

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

t_table	*set_table(t_philo *ref, char **forks, unsigned int ct)
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
	printf("	-1\n");
	if (ct == 0)
		table->left_fork = forks[ref->nb_philos - 1];
	else
		table->left_fork = forks[ct - 1];
	table->right_fork = forks[ct];
	table->philo = ref->nb_philos;
	return (table);
}

char	**init_forks(unsigned int nb)
{
	char			**forks;
	unsigned int	ct;

	forks = malloc(sizeof(char *) * nb);
	if (forks == NULL)
		return (NULL);
	ct = 0;
	printf("nb = %u\n", nb);
	while (ct < nb)
	{
		printf("	+1\n");
		forks[ct] = malloc(sizeof(char));
		if (forks[ct] == NULL)
		{
			free_forks(forks, ct);
			return (NULL);
		}
		*(forks[ct]) = 'f';
		ct++;
	}
	return (forks);
}

void	*live(void *table)
{
	// t_table	t;

	if (table == NULL)
		return (NULL);
	// t = (t_table *) table;
	sleep(5);
	printf("Coucou\n");
	free_table(table);
	return (NULL);
}

void	philosopher(t_philo *p, t_thread *t)
{
	t_thread		*cur;
	t_table			*table;
	char			**forks;
	unsigned int	ct;

	cur = t;
	forks = init_forks(p->nb_philos);
	if (forks == NULL)
	{
		free(p);
		free_threads(t);
		print_error("Forks failed to be picked in init_forks()");
	}
	ct = 0;
	while (cur != NULL)
	{
		table = set_table(p, forks, ct++);
		if (table == NULL)
		{
			free(p);
			free_forks(forks, p->nb_philos);
			free_threads(t);
			print_error("Table failed to be set in philosopher");
		}
		// printf("cur =%p \n", cur->thread);
		pthread_create(&cur->thread, NULL, &live, table);
		cur = cur->next;
		ct++;
	}
	cur = t;
	while (cur != NULL)
	{
		printf("FIn\n");
		pthread_join(cur->thread, NULL);
		cur = cur->next;
	}
	free_forks(forks, p->nb_philos);
}

int	main(int argc, char **argv)
{
	t_philo		*philo;
	t_thread	*threads;

	if (argc < 5 || argc > 6)
		print_error("Usage : ./philo <nb> <ttd> <tte> <tts> [nb_e].");
	philo = ft_parse(argc - 1, argv + 1);
	if (philo->nb_philos == 0)
	{
		free(philo);
		return (1);
	}
	print_philo(philo);
	// pthread_mutex_init();
	threads = init_threads(philo);
	philosopher(philo, threads);
	free_threads(threads);
	free(philo);
	// pthread_mutex_destroy();
}
