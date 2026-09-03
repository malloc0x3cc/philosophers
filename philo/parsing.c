/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 17:00:00 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/03 17:11:02 by madelwau         ###   ########.fr       */
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

static int	ft_isspace(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *nptr)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if ((*nptr == '+' || *nptr == '-') && *nptr++ == '-')
		sign = -1;
	while (ft_isdigit(*nptr))
		n = (n * 10) + (*nptr++ - '0');
	return (n * sign);
}
