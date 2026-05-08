/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 13:56:37 by madelwau          #+#    #+#             */
/*   Updated: 2026/05/08 14:16:07 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("USAGE: %s n_of_philo death_time eat_time sleep_time", av[0]);
		return (printf(" [n_of_times_each_philo_must_eat]\n"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
