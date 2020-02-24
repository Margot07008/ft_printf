/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:32:12 by afilch            #+#    #+#             */
/*   Updated: 2020/02/24 15:32:15 by afilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int		ft_atoi(const char *str, int allow_neg, int *result, int *i)
{
	int nbr;
	int neg;

	neg = (str[*i] == '-');
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (allow_neg)
			(*i)++;
		else
			return (1);
	}
	nbr = 0;
	while (pf_isdigit(str[*i]))
	{
		nbr = nbr * 10 + (str[*i] - '0');
		(*i)++;
	}
	*result = (neg ? -nbr : nbr);
	return (0);
}

int		len_base(char c, uintmax_t nb, int *base)
{
	int		len;

	len = 0;
	(c == 'x' || c == 'X' || c == 'p') ? *base = 16 : 0;
	(c == 'u' || c == 'd') ? *base = 10 : 0;
	c == 'o' ? *base = 8 : 0;
	c == 'b' ? *base = 2 : 0;
	while (nb)
	{
		nb /= *base;
		++len;
	}
	return (len + (len == 0));
}

void	begin_param(t_param *param, const char *str, int len)
{
	(*param).error = 0;
	(*param).flags = 0;
	(*param).precision = 0;
	(*param).mode = NONE;
	(*param).conversion = 0;
	(*param).str.str = str;
	(*param).str.len = len;
}

void	set_param(va_list arg, t_param *param)
{
	(*param).conversion == 'i' ? (*param).conversion = 'd' : 0;
	(*param).conversion == 'p' ? (*param).mode = LL : 0;
	(*param).width != -2 && (*param).width_flag == 1 ?
			(*param).width_flag = 0 * va_arg(arg, int) : 0;
	(*param).width == -2 ? (*param).width = va_arg(arg, int) : 0;
	if ((*param).width < 0)
	{
		(*param).flags |= FLAG_MINUS;
		(*param).width = -(*param).width;
	}
	(*param).precision == -2 ? (*param).precision = va_arg(arg, int) : 0;
	(*param).precision < 0 && (*param).precision != -1 ?
			(*param).precision = -1 : 0;
}
