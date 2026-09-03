/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:26:30 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/03 17:02:58 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int ac, char **av)
{
	data->nb_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->must_eat_count = -1;
	if (ac == 6)
		data->must_eat_count = ft_atoi(av[5]);
	if (data->nb_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (ac == 6 && data->must_eat_count <= 0))
		return (0);
	data->dead_flag = 0;
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
	{
		pthread_mutex_destroy(&data->dead_mutex);
		pthread_mutex_destroy(&data->write_mutex);
		return (0);
	}
	return (1);
}

void	clean_all(t_data *data, t_philo *philos)
{
	int	i;

	if (data->forks)
	{
		i = -1;
		while (++i < data->nb_philos)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	if (philos)
	{
		i = -1;
		while (++i < data->nb_philos)
			pthread_mutex_destroy(&philos[i].meal_mutex);
		free(philos);
	}
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_dead_flag(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->dead_mutex);
	res = data->dead_flag;
	pthread_mutex_unlock(&data->dead_mutex);
	return (res);
}

void	ft_usleep(long ms, t_data *data)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < ms)
	{
		if (check_dead_flag(data))
			break ;
		usleep(500);
	}
}
