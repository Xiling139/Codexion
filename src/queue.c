/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 12:16:56 by zhewu             #+#    #+#             */
/*   Updated: 2026/10/07 18:23:59 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heapify_up(t_queue *pq, int index)
{
	int	parent;

	parent = (index - 1) / 2;
	if (index != 0 && pq->items[parent].priority > pq->items[index].priority)
	{
		swap(&pq->items[parent], &pq->items[index]);
		heapify_up(pq, parent);
	}
}

void	enqueue(t_queue *pq, t_request request)
{
	if (pq->size == 4)
		return ;
	pq->items[pq->size] = request;
	pq->size++;
	heapify_up(pq, pq->size - 1);
}

void	heapify_down(t_queue *pq, int index)
{
	int	smallest;
	int	left;
	int	right;

	smallest = index;
	left = 2 * index + 1;
	right = 2 * index + 2;
	if (left < pq->size
		&& pq->items[left].priority < pq->items[smallest].priority)
		smallest = left;
	if (right < pq->size
		&& pq->items[right].priority < pq->items[smallest].priority)
		smallest = right;
	if (smallest != index)
	{
		swap(&pq->items[index], &pq->items[smallest]);
		heapify_down(pq, smallest);
	}
}

t_request	dequeue(t_queue *pq)
{
	t_request	null_request;
	t_request	request;

	null_request.priority = 0;
	null_request.tid = -1;
	if (pq->size == 0)
	{
		return (null_request);
	}
	request = pq->items[0];
	pq->size--;
	pq->items[0] = pq->items[pq->size];
	heapify_down(pq, 0);
	return (request);
}

t_request	peek(t_queue *pq)
{
	t_request	null_request;

	null_request.priority = 0;
	null_request.tid = -1;
	if (pq->size == 0)
	{
		return (null_request);
	}
	return (pq->items[0]);
}
