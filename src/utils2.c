/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:52:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/04/25 16:13:06 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	compile_available(t_hub *hub)
{
	int	i;
	int	unused;

	i = 0;
	unused = 0;
	while (i < hub->config.number_of_coders)
	{
		if (hub->dongles[i].available)
			unused++;
		i++;
	}
	if (unused >= 2)
		return (true);
	return (false);
}
