/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 22:14:16 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 15:58:33 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philos(t_philo_args *args)
{
	t_philo		*philos;
	int			i;

	philos = malloc(sizeof(t_philo) * args->nb_philo);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < args->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].right_fork = i;
		philos[i].left_fork = (i + 1) % args->nb_philo;
		philos[i].eat_count = 0;
		philos[i].args = args;
		i++;
	}
	return (philos);
}

pthread_mutex_t	*init_forks(int nb_forks, t_philo *philo)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * nb_forks);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < nb_forks)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (NULL);
		pthread_mutex_init(&philo[i].mutex_eat_count, NULL);
		pthread_mutex_init(&philo[i].mutex_last_eat, NULL);
		i++;
	}
	pthread_mutex_init(&philo[0].args->mutex_dead, NULL);
	return (forks);
}

int	init_data(t_philo_args *args, t_philo **philos)
{
	pthread_mutex_init(&args->print, NULL);
	*philos = init_philos(args);
	if (!*philos)
		return (1);
	args->forks = init_forks(args->nb_philo, *philos);
	if (!args->forks)
	{
		free(*philos);
		return (1);
	}
	return (0);
}
