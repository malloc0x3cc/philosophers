/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 13:56:37 by madelwau          #+#    #+#             */
/*   Updated: 2026/08/22 12:14:30 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_philo_dead(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal_time > data->time_to_die)
	{
		pthread_mutex_lock(&data->dead_mutex);
		data->dead_flag = 1;
		pthread_mutex_unlock(&data->dead_mutex);
		pthread_mutex_lock(&data->write_mutex);
		printf("[%ldms] %d died\n", get_time() - data->start_time, philo->id);
		pthread_mutex_unlock(&data->write_mutex);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

static int	check_death_and_meals(t_data *data, t_philo *philos)
{
	int	i;
	int	finished_eating;

	while (1)
	{
		i = -1;
		finished_eating = 0;
		while (++i < data->nb_philos)
		{
			if (is_philo_dead(&philos[i], data))
				return (1);
			pthread_mutex_lock(&philos[i].meal_mutex);
			if (data->must_eat_count != -1
				&& philos[i].meals_eaten >= data->must_eat_count)
				finished_eating++;
			pthread_mutex_unlock(&philos[i].meal_mutex);
		}
		pthread_mutex_lock(&data->dead_mutex);
		if (data->must_eat_count != -1 && finished_eating == data->nb_philos)
			return (data->dead_flag = 1,
				pthread_mutex_unlock(&data->dead_mutex), 1);
		pthread_mutex_unlock(&data->dead_mutex);
		usleep(1000);
	}
}

static void	clean_all(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	free(data->forks);
	free(philos);
}

static int	start_threads(t_data *data, t_philo *philos)
{
	int	i;

	data->start_time = get_time();
	i = -1;
	while (++i < data->nb_philos)
	{
		philos[i].last_meal_time = data->start_time;
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
	{
		printf("USAGE: %s n_of_philo death_time eat_time sleep_time", av[0]);
		return (printf(" [n_of_times_each_philo_must_eat]\n"), EXIT_FAILURE);
	}
	if (!init_data(&data, ac, av))
		return (EXIT_FAILURE);
	philos = malloc(sizeof(t_philo) * data.nb_philos);
	if (!philos)
		return (free(data.forks), EXIT_FAILURE);
	init_philos(&data, philos);
	if (!start_threads(&data, philos))
		return (clean_all(&data, philos), EXIT_FAILURE);
	check_death_and_meals(&data, philos);
	i = -1;
	while (++i < data.nb_philos)
		pthread_join(philos[i].thread, NULL);
	return (clean_all(&data, philos), EXIT_SUCCESS);
}
