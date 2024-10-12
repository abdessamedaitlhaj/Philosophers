/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 09:00:51 by aait-lha          #+#    #+#             */
/*   Updated: 2024/10/12 16:01:52 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	time;
	long			result;

	if (gettimeofday(&time, NULL))
		return (write(2, "Error\n", 6));
	result = ((size_t)time.tv_sec * 1000) + ((size_t)time.tv_usec / 1000);
	return (result);
}

void	ft_usleep(long sleep_time)
{
	long	start;

	start = get_time();
	while (start + sleep_time > get_time())
		usleep(100);
}
