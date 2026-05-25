/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:53:51 by zhenming          #+#    #+#             */
/*   Updated: 2026/05/25 16:24:53 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

bool	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
		{
			if (!(str[i] == '-' && ft_isdigit(str[i + 1]) != 0))
				return (false);
		}
		i++;
	}
	return (true);
}

char	*str_to_upper(char *str)
{
	int		i;
	int		len;
	char	*new_str;

	len = strlen(str);
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			new_str[i] = str[i] - 'a' + 'A';
		else
			new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	*init_int_arr(t_config config)
{
	int	*arr;
	int	size;
	int	i;

	size = config.number_of_coders;
	arr = (int *)malloc(sizeof(int) * size);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		arr[i] = config.time_to_burnout;
		i++;
	}
	return (arr);
}
