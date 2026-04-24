/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhenming <zhewu@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:59:19 by zhenming          #+#    #+#             */
/*   Updated: 2026/04/24 17:09:21 by zhenming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	argument_count_error(int argc)
{
	fprintf(stderr, "Invalid argument format.\n");
	fprintf(stderr, "Received %d arguments, but 8 are required.\n", argc - 1);
	return (-1);
}

int	argument_type_error(int arg_num)
{
	if (arg_num < 8)
	{
		fprintf(stderr, "Invalid argument type.\n");
		fprintf(stderr, "Time or number must be a non-negative integer.\n");
	}
	if (arg_num == 8)
	{
		fprintf(stderr, "Invalid argument type.\n");
		fprintf(stderr, "Scheduler must be either 'FIFO' or 'EDF'.\n");
	}
	return (-1);
}
