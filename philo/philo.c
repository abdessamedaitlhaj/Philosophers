/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 08:40:28 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/14 12:31:03 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(t_philo_args *args, t_philo *philos)
{
	clean_up(args, philos);
	write(2, "Error\n", 6);
	return (1);
}

void	clean_up(t_philo_args *args, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args->nb_philo)
	{
		pthread_mutex_destroy(&(args->forks[i]));
		pthread_mutex_destroy(&(philos[i].mutex_eat_count));
		pthread_mutex_destroy(&(philos[i].mutex_last_eat));
		i++;
	}
	pthread_mutex_destroy(&args->print);
	free(args->forks);
	free(philos);
}

int	join_threads(t_philo *philos, pthread_t *monitor, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		if (pthread_join(philos[i].thread, NULL))
			return (1);
		i++;
	}
	if (pthread_join(*monitor, NULL))
		return (1);
	return (0);
}

int	create_threads(t_philo *philos, t_philo_args *args)
{
	int		i;
	long	start_time;

	start_time = get_time();
	i = 0;
	while (i < args->nb_philo)
	{
		philos[i].start_time = start_time;
		pthread_mutex_lock(&philos[i].mutex_last_eat);
		philos[i].last_eat = start_time;
		pthread_mutex_unlock(&philos[i].mutex_last_eat);
		if (pthread_create(&(philos[i].thread), NULL, \
		(void *)philo_routine, &philos[i]))
			return (1);
		i++;
	}
	return (0);
}

int	main(int ac, char *av[])
{
	pthread_t		monitor;
	t_philo			*philos;
	t_philo_args	args;

	if (ac < 5 || ac > 6)
		return (write(2, "Error\n", 6));
	if (check_args(&args, av))
		return (write(2, "Error\n", 6));
	if (init_data(&args, &philos))
		return (write(2, "Error\n", 6));
	if (create_threads(philos, &args))
		return (error(&args, philos));
	if (pthread_create(&monitor, NULL, (void *)monitoring, (void *)philos))
		return (error(&args, philos));
	if (join_threads(philos, &monitor, args.nb_philo))
		return (error(&args, philos));
	clean_up(&args, philos);
	return (0);
}
