/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:45:12 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 15:53:09 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->args->mutex_dead);
	dead = philo->args->dead;
	pthread_mutex_unlock(&philo->args->mutex_dead);
	return (dead);
}

void	print(t_philo *philo, int id, char *str)
{
	long	now_time;

	pthread_mutex_lock(&philo->args->print);
	now_time = get_time() - philo->start_time;
	if (!get_dead(philo))
		printf("%ld %d %s\n", now_time, id, str);
	pthread_mutex_unlock(&philo->args->print);
}

int	take_fork(t_philo *philo)
{
	if (philo->args->nb_philo == 1)
	{
		pthread_mutex_lock(&(philo->args->forks[philo->left_fork]));
		print(philo, philo->id, "has taken a fork");
		pthread_mutex_unlock(&(philo->args->forks[philo->left_fork]));
		return (1);
	}
	pthread_mutex_lock(&(philo->args->forks[philo->left_fork]));
	print(philo, philo->id, "has taken a fork");
	pthread_mutex_lock(&(philo->args->forks[philo->right_fork]));
	print(philo, philo->id, "has taken a fork");
	return (0);
}

void	philo_eating(t_philo *philo)
{
	print(philo, philo->id, "is eating");
	pthread_mutex_lock(&philo->mutex_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->mutex_last_eat);
	ft_usleep(philo->args->eat_time);
	pthread_mutex_lock(&philo->mutex_eat_count);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->mutex_eat_count);
	pthread_mutex_unlock(&(philo->args->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->args->forks[philo->right_fork]));
}

void	*philo_routine(void *data)
{
	t_philo		*philo;
	int			meals;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(100);
	meals = philo->args->meals;
	while (!get_dead(philo))
	{
		if (take_fork(philo))
			break ;
		philo_eating(philo);
		print(philo, philo->id, "is sleeping");
		ft_usleep(philo->args->sleep_time);
		print(philo, philo->id, "is thinking");
		if (meals != -1 && get_eat_count(philo) >= meals)
			break ;
	}
	return (NULL);
}
