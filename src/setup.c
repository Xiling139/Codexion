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

t_dongle	*dongle_initialize(t_config config)
{
	t_dongle	*dongles;
	int			size;
	int			i;

	size = config.number_of_coders;
	dongles = (t_dongle *)malloc(sizeof(t_dongle) * size);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dongles[i].available = true;
		dongles[i].id = i;
		dongles[i].t_unlock_ms = -1;
		pthread_cond_init(&dongles[i].cv_dongle, NULL);
		i++;
	}
	return (dongles);
}

t_hub	*hub_setup(t_config config, pthread_t *coders)
{
	t_hub	*hub;

	hub = (t_hub *)malloc(sizeof(t_hub));
	if (!hub)
		return (NULL);
	if (gettimeofday(&hub->start_time, NULL) == -1)
		return (NULL);
	hub->config = config;
	hub->coders = coders;
	hub->termination_signal = 0;
	pthread_mutex_init(&hub->d_mutex, NULL);
	pthread_mutex_init(&hub->p_mutex, NULL);
	pthread_cond_init(&hub->cv, NULL);
	hub->dongles = dongle_initialize(config);
	hub->burnout_time = init_int_arr(config);
	if (hub->dongles == NULL || hub->burnout_time == NULL)
		return (NULL);
	return (hub);
}

t_coder_arg	*arg_setup(t_config config, pthread_t *coders, int size)
{
	t_coder_arg	*args;
	t_hub		*hub;
	int			i;

	args = (t_coder_arg *)malloc(sizeof(t_coder_arg) * (size));
	if (!args)
		return (NULL);
	hub = hub_setup(config, coders);
	if (!hub)
		return (NULL);
	i = 0;
	while (i < size)
	{
		args[i].thread_id = i;
		args[i].hub = hub;
		i++;
	}
	return (args);
}

void	release(t_coder_arg *args)
{
	t_hub	*hub;
	int		i;

	i = 0;
	hub = args->hub;
	while (i < hub->config.number_of_coders)
	{
		pthread_cond_broadcast(&hub->dongles[i].cv_dongle);
		pthread_cond_destroy(&hub->dongles[i].cv_dongle);
		i++;
	}
	free(hub->dongles);
	free(hub->coders);
	free(hub->burnout_time);
	pthread_mutex_destroy(&hub->d_mutex);
	pthread_mutex_destroy(&hub->p_mutex);
	pthread_cond_destroy(&hub->cv);
	free(hub);
	free(args);
}

int	setup(t_config config)
{
	pthread_t	*threads;
	t_coder_arg	*args;
	int			i;
	int			size;

	size = config.number_of_coders + 1;
	threads = (pthread_t *)malloc(sizeof(pthread_t) * (size));
	if (!threads)
		return (-1);
	args = arg_setup(config, threads, size);
	if (!args)
		return (-1);
	i = 0;
	while (i < config.number_of_coders)
	{
		pthread_create(&threads[i], NULL, coder, (void *)&args[i]);
		i++;
	}
	pthread_create(&threads[i], NULL, monitor_run, (void *)&args[i]);
	wait_threads(args->hub);
	release(args);
	return (0);
}
