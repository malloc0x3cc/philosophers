/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:28:39 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/03 16:56:59 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!*philos)
		return (0);
	i = -1;
	while (++i < data->nb_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	i = -1;
	while (++i < data->nb_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].data = data;
		pthread_mutex_init(&(*philos)[i].meal_mutex, NULL);
		(*philos)[i].left_fork = &data->forks[i];
		(*philos)[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		if ((*philos)[i].id % 2 == 0)
		{
			(*philos)[i].left_fork = &data->forks[(i + 1) % data->nb_philos];
			(*philos)[i].right_fork = &data->forks[i];
		}
	}
	return (1);
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
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status("is eating", philo);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

static void	ft_think(t_philo *philo)
{
	long	think;

	print_status("is thinking", philo);
	if (philo->data->nb_philos % 2 != 0)
	{
		think = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
		if (think < 0)
			think = 0;
		ft_usleep(think * 0.42, philo->data);
	}
}

static int	is_philo_full(t_philo *philo)
{
	int	full;

	full = 0;
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->data->must_eat_count != -1
		&& philo->meals_eaten >= philo->data->must_eat_count)
		full = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (full);
}

void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	while (!check_dead_flag(philo->data))
	{
		philo_eat(philo);
		if (philo->data->nb_philos == 1 || is_philo_full(philo))
			break ;
		print_status("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		ft_think(philo);
	}
	return (NULL);
}
