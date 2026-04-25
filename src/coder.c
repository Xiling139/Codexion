/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/04/25 16:47:26 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	grab_dongle(t_hub *hub)
{
	int	i;

	i = 0;
	while (i < hub->config.number_of_coders)
	{
		if (hub->dongles[i].available)
		{
			hub->dongles[i].available = false;
		}
		i++;
	}
}

void	*coder(void *arg)
{
	t_hub	*hub;
	int		tid;

	hub = (t_hub *)arg;
	tid = hub->thread_id;
	printf("Thread ID: %d initialized.\n", tid);
	if (compile_available(hub))
	{
		pthread_mutex_lock(&hub->d_mutex);
	}
	return (NULL);
}
