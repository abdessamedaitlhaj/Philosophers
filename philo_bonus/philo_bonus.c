/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 08:40:28 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 20:25:14 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print(t_philo *philo, int id, char *str)
{
	long	now_time;

	sem_wait(philo->args->print);
	now_time = get_time();
	printf("%ld %d %s\n", now_time - philo->start_time, id, str);
	sem_post(philo->args->print);
}

int	take_fork(t_philo *philo)
{
	if (philo->args->nb_philo == 1)
	{
		sem_wait(philo->args->fork);
		print(philo, philo->id, "has taken a fork");
		sem_post(philo->args->fork);
		return (1);
	}
	sem_wait(philo->args->fork);
	print(philo, philo->id, "has taken a fork"); 
	sem_wait(philo->args->fork);
	print(philo, philo->id, "has taken a fork");
	return (0);
} 

void	philo_eating(t_philo *philo)
{
	print(philo, philo->id, "is eating");
	sem_wait(philo->last_eat_sem);
	philo->last_eat = get_time();
	sem_post(philo->last_eat_sem);
	ft_usleep(philo->args->eat_time);
	sem_wait(philo->eat_count_sem);
	philo->eat_count++;
	sem_post(philo->eat_count_sem);
	sem_post(philo->args->fork);
	sem_post(philo->args->fork);
}

// long	get_last_eat(t_philo *philo)
// {
// 	long	last_eat;

// 	sem_wait(philo->check);
// 	last_eat = philo->last_eat;
// 	sem_post(philo->check);
// 	return (last_eat);
// }

// int	get_eat_count(t_philo *philo)
// {
// 	int	eat_count;

// 	sem_wait(philo->check);
// 	eat_count = philo->eat_count;
// 	sem_post(philo->check);
// 	return (eat_count);
// }

void	*monitor(void *data)
{
	t_philo		*philo;
	long		now_time;
	int			meals;

	philo = (t_philo *)data;
	meals = philo->args->meals;
	while (1)
	{
		sem_wait(philo->last_eat_sem);
		now_time = get_time();
		if (now_time - philo->last_eat >= philo->args->die_time)
		{
		sem_post(philo->last_eat_sem);
			sem_wait(philo->args->print);
			printf("%ld %d died\n", get_time() - philo->start_time, philo->id);
			exit(1);
		}
		sem_post(philo->last_eat_sem);
		sem_wait(philo->eat_count_sem);
		if (meals != -1 && philo->eat_count >= meals)
		{
			sem_post(philo->eat_count_sem);
			return (NULL);
		}
		sem_post(philo->eat_count_sem);
		ft_usleep(philo->args->eat_time / 2);
	}
}

void	*philo_routine(void *data)
{
	t_philo		*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(philo->args->eat_time);
	if (pthread_create(&philo->thread, NULL, (void *)monitor, philo))
		return (NULL);
	while (1)
	{
		if (take_fork(philo))
			break ;
		philo_eating(philo);
		print(philo, philo->id, "is sleeping");
		ft_usleep(philo->args->sleep_time);
		print(philo, philo->id, "is thinking");
		sem_wait(philo->eat_count_sem);
		if (philo->args->meals != -1 && philo->eat_count >= philo->args->meals)
		{
			sem_post(philo->eat_count_sem);
			break ;
		}
		sem_post(philo->eat_count_sem);
	}
	if (pthread_join(philo->thread, NULL))
		return (NULL);
	return (NULL);
}

t_philo	*init_philos(t_philo_args *args)
{
	t_philo		*philos;
	int			i;
	long		start_time;
	char		*eat;
	char		*count;

	philos = malloc(sizeof(t_philo) * args->nb_philo);
	if (!philos)
		return (NULL);
	start_time = get_time();
	i = 0;
	while (i < args->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].last_eat = 0;
		philos[i].eat_count = 0;
		philos[i].thread = 0;
		philos[i].args = args;
		philos[i].start_time = start_time;
		philos[i].last_eat = start_time;
		eat = ft_strjoin("eat", ft_itoa(i));
		count = ft_strjoin("count", ft_itoa(i));
		sem_unlink(eat);
		sem_unlink(count);
		philos[i].last_eat_sem = sem_open(eat, O_CREAT, 0644, 1);
		philos[i].eat_count_sem = sem_open(count, O_CREAT, 0644, 1);
		free(eat);
		free(count);
		i++;
	}
	return (philos);
}

int	init_data(t_philo_args *args)
{
	sem_unlink("fork");
	sem_unlink("print");
	args->fork = sem_open("fork", O_CREAT, 0644, args->nb_philo);
	args->print = sem_open("print", O_CREAT, 0644, 1);
	return (0);
}

int	clean_up(t_philo_args *args, t_philo *philos)
{
	int	i;
	char	*eat;
	char	*count;

	sem_close(args->fork);
	sem_close(args->print);
	sem_unlink("fork");
	sem_unlink("print");
	i = 0;
	while (i < args->nb_philo)
	{
		eat = ft_strjoin("eat", ft_itoa(i));
		count = ft_strjoin("count", ft_itoa(i));
		sem_close(philos[i].last_eat_sem);
		sem_close(philos[i].eat_count_sem);
		sem_unlink(eat);
		sem_unlink(count);
		free(eat);
		free(count);
		i++;
	}
	free(philos);
	return (0);
}

void	wait_and_kill(pid_t *pids, int nb_philo)
{
	int	i;
	int	status;

	i = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status) != 0)
		{
			i = -1;
			while (++i < nb_philo)
				kill(pids[i], SIGKILL);
			break ;
		}
		i++;
	}
}

int	main(int ac, char *av[])
{
	t_philo			*philos;
	t_philo_args	args;
	pid_t			*pids;
	int				i;

	if (ac < 5 || ac > 6)
		return (write(2, "Error\n", 6));
	if (check_args(&args, av))
		return (write(2, "Error\n", 6));
	if (init_data(&args))
		return (write(2, "Error\n", 6));
	philos = init_philos(&args);
	if (!philos)
		return (write(2, "Error\n", 6));
	pids = malloc(sizeof(pid_t) * args.nb_philo);
	if (!pids)
		return (write(2, "Error\n", 6));
	i = 0;
	while (i < args.nb_philo)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free(philos);
			return (write(2, "fork failed\n", 6));
		}
		if (pids[i] == 0)
		{
			philo_routine(&philos[i]); 
			exit(0);
		}
		i++;
	}
	wait_and_kill(pids, args.nb_philo);
	if (clean_up(&args, philos))
		return (write(2, "Error\n", 6));
	return (0);
}
