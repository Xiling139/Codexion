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

void	wait_threads(pthread_t *threads, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

t_coder_arg	*hub_setup(t_config config, pthread_t *coders, int size)
{
	t_coder_arg	*args;
	t_hub		*hub;
	int			i;

	args = (t_coder_arg *)malloc(sizeof(t_coder_arg) * (size));
	hub = (t_hub *)malloc(sizeof(t_hub));
	if (gettimeofday(&hub->start_time, NULL) == -1)
		return (NULL);
	hub->config = config;
	hub->coders = coders;
	hub->free_dongles = config.number_of_coders;
	i = 0;
	while (i < size)
	{
		args[i].thread_id = i;
		args[i].hub = hub;
		i++;
	}
	return (args);
}

void	free_resource(t_coder_arg *args)
{
	t_hub	*hub;

	hub = args->hub;
	free(hub->coders);
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
	args = hub_setup(config, threads, size);
	if (!args)
		return (-1);
	i = 0;
	while (i < config.number_of_coders)
	{
		pthread_create(&threads[i], NULL, coder, (void *)&args[i]);
		usleep(1000);
		i++;
	}
	pthread_create(&threads[i], NULL, monitor_run, (void *)&args[i]);
	wait_threads(threads, size);
	free_resource(args);
	return (0);
}
