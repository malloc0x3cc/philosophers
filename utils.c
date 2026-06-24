/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:26:30 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/24 16:33:15 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long usec, t_data *data)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < usec)
	{
		pthread_mutex_lock(&data->dead_mutex);
		if (data->dead_flag)
		{
			pthread_mutex_unlock(&data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->dead_mutex);
		usleep(500);
	}
}

void	print_status(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->write_mutex);
	if (!philo->data->dead_flag)
		printf("[%ldms] %d %s\n",
			get_time() - philo->data->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write_mutex);
	pthread_mutex_unlock(&philo->data->dead_mutex);
}

int	ft_atoi(const char *nptr)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\n'
		|| *nptr == '\r' || *nptr == '\v' || *nptr == '\f')
		nptr++;
	if ((*nptr == '+' || *nptr == '-') && *nptr++ == '-')
		sign = -1;
	while (*nptr >= '0' && *nptr <= '9')
		n = (n * 10) + (*nptr++ - '0');
	return (n * sign);
}
