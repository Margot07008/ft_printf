/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa1_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:30:12 by afilch            #+#    #+#             */
/*   Updated: 2020/02/24 15:30:15 by afilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../includes/ft_printf.h"

void	ft_itoa_1(t_for_itoa a)
{
	if (a.sign)
	{
		a.str[a.i++] = a.sign;
		a.j++;
	}
	while (a.i < a.j)
		a.str[a.i++] = '0';
	a.j = a.i;
	a.i--;
	while (a.count)
	{
		a.str[a.i + a.count--] = a.buf % 10 + '0';
		a.buf /= 10;
		++a.j;
	}
	while (a.j < a.length)
		a.str[a.j++] = ' ';
	pf_write(a.str, a.k, 0, a.colour);
	free(a.str);
}

void	ft_itoa_2(t_for_itoa a)
{
	while (a.count--)
	{
		a.str[--a.length] = a.buf % 10 + '0';
		a.buf /= 10;
	}
	while (a.i < a.length - a.j)
		a.str[a.i++] = a.flag_fill;
	if (a.sign && a.flag_fill != ' ')
		a.str[0] = a.sign;
	else if (a.sign)
		(a.str[a.i - 1] = a.sign);
	while (a.i < a.length)
		a.str[a.i++] = '0';
	pf_write(a.str, a.k, 0, a.colour);
	free(a.str);
}

void	ft_uitoa_1(t_for_itoa a, t_param param)
{
	const char	hex16[16] = "0123456789abcdef";
	const char	arr16[16] = "0123456789ABCDEF";

	if (((a.flag == 2 && a.buf) ||
		(a.flag && param.conversion == 'p')) && ++a.i >= 0)
		a.str[1] = (param.conversion == 'x'
					|| param.conversion == 'p') ? 'x' : 'X';
	a.flag && ++a.i >= 0 ? a.str[0] = '0' : 0;
	while (a.i < a.j + a.flag)
		a.str[a.i++] = '0';
	a.j = a.i--;
	while (a.count)
	{
		if (param.conversion == 'x' || param.conversion == 'p')
			a.str[a.i + a.count--] = hex16[a.buf % a.base];
		else
			a.str[a.i + a.count--] = arr16[a.buf % a.base];
		a.buf /= a.base;
		++a.j;
	}
	while (a.j < a.length)
		a.str[a.j++] = ' ';
	pf_write(a.str, a.k, 0, a.colour);
	free(a.str);
}

int		help_uitoa_2(t_for_itoa a, t_param param)
{
	if (param.width > a.count && param.precision == -1
		&& param.flags & FLAG_ZERO && a.flag == 2)
		return (a.flag - 1);
	else if (param.precision != -1)
		return (a.length - a.count - 1 - a.j);
	else
		return (a.length - a.count - 1);
}

void	ft_uitoa_2(t_for_itoa a, t_param param)
{
	const char	hex16[16] = "0123456789abcdef";
	const char	arr16[16] = "0123456789ABCDEF";
	int			l;

	a.nb = a.buf;
	l = help_uitoa_2(a, param);
	while (a.count--)
	{
		if (param.conversion == 'x' || param.conversion == 'p')
			a.str[--a.length] = hex16[a.buf % a.base];
		else
			a.str[--a.length] = arr16[a.buf % a.base];
		a.buf /= a.base;
	}
	while (a.i < a.length - a.j)
		a.str[a.i++] = a.flag_fill;
	while (a.i < a.length)
		a.str[a.i++] = '0';
	if ((a.flag == 2 && a.nb) || (a.flag && param.conversion == 'p'))
		a.str[l--] = (param.conversion == 'x' ||
				param.conversion == 'p') ? 'x' : 'X';
	a.flag ? a.str[l] = '0' : 0;
	pf_write(a.str, a.k, 0, a.colour);
	free(a.str);
}
