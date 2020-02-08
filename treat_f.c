#include <math.h>
#include "../includes/ft_printf.h"

int treat_f_number_d(t_param param, va_list arg) {
	int count;
	char sign;
	intmax_t n;
	char *str;
	
	sign = 0;
	count = 0;
	if (param.mode == L)
		n = va_arg(arg, long);
	else if (param.mode == LL)
		n = va_arg(arg, long long);
	else if (param.mode == J)
		n = va_arg(arg, intmax_t);
	else if (param.mode == Z)
		n = va_arg(arg, ssize_t); // уточнить не size_t случаем!!!!!!!!!!!!!!!!!!!
	else
		n = va_arg(arg, int);
	if (param.mode == HH)
		n = (char) n;
	else if (param.mode == H)
		n = (short) n;
	param.flags & FLAG_PLUS ? sign = '+' : 0;
	param.flags & FLAG_SPACE ? sign = ' ' : 0;
	if (!param.precision && !n)
		return (0);
	str = ft_itoa(param, n, sign, &count);
	if (param.width > count && str)
	{
		if (param.flags & FLAG_MINUS)
			return(pf_write(str,count, 1) + repeat_write(' ', param.width - count));
		return(repeat_write(' ', param.width - count)+ pf_write(str, count, 1));
	}
	return(pf_write(str, count, 1));
}

int treat_f_number(t_param param, va_list arg)
{
	int count;
	int flag_clear;
	uintmax_t n;
	char *str;
	
	flag_clear = 1;
	if (param.conversion == 'd')
		return(treat_f_number_d(param, arg));
	if (param.mode == L)
		n = va_arg(arg, unsigned long);
	else if (param.mode == LL)
		n = va_arg(arg, unsigned long long);
	else if (param.mode == J)
		n = va_arg(arg, uintmax_t);
	else if (param.mode == Z)
		n = va_arg(arg, ssize_t);
	else
		n = va_arg(arg, unsigned int);
	if (param.mode == HH)
		n = (unsigned char) n;
	else if (param.mode == H)
		n = (unsigned short) n;
	if (!param.precision && !n)//+ x  и n==0 при conv = o
		return (0);
	if (param.conversion == 'p' && !n)
	{
		str = "(nil)";
		count = 5;
		flag_clear = 0;
	}
	else
		str = ft_uitoa(param, n, &count);
	if (param.width > count && str)
	{
		if (param.flags & FLAG_MINUS)
			return(pf_write(str,count, flag_clear) + repeat_write(' ', param.width - count));
		return(repeat_write(' ', param.width - count)+ pf_write(str, count, flag_clear));
	}
	return(pf_write(str, count, flag_clear));
}

int treat_f_char(t_param param, va_list arg)
{
	char a;
	char c;
	
	a = param.flags & FLAG_ZERO ? '0' : ' ';
	c = (char)va_arg(arg, int);
	if ((param.flags & FLAG_MINUS) == 0)
		return(repeat_write(a, param.width - 1) + pf_write(&c,1,0));
	return(pf_write(&c,1,0) + repeat_write(a, param.width - 1));
}

int treat_f_string(t_param param, va_list arg)
{
	char *str;
	int i;
	int length;
	
	i = -1;
	str = va_arg(arg, char *);
	length = ft_strlen(str);
	if (!str)
	{
		str = "(null)";
		length = 6;
	}
	param.precision == -1 ? param.precision = INT_MAX : 0;
	param.precision > length ? param.precision = length : 0;
	if ((param.flags & FLAG_MINUS) == 0 && param.width > param.precision)
		return(repeat_write(' ', param.width - param.precision) + pf_write(str,param.precision,0));
	if (param.width > param.precision)
		return(pf_write(str,param.precision,0) + repeat_write(' ', param.width - param.precision));
	return (pf_write(str, param.precision,0));
}

int treat_f_percent(t_param param, va_list arg)
{
	char a;
	
	a = param.flags & FLAG_ZERO ? '0' : ' ';
	if ((param.flags & FLAG_MINUS) == 0)
		return(repeat_write(a, param.width - 1) + pf_write("%",1,0)); //  В оригинальном принтф получить сдвиг процента не получилось, но реализация все равное такая, т.к. вывод совпал с чуваком с гита + хер знает, зачем такую
	return(pf_write("%",1,0) + repeat_write(a, param.width - 1));//парашу выводить, но мб кто-то додумался и до таких тестов
}


double multiplication(double num, int *size)
{
	double multipl;

	multipl = 1;
	while ((int)num/10 != 0)
	{
		multipl *= 10;
		(*size)++;
		num /= 10;
	}
	return (multipl);
}

void handle_integer(double *num, char **str, int *i, double multipl)
{
	char *tmp_str;

	tmp_str = *str;
	while ((int)(*num) != 0)
	{
		tmp_str[(*i)] = (char)((*num)/multipl + '0'); //берем первую цифру из целой части
		*num -= (int)((*num)/multipl)*multipl; //вычитаем ее в соответсвии с разрядом
		multipl /= 10;
		(*i)++;
	}
}


int handle_decimals(char **str, int *i, double num, int precision)
{
	char *tmp_str;
	int k;
	int tmp_num;
	int flag;
	double ac = 0.5;

	k = 0;
	num *= 10; //появляется цифра в целой части
	while (k++ < precision)
		ac *= 0.1;
	tmp_str = *str;
	k = 0;

	if (precision != 0)
		tmp_str[(*i)++] = '.';
	else

	while (k++ <= precision)
	{
		if ((int)num == 0)
		{
			tmp_str[(*i)++] = '0';
			num *= 10;
			continue ;
		}
		tmp_num = ((int)num != 9) ? (int)(num + ac) : (int)num;
		if (k > precision && tmp_num > 5)
			return (1);
		else if (k > precision)
			return (0);
		tmp_str[(*i)++] = (char)(tmp_num + '0');
		num = (num - tmp_num) * 10;
	}
	return (0);
}

void rev_str (char **str, int size)
{
	int i;
	char temp;

	i = 0;
	while (i < size/2)
	{
		temp = (*str)[i];
		(*str)[i] = (*str)[size - i -1];
		(*str)[size - i -1] = temp;
		i++;
	}
}

void plus_one(char** str, int size)
{
	char *tmp_a = malloc(sizeof(char)*(size+2));
	int i = size;
	int carry = 0;
	int j = 0;
	int temp = 0;
	int flag = 1;

	// вот здесь проблема с точкой

	while (i >= 0)
	{
		if ((*str)[i] == '.')
			tmp_a[j] = '.';
		else {
			temp = ((*str)[i] - '0') + flag + carry; //а точнее тут, выход за пределы массива получается....
			flag = 0;
			carry = temp / 10;
			tmp_a[j] = (temp % 10) + '0';
		}
		j++;
		i--;
	}
	tmp_a[j] = '\0';
	*str = tmp_a;
	rev_str(&(*str), size+1);
}

int float_to_str(char **str, double num, int precision, int neg)
{
	int size;
	double multipl;
	char *tmp_str;
	int i;

	i = 0;
	size = 1;
	if (num < 0)
	{
		size++;
		neg = 1;
		num = -num;
	}
	multipl = multiplication(num, &size); // считаем сколько степеней 10ки в целой части
	size += precision;
	if (!(tmp_str = malloc(sizeof(char)*(size + 1)))) // +1 для точки
		return(0);
	if (neg)
		tmp_str[i++] = '-';
	handle_integer(&num, &tmp_str, &i, multipl); // записываем в tmp_str целую часть num, остается 0.(дробная часть)
	if (handle_decimals(&tmp_str, &i, num, precision))
	{
		tmp_str[i] = '\0';
		plus_one(&tmp_str, size);
	}
	else
		tmp_str[i] = '\0';
	*str = tmp_str;

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
	size = float_to_str(&str, va_arg(arg, double), param.precision, neg);


	if (param.width >= size && param.flags == FLAG_PLUS)
		return(repeat_write(' ', param.width-(size+2)) + pf_write("+", 1, 0) + pf_write(str, size+1, 0));
	else if (param.width >= size && param.flags == FLAG_SPACE && param.width != (size + 1))
		return (repeat_write(' ', param.width - (size + 1)) + pf_write(str, size + 1, 0));
	else if (param.width >= size && param.flags == FLAG_SPACE && param.width == (size + 1))
		return (repeat_write(' ', 1) + pf_write(str, size + 1, 0));
	else if (param.width >= size)
		return (repeat_write(' ', param.width - (size + 1))+pf_write(str, size + 1, 0));
	else if (param.width <= param.precision && param.flags == FLAG_PLUS)
		return(pf_write("+", 1, 0) + pf_write(str, size+1, 0));
	else if ((param.width <= param.precision && param.flags == FLAG_SPACE) )
		return (repeat_write(' ', 1) + pf_write(str, size + 1, 0));
	else if (param.width <= param.precision)
		return (pf_write(str, size + 1, 0));
	else
		return(0);

}