/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:11:57 by zhewu             #+#    #+#             */
/*   Updated: 2026/04/25 11:19:58 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_threads(pthread_t *threads, pthread_t monitor, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

t_dongle	*dongle_initialize(t_config config)
{
	t_dongle	*dongles;
	int			i;

	i = 0;
	dongles = (t_dongle *)malloc(sizeof(t_dongle) * config.number_of_coders);
	if (!dongles)
		return (NULL);
	while (i < config.number_of_coders)
	{
		dongles[i].cd_end = 0;
		dongles[i].available = true;
		i++;
	}
	return (dongles);
}

t_hub	*hub_initialize(t_config config, pthread_t *coders)
{
	t_hub	*hub;

	hub = (t_hub *)malloc(sizeof(t_hub));
	if (gettimeofday(&hub->start_time, NULL) == -1)
		return (NULL);
	hub->config = config;
	hub->coders = coders;
	hub->dongles = dongle_initialize(config);
	if (!hub->dongles)
		return (NULL);
	return (hub);
}

void	free_hub(t_hub *hub)
{
	free(hub->coders);
	free(hub->dongles);
}

int	setup(t_config config)
{
	pthread_t	*coders;
	t_hub		*working_hub;
	int			i;
	pthread_t	monitor;

	coders = (pthread_t *)malloc(sizeof(pthread_t) * (config.number_of_coders));
	if (!coders)
		return (-1);
	working_hub = hub_initialize(config, coders);
	if (!working_hub)
		return (-1);
	i = 0;
	while (i < config.number_of_coders)
	{
		working_hub->thread_id = i;
		pthread_create(&coders[i], NULL, coder, (void *)working_hub);
		usleep(1000);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_run, (void *)working_hub);
	wait_threads(coders, monitor, config.number_of_coders);
	free_hub(working_hub);
	return (0);
}
