//
// Created by Margot on 13.02.2020.
//

#include <math.h>
#include "../includes/ft_printf.h"

long int multiplication(long double num, int *size)
{
	long int multipl;

	multipl = 1;
	while ((int)num/10 != 0)
	{
		multipl *= 10;
		(*size)++;
		num /= 10;
	}
	return (multipl);
}

void handle_integer(long double *num, char **str, int *i, long int multipl)
{
	int k;
	int j;

	j = 1;
	k = (int)*num;

	while (j-- == 1 || k != 0 )
	{
		(*str)[(*i)] = (char)((*num)/multipl + '0'); //берем первую цифру из целой части
		*num -= (int)((*num)/multipl)*multipl; //вычитаем ее в соответсвии с разрядом
		k -= ((int)k/multipl)*multipl;
		multipl /= 10;
		(*i)++;
	}

}

int need_to_plus(char **str, int only_zero, int precision)
{
	int i;
	int number;

	i = 0;
	if (only_zero == 0)
		return (1);
	else
	{
		while ((*str)[i] != '.')
			i++;
		number = ((*str)[i + precision] - '0');
		if (number % 2 == 0)
			return (0);
		else
			return (1);
	}
}


int handle_decimals(char **str, int *i, long double num, int precision)
{
	int k;
	int tmp_num;
	double ac;
	int only_zero;

	only_zero = 1;
	ac = 5;
	k = 0;
	num *= 10; //появляется цифра в целой части
	while (k++ < precision)
		ac *= 0.1;
	k = 1;

	if (precision <= 0)
		ac = 0.5;
	else
		(*str)[(*i)++] = '.';

	while (k <= 15)
	{
		if ((num >= 0 && k <= precision) || (num < 0 && k <= precision))
		{
			tmp_num = ((int) num != 9) ? (int) (num + ac) : (int) num;
			(*str)[(*i)++] = (char) (tmp_num + '0');
			num = (num - tmp_num) * 10;
		}
		else
		{
			tmp_num = ((int) num != 9) ? (int) (num + ac) : (int) num;
			if (k > precision + 1 && tmp_num > 5 && num > 0)
				return (1);
			else if ((k == precision + 1) && tmp_num < 5 && num > 0)
				return (0);
			if (tmp_num > 0 && k > precision + 1)
			{
				only_zero = 0;
				break;
			}
			num = (num - tmp_num) * 10;
		}
		k++;
	}
//	printf ("only_zero = %d\n", only_zero);
	return (need_to_plus(&(*str), only_zero, precision));

//
//	while (k++ <= precision)
//	{
//		if ((int)num == 0)
//		{
//			tmp_str[(*i)++] = '0';
//			num *= 10;
//			continue ;
//		}
//		tmp_num = ((int)num != 9) ? (int)(num + ac) : (int)num;
//		if (k > precision && tmp_num > 5)
//			return (1);
//		else if (k > precision)
//			return (0);
//		tmp_str[(*i)++] = (char)(tmp_num + '0');
//		num = (num - tmp_num) * 10;
//	}
	//return (0);
}

void rev_str (char **str, int size)
{
	int i;
	char temp;

	i = 0;
	//printf ("bef = %s\n", *str);
//	printf ("rev size %d\n", size);
	while (i < size/2)
	{
		temp = (*str)[i];

		(*str)[i] = (*str)[size - i - 1];

		(*str)[size - i - 1] = temp;
		i++;
	}

}

void plus_one(char** str, int *size, int neg)
{
	char *tmp_a = malloc(sizeof(char)*((*size)+1));
	int i = (*size) - 1;
	int carry = 0;
	int j = 0;
	int temp = 0;
	int flag = 1;

	while (i >= 0 || carry > 0)
	{
		if ((*str)[i] == '.')
			tmp_a[j] = '.';
		else
		{
			if ((*str)[i] - '0' >= 0 && (*str)[i] - '0' <= 9)
				temp = ((*str)[i] - '0') + flag + carry;
			else
				temp = flag + carry;
			flag = 0;
			carry = temp / 10;
			tmp_a[j] = (temp % 10) + '0';
		}
		j++;
		i--;
	}
	(*size) = j;
	neg == 1 ? tmp_a[j-1] = '-' : 0;
	tmp_a[j] = '\0';
	//*str = tmp_a;
//	printf("tmp_a %s\n", tmp_a);
//	printf("size %d\n", size);
	rev_str(&tmp_a, (*size));
	*str = tmp_a;
}

int float_to_str(char **str, long double num, int precision, int neg)
{
	int size;
	long int multipl;
	char *tmp_str;
	int i;

	i = 0;
	size = 1;
	if (num < 0 || (1/num == -INFINITY))
	{
		size++;
		neg = 1;
		num = -num;
	}
	multipl = multiplication(num, &size); // считаем сколько степеней 10ки в целой части
	//printf("mult = %ld\n", multipl); // +
	size += precision + 1;
	if (!(tmp_str = malloc(sizeof(char)*(size))))
		return(0);
	//printf("size = %d\n", size); // длина строки -99.99 (6)
	if (neg)
		tmp_str[i++] = '-';
	handle_integer(&num, &tmp_str, &i, multipl); // записываем в tmp_str целую часть num с точкой, остается 0.(дробная часть)
	//tmp_str[size] = '\0';
	//printf("tmp_str = %-5s %Lf\n", tmp_str, num); // num имеет вид 0.9999
	if (handle_decimals(&tmp_str, &i, num, precision))
	{
		tmp_str[i] = '\0';
		plus_one(&tmp_str, &size, neg);
	}
	else
		tmp_str[i] = '\0';
	*str = tmp_str;
	//printf("%s\n", *str);
	//printf("size = %d\n", size);
	//precision == 0 ? (*str)[size--] = '\0' : 0;
	return (size);
}

int treat_f_float(t_param param, va_list arg)
{
	char *str;
	int size;
	int neg;

	if (param.precision < 0)
		param.precision = 6;
	neg = 0;
	size = float_to_str(&str, va_arg(arg,double), param.precision, neg);
	printf("%s\n", str);
	return (size);
}