/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:59:19 by zhenming          #+#    #+#             */
/*   Updated: 2026/06/13 11:51:43 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	empty_argument_error(void)
{
	fprintf(stderr, "Invalid argument.\n");
	fprintf(stderr, "Argument must not be empty.\n");
	return (-1);
}

int	argument_count_error(int argc)
{
	fprintf(stderr, "Invalid argument format.\n");
	fprintf(stderr, "Received %d arguments, but 8 are expected.\n", argc - 1);
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

int	integer_overflow_error(void)
{
	fprintf(stderr, "Invalid argument.\n");
	fprintf(stderr, "Integer exceeds its maximum limit.\n");
	return (-1);
}
