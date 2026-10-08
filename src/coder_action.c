/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:41:06 by zhewu             #+#    #+#             */
/*   Updated: 2026/09/25 15:07:51 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	terminated(t_hub *hub)
{
	pthread_mutex_lock(&hub->signal_mutex);
	if (hub->termination_signal == 1)
	{
		pthread_mutex_unlock(&hub->signal_mutex);
		return (true);
	}
	else
	{
		pthread_mutex_unlock(&hub->signal_mutex);
		return (false);
	}
}

void	compile(t_hub *hub, int tid)
{
	long long	time_ms;

	time_ms = gettime_ms(hub->start_time);
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 1, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	pthread_mutex_lock(&hub->arr_mutex);
	hub->burnout_time[tid - 1] = time_ms + hub->config.time_to_burnout;
	pthread_mutex_unlock(&hub->arr_mutex);
	usleep(hub->config.time_to_compile * 1000);
}

void	debug(t_hub *hub, int tid)
{
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 2, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	usleep(hub->config.time_to_debug * 1000);
}

void	refactor(t_hub *hub, int tid)
{
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 3, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	usleep(hub->config.time_to_refactor * 1000);
}

int	coder_action(t_hub *hub, int tid)
{
	compile(hub, tid);
	release_dongles(hub, tid);
	if (terminated(hub))
		return (-1);
	debug(hub, tid);
	if (terminated(hub))
		return (-1);
	refactor(hub, tid);
	if (terminated(hub))
		return (-1);
	return (0);
}
