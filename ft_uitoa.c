/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:26:05 by afilch            #+#    #+#             */
/*   Updated: 2020/02/24 15:26:14 by afilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../includes/ft_printf.h"

void	call_uitoa(t_param param, t_for_itoa a)
{
	if (param.flags & FLAG_MINUS)
		ft_uitoa_1(a, param);
	else
		ft_uitoa_2(a, param);
}

int		help_uitoa_a(t_param param)
{
	if ((param.flags & FLAG_HASH || param.conversion == 'p')
		&& (param.conversion == 'x' || param.conversion == 'X'
		|| param.conversion == 'p'))
		return (2);
	else if (param.flags & FLAG_HASH || param.conversion == 'p')
		return (1);
	return (0);
}

char	help_uitoa_flagfill(t_param param)
{
	if (param.flags & FLAG_ZERO && param.precision == -1)
		return ('0');
	else
		return (' ');
}

int		help_uitoa_j(t_param param, t_for_itoa a)
{
	if (param.precision != -1)
		return (param.precision - a.count);
	else
		return (0);
}

int		ft_uitoa(t_param param, uintmax_t nb)
{
	t_for_itoa	a;

	a.flag = 0;
	a.i = 0;
	a.buf = nb;
	a.colour = param.colour;
	a.flag = help_uitoa_a(param);
	a.flag_fill = help_uitoa_flagfill(param);
	a.count = len_base(param.conversion, nb, &a.base);
	!nb && param.precision != -1 && a.flag != 1 ? a.count = 0 : 0;
	a.length = a.count;
	!nb && param.conversion != 'p' && a.flag &&
	(param.precision > a.count || param.width > a.count) ? a.flag = 0 : 0;
	param.precision > a.count && param.conversion == 'o' ? a.flag = 0 : 0;
	a.length < param.precision ? a.length = param.precision : 0;
	((a.flag && nb) || (a.flag && param.conversion == 'p'))
	? a.length += a.flag : 0;
	a.length < param.width ? a.length = param.width : 0;
	if (!(a.str = (char *)malloc(a.length)))
		return (-1);
	a.k = a.length;
	a.j = help_uitoa_j(param, a);
	a.j < 0 ? a.j = 0 : 0;
	call_uitoa(param, a);
	return (a.k);
}
