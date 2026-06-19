/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:28:39 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/19 15:07:02 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
