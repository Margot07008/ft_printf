cmake_minimum_required(VERSION 3.14)
project(ft_printf C)

set(CMAKE_C_STANDARD 99)

add_executable(printf_new srcs/ft_printf.c srcs/check.c includes/ft_printf.h srcs/func.c  srcs/ft_itoa.c srcs/treatment.c srcs/treat_f.c srcs/write_func.c mmm.c)