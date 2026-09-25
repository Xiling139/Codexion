/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 16:47:23 by zhewu             #+#    #+#             */
/*   Updated: 2026/09/25 14:33:06 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

bool	has_request(t_queue *pq, int tid)
{
	int	i;

	i = 0;
	while (i < pq->size)
	{
		if (pq->items[i].tid == tid)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

void	queue_init(t_queue *pq)
{
	pq->items = malloc(sizeof(t_request) * 2);
	pq->size = 0;
}

void	queue_free(t_queue *pq)
{
	free(pq->items);
}
