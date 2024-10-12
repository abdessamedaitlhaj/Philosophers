/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 08:38:43 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 20:17:58 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <time.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_philo_args
{
	int				nb_philo;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				meals;
	sem_t			*fork;
	sem_t			*print;
}	t_philo_args;

typedef struct s_philo
{
	int				id;
	long			last_eat;
	int				eat_count;
	long			start_time;
	sem_t			*last_eat_sem;
	sem_t			*eat_count_sem;
	t_philo_args	*args;
	pthread_t		thread;
}	t_philo;

void			error(void);
void			free_args(char **args);
int				ft_atoi(char const *str);
int				ft_isspace(int c);
int				ft_isdigit(int c);
char			**ft_split(char const *s, char c);
int				checking(t_philo_args *arg);
long			get_time(void);
int				checking(t_philo_args *arg);
void			ft_usleep(long sleep_time);
int				philo_dead(t_philo *philos);
int				get_eat_count(t_philo *philo);
int				philos_eat(t_philo *philos);
void			*monitoring(void *data);
t_philo			*init_philos(t_philo_args *args);
pthread_mutex_t	*init_forks(int nb_forks, t_philo *philo);
int				init_data(t_philo_args *args);
int				check_args(t_philo_args *args, char **av);
long			last_eat(t_philo *philo);
void			print(t_philo *philo, int id, char *str);
int				take_fork(t_philo *philo);
void			philo_eating(t_philo *philo);
void			*philo_routine(void *data);
int				clean_up(t_philo_args *args, t_philo *philos);
void			wait_and_kill(pid_t *pids, int nb_philo);
char	*ft_itoa(int n);
char	*ft_strjoin(const char *s1, const char *s2);

#endif