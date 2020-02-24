/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjuice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 17:24:42 by bjuice            #+#    #+#             */
/*   Updated: 2020/02/23 17:25:28 by bjuice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_NEW_FT_PRINTF_H
# define PRINTF_NEW_FT_PRINTF_H

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>
# include <limits.h>
# include <math.h>

# define NONE 0
# define FLAG_HASH 1
# define FLAG_ZERO 2
# define FLAG_MINUS 4
# define FLAG_PLUS 8
# define FLAG_SPACE 16
# define FLAG_APOSTROPHE 32

typedef struct	s_for_float
{
	char **str;
	char sign;
	int precision;
	int count;
	int size;
	int multipl;
	int i;
	int num_size;
	int j;
	int neg;
	int flag;
}							t_float;

typedef struct	s_string
{
	const char	*str;
	int		len;
}				t_string;

typedef enum	e_modifier
{
	NO = 0, L = 1, H = 2, J = 3, Z = 4, LL = 5, HH = 6
}				t_modifier;

typedef enum	e_colour
{
	WHITE = 0, GREEN = 1, BLUE = 2, RED = 3, PURPLE = 4,
	GREY = 5, YELLOW = 6, AQUA = 7
}				t_colour;

typedef struct	s_param
{
	t_string	str;
	t_modifier	mode;
	t_colour	colour;
	char		conversion;
	char		flags;
	int			width;
	int			width_flag;
	int			precision;
	int			error;
}						t_param;

typedef struct	s_for_pr
{
	int count;
	int i;
	int j;
	int k;
}				t_for_pr;

typedef struct	s_treat
{
	char			conversion;
	int				(*treat)(t_param, va_list);
}				t_treat_conv;

typedef struct s_for_itoa
{
	char *str;
	char sign;
	char flag_fill;
	int flag;
	int base;
	int length;
	int count;
	int i;
	int j;
	int k;
	t_colour	colour;
	uintmax_t buf;
	uintmax_t nb;
} t_for_itoa;

int ft_printf(const char *format, ...);

int		ft_atoi(const char *str, int allow_neg, int *result, int *i);
int		pf_is_conversion(char c);
int		pf_is_modifier(char c);
int		pf_is_flag(char c);
int		pf_is_precision(char c);
int		pf_is_valid(char c);
int		pf_isdigit(int c);
int		ft_itoa(t_param param, intmax_t nb, char sign);
int		ft_uitoa(t_param param, uintmax_t nb);
int		repeat_write(char c, int n);
int		pf_write(char *str, int len, int flag, t_colour colour);
int		ft_strlen(const char *s);

int treat_conversion(t_param *param, const char *str, int *i);
int treat_flags(t_param *param, const char *str, int *i);
int treat_modifier(t_param *param, const char *str, int *i);
int treat_precision(t_param *param, const char *str, int *i);
int treat_colour(t_param *param, const char *str, int *i);
int	treat_width(t_param *param, const char *str, int *i);

int treat_f_number(t_param param, va_list arg);
int treat_f_char(t_param param, va_list arg);
int treat_f_string(t_param param, va_list arg);
int treat_f_percent(t_param param, va_list arg);
int treat_f_float(t_param param, va_list arg);


//!!!!!!!!!
int		len_base(char c, uintmax_t nb, int *base);
void	ft_itoa_2(t_for_itoa a);
void	ft_itoa_1(t_for_itoa a);
void	ft_uitoa_1(t_for_itoa a, t_param param);
void	ft_uitoa_2(t_for_itoa a, t_param param);
void	set_param(va_list arg, t_param *param);
void	begin_param(t_param *param, const char *str, int len);


#endif
