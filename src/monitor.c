/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/04/25 16:10:18 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int *init_int_arr(t_hub *hub) {
	int *arr;
	int size = hub->config.number_of_coders;
	arr = (int *)malloc(sizeof(int) *  size);
	if (!arr)
		return (NULL);
	
	int i = 0;
	while (i < size) {
		arr[i] = hub->config.time_to_burnout;
		i++;
	}
	return arr;
}

void	*monitor_run(void *arg)
{
	t_hub	*hub;
	long long time;

	hub = (t_hub *)arg;
	hub->burnout_time = init_int_arr(hub);
	while (true) {
		time = gettime_ms(hub->start_time);
		if (time >= hub->burnout_time[i])
	} 
	return (NULL);
}
