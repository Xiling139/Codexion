/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 15:17:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/10/20 15:46:51 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	hub_free_partial(t_hub *hub)
{
	int	i;

	if (!hub)
		return ;
	if (hub->dongles)
	{
		i = 0;
		while (i < hub->config.number_of_coders)
		{
			pthread_mutex_destroy(&hub->dongles[i].mutex);
			i++;
		}
		free(hub->dongles);
	}
	free(hub->burnout_time);
	pthread_mutex_destroy(&hub->d_mutex);
	pthread_mutex_destroy(&hub->p_mutex);
	pthread_mutex_destroy(&hub->arr_mutex);
	pthread_mutex_destroy(&hub->signal_mutex);
	free(hub);
}

void	join_and_release(pthread_t *threads, t_coder_arg *args, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	release(args);
}

void	release(t_coder_arg *args)
{
	t_hub	*hub;
	int		i;

	i = 0;
	hub = args->hub;
	while (i < hub->config.number_of_coders)
	{
		pthread_mutex_destroy(&hub->dongles[i].mutex);
		i++;
	}
	free(hub->dongles);
	free(hub->coders);
	free(hub->burnout_time);
	pthread_mutex_destroy(&hub->d_mutex);
	pthread_mutex_destroy(&hub->p_mutex);
	pthread_mutex_destroy(&hub->arr_mutex);
	pthread_mutex_destroy(&hub->signal_mutex);
	free(hub);
	free(args);
}
