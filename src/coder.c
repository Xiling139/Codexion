/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/11 18:30:57 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_hub *hub, int id)
{
	long long	timediff;
	
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d has taken a dongle\n", timediff, id);
	hub->free_dongles--;
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d has released a dongle\n", timediff, id);
	hub->free_dongles++;
	timediff = gettime_ms(hub->start_time);
	printf("%lld %d is compiling\n", timediff, id);
	usleep(hub->config.time_to_compile * 1000);
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
	int	loops;

	loops = 0;
	while (loops < hub->config.number_of_compiles_required)
	{
		pthread_mutex_lock(&hub->d_mutex);
		while(hub->free_dongles < 2)
			pthread_cond_wait(&hub->cv, &hub->d_mutex);
		compile(hub, tid);
		pthread_mutex_unlock(&hub->d_mutex);
		debug_and_refactor(hub, tid);
		loops++;
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
	printf("Thread ID: %d initialized.\n", tid);
	main_loop(hub, tid);
	return (NULL);
}
