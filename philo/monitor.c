/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 22:23:02 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 16:03:01 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_eat_count(t_philo *philo)
{
	int	count;

	pthread_mutex_lock(&philo->mutex_eat_count);
	count = philo->eat_count;
	pthread_mutex_unlock(&philo->mutex_eat_count);
	return (count);
}

long	last_eat(t_philo *philo)
{
	long	last;

	pthread_mutex_lock(&philo->mutex_last_eat);
	last = philo->last_eat;
	pthread_mutex_unlock(&philo->mutex_last_eat);
	return (last);
}

int	all_eat(t_philo *philos)
{
	int				i;
	int				all_eat;
	t_philo_args	*args;

	i = 0;
	all_eat = 1;
	args = philos[0].args;
	while (i < args->nb_philo)
	{
		if (get_eat_count(&philos[i]) < args->meals)
			all_eat = 0;
		i++;
	}
	if (all_eat)
		return (1);
	return (0);
}

void	*monitoring(void *data)
{
	t_philo			*philos;
	long			now_time;
	int				i;

	philos = (t_philo *)data;
	while (1)
	{
		i = -1;
		while (++i < philos[0].args->nb_philo)
		{
			now_time = get_time();
			if (now_time - last_eat(&philos[i]) >= philos[0].args->die_time)
			{
				print(&philos[i], philos[i].id, "died");
				pthread_mutex_lock(&philos[0].args->mutex_dead);
				philos[0].args->dead = 1;
				pthread_mutex_unlock(&philos[0].args->mutex_dead);
				return (NULL);
			}
		}
		if (philos[0].args->meals != -1 && all_eat(philos))
			return (NULL);
	}
}
