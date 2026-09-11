/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 13:56:37 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/11 12:36:40 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_philo_dead(t_philo *philo, t_data *data)
{
	long	last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (get_time() - last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->dead_mutex);
		data->dead_flag = 1;
		pthread_mutex_unlock(&data->dead_mutex);
		pthread_mutex_lock(&data->write_mutex);
		printf("[%ldms] %d died\n", get_time() - data->start_time, philo->id);
		pthread_mutex_unlock(&data->write_mutex);
		return (1);
	}
	return (0);
}

static int	check_meals(t_data *data, t_philo *philo)
{
	int	full;

	full = 0;
	pthread_mutex_lock(&philo->meal_mutex);
	if (data->must_eat_count != -1
		&& philo->meals_eaten >= data->must_eat_count)
		full = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (full);
}

static int	check_death_and_meals(t_data *data, t_philo *philos)
{
	int	i;
	int	finished;

	while (1)
	{
		i = -1;
		finished = 0;
		while (++i < data->nb_philos)
		{
			if (is_philo_dead(&philos[i], data))
				return (1);
			finished += check_meals(data, &philos[i]);
		}
		if (data->must_eat_count != -1 && finished == data->nb_philos)
		{
			pthread_mutex_lock(&data->dead_mutex);
			data->dead_flag = 1;
			pthread_mutex_unlock(&data->dead_mutex);
			return (1);
		}
		usleep(1000);
	}
}

static int	start_threads(t_data *data, t_philo *philos)
{
	int	i;

	data->start_time = get_time();
	i = -1;
	while (++i < data->nb_philos)
		philos[i].last_meal_time = data->start_time;
	i = -1;
	while (++i < data->nb_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]))
			return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;
	int		i;

	if (ac != 5 && ac != 6)
		return (printf("Error: invalid arguments\n"), 1);
	philos = NULL;
	data.forks = NULL;
	if (!init_data(&data, ac, av))
		return (printf("Error: invalid parameters\n"), 1);
	if (!init_philos(&data, &philos))
		return (clean_all(&data, philos), 1);
	if (!start_threads(&data, philos))
		return (clean_all(&data, philos), 1);
	check_death_and_meals(&data, philos);
	i = -1;
	while (++i < data.nb_philos)
		pthread_join(philos[i].thread, NULL);
	return (clean_all(&data, philos), 0);
}
