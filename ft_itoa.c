/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:48:28 by afilch            #+#    #+#             */
/*   Updated: 2020/02/24 14:48:38 by afilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../includes/ft_printf.h"

void		help_itoa(t_param param, t_for_itoa a)
{
	if (param.flags & FLAG_MINUS)
		ft_itoa_1(a);
	else
		ft_itoa_2(a);
}

intmax_t	help_itoa_nb(intmax_t nb)
{
	if (nb < 0)
		return (-nb);
	else
		return (nb);
}

char		help_itoa_flagfill(t_param param)
{
	if (param.flags & FLAG_ZERO && param.precision == -1)
		return ('0');
	else
		return (' ');
}

int			help_itoa_j(t_param param, t_for_itoa a)
{
	if (param.precision != -1)
		return (param.precision - a.count);
	else
		return (0);
}

int			ft_itoa(t_param param, intmax_t nb, char sign_flag)
{
	t_for_itoa a;

	a.buf = help_itoa_nb(nb);
	a.count = len_base(param.conversion, a.buf, &a.base);
	nb == 0 && !param.precision ? a.count = 0 : 0;
	a.i = 0;
	a.colour = param.colour;
	a.flag_fill = help_itoa_flagfill(param);
	a.length = a.count;
	a.length < param.precision ? a.length = param.precision : 0;
	nb < 0 || sign_flag == '+' ? a.length++ : 0;
	if (nb < 0)
		a.sign = '-';
	else
		a.sign = sign_flag;
	if (param.width <= a.length && sign_flag == ' ' && nb >= 0)
		a.length++;
	a.length < param.width ? a.length = param.width : 0;
	if (!(a.str = (char *)malloc(a.length)))
		return (-1);
	a.k = a.length;
	a.j = help_itoa_j(param, a);
	a.j < 0 ? a.j = 0 : 0;
	help_itoa(param, a);
	return (a.k);
}
