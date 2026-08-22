/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:28:39 by madelwau          #+#    #+#             */
/*   Updated: 2026/08/22 12:10:36 by madelwau         ###   ########.fr       */
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
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (0);
	data->dead_flag = 0;
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (0);
	return (1);
}

void	init_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 0;
	while (i < data->nb_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].data = data;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		if (philos[i].id % 2 == 0)
		{
			philos[i].left_fork = &data->forks[(i + 1) % data->nb_philos];
			philos[i].right_fork = &data->forks[i];
		}
		else
		{
			philos[i].left_fork = &data->forks[i];
			philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		}
		i++;
	}
}

static int	check_dead_flag(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->dead_mutex);
	res = data->dead_flag;
	pthread_mutex_unlock(&data->dead_mutex);
	return (res);
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status("has taken a fork", philo);
	if (philo->data->nb_philos == 1)
	{
		ft_usleep(philo->data->time_to_die, philo->data);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_status("has taken a fork", philo);
	pthread_mutex_lock(&philo->meal_mutex);
	print_status("is eating", philo);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		usleep(1500);
	while (!check_dead_flag(philo->data))
	{
		philo_eat(philo);
		if (philo->data->nb_philos == 1)
			break ;
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		print_status("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		print_status("is thinking", philo);
	}
	return (NULL);
}
