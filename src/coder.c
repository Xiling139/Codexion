/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/15 17:17:32 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_hub *hub, int id)
{
	long long	timediff;
	int			size;

	size = hub->config.number_of_coders;
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d has taken a dongle\n", timediff, id);
	hub->dongles[id - 1].available = false;
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d has taken a dongle\n", timediff, id);
	hub->dongles[id % size].available = false;
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d is compiling\n", timediff, id);
}

void	debug_and_refactor(t_hub *hub, int id)
{
	long long	timediff;

	timediff = gettime_ms(hub->start_time);
	printf("%lld %d is debugging\n", timediff, id);
	usleep(hub->config.time_to_debug * 1000);
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d is refactoring\n", timediff, id);
	usleep(hub->config.time_to_refactor * 1000);
}

void	main_loop(t_hub *hub, int tid)
{
	int		loops;
	bool	grabbed;

	grabbed = false;
	loops = 0;
	while (loops < hub->config.number_of_compiles_required)
	{
		grabbed = false;
		while (grabbed == false)
		{
			pthread_mutex_lock(&hub->d_mutex);
			if (compile_available(hub, tid))
			{
				compile(hub, tid);
				loops++;
				grabbed = true;
			}
			pthread_mutex_unlock(&hub->d_mutex);
		}
		usleep(hub->config.time_to_compile * 1000);
		d_release(hub, tid);
		debug_and_refactor(hub, tid);
	}
}

void	*coder(void *arg)
{
	t_coder_arg	*c_arg;
	t_hub		*hub;
	int			tid;

	c_arg = (t_coder_arg *)arg;
	tid = c_arg->thread_id + 1;
	hub = c_arg->hub;
	main_loop(hub, tid);
	return (NULL);
}
