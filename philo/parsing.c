/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 17:00:00 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/11 12:34:25 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *nptr)
{
	long	n;

	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
		nptr++;
	if (*nptr == '+')
		nptr++;
	if (!ft_isdigit(*nptr))
		return (-1);
	n = 0;
	while (*nptr)
	{
		if (!ft_isdigit(*nptr))
			return (-1);
		n = (n * 10) + (*nptr++ - '0');
		if (n > 2147483647)
			return (-1);
	}
	return ((int)n);
}
