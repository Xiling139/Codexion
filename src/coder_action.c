/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:41:06 by zhewu             #+#    #+#             */
/*   Updated: 2026/06/13 12:21:47 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_hub *hub, int tid)
{
	long long	time_ms;

	time_ms = gettime_ms(hub->start_time);
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 1, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	hub->burnout_time[tid - 1] = time_ms + hub->config.time_to_burnout * 1000;
	usleep(hub->config.time_to_compile * 1000);
}

void	debug(t_hub *hub, int tid)
{
	long long	time_ms;

	time_ms = gettime_ms(hub->start_time);
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 2, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	usleep(hub->config.time_to_debug * 1000);
}

void	refactor(t_hub *hub, int tid)
{
	long long	time_ms;

	time_ms = gettime_ms(hub->start_time);
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 3, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	usleep(hub->config.time_to_refactor * 1000);
}

int	coder_action(t_hub *hub, int tid)
{
	int	size;

	size = hub->config.number_of_coders;
	compile(hub, tid);
	d_release(hub, tid);
	if (hub->config.scheduler == 1)
	{
		pthread_cond_broadcast(&hub->dongles[tid - 1].cv_dongle);
		pthread_cond_broadcast(&hub->dongles[tid % size].cv_dongle);
	}
	if (hub->termination_signal == 1)
		return (-1);
	debug(hub, tid);
	if (hub->termination_signal == 1)
		return (-1);
	refactor(hub, tid);
	if (hub->termination_signal == 1)
		return (-1);
	return (0);
}
