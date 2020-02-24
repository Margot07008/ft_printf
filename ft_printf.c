/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 16:55:55 by afilch            #+#    #+#             */
/*   Updated: 2020/02/24 16:55:57 by afilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int		transform_param_conv(t_param param, va_list arg)
{
	static t_treat_conv	(treat_array[12]) = {
			{.conversion = 'd', .treat = treat_f_number},
			{.conversion = 'o', .treat = treat_f_number},
			{.conversion = 'u', .treat = treat_f_number},
			{.conversion = 'x', .treat = treat_f_number},
			{.conversion = 'b', .treat = treat_f_number},
			{.conversion = 'r', .treat = treat_f_string},
			{.conversion = 'X', .treat = treat_f_number},
			{.conversion = 'c', .treat = treat_f_char},
			{.conversion = 's', .treat = treat_f_string},
			{.conversion = 'p', .treat = treat_f_number},
			{.conversion = 'f', .treat = treat_f_float},
			{.conversion = '%', .treat = treat_f_percent},
	};
	int					i;

	i = -1;
	if (param.error)
		return (0);
	while (++i < 12)
		if (param.conversion == treat_array[i].conversion)
			return (treat_array[i].treat(param, arg));
	return (0);
}

int		begin_conv(va_list arg, const char *str, int *i, int len)
{
	t_param		param;
	int			j;
	int			start;
	static int	(*tra[6]) (t_param *param, const char *str, int *i) = {
			treat_colour, treat_flags, treat_width,
			treat_precision, treat_modifier, treat_conversion};

	j = 0;
	start = *i;
	begin_param(&param, str, len);
	while (j < 6 && *i <= start + len)
	{
		if (tra[j](&param, str, i))
		{
			param.error = 1;
			break ;
		}
		++j;
	}
	if (param.conversion == 0)
		return (-1);
	set_param(arg, &param);
	return (transform_param_conv(param, arg));
}

void	pf_is_colour(const char *str, int *i)
{
	int	j;

	j = *i + 1;
	while (str[j] && str[j] != '}')
		++j;
	*i = ++j;
}

void	format_parser(const char *str, va_list arg, t_for_pr *b)
{
	while (str[b->i])
	{
		if (str[b->i] == '%' && (b->j = ++b->i))
		{
			str[b->j] == '{' ? pf_is_colour(str, &b->i) : 0;
			while ((str[b->i] && pf_is_valid(str[b->i])
			&& !pf_is_conversion(str[b->i]))
			|| (str[b->i] && str[b->i] != '%' && pf_is_conversion(str[b->i])))
				++b->i;
			str[b->i] == '%' ? ++b->i : 0;
			if ((b->k = begin_conv(arg, str, &b->j, b->i - b->j)) == -1)
				break ;
			b->count += b->k;
			b->i = b->j;
		}
		else
		{
			b->k = b->i;
			while (str[b->i] && str[b->i] != '%')
				++b->i;
			b->count += b->i - b->k;
			write(1, str + b->k, b->i - b->k);
		}
	}
}

int		ft_printf(const char *format, ...)
{
	va_list		arg;
	t_for_pr	b;

	b.count = 0;
	b.i = 0;
	va_start(arg, format);
	format_parser(format, arg, &b);
	va_end(arg);
	return (b.count);
}
