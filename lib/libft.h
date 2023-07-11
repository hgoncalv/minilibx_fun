/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:03:37 by vrubio            #+#    #+#             */
/*   Updated: 2023/01/04 21:24:46 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 2

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

//ft_atoi.c
int					ft_atoi(const char *s);

//ft_bzero.c
void				ft_bzero(void *str, size_t l);

//ft_c_counter.c
int					count_words(char *str, char c);

//ft_calloc.c
void				*ft_calloc(size_t count, size_t size);

//ft_d_counter.c
int					ft_d_counter(int n);

//ft_is_n_o_a.c
int					ft_is_n_or_a(int c);

//ft_isalnum.c
int					ft_isalnum(int c);

//ft_isalpha.c
int					ft_isalpha(int c);

//ft_isascii.c
int					ft_isascii(int c);

//ft_isdigit.c
int					ft_isdigit(int c);

//ft_isint.c
int					ft_isint(char *nbr);

int					ft_isnbr(char *c);

//ft_isprint.c
int					ft_isprint(int c);

//ft_isspace.c
int					ft_isspace(char c);

//ft_isupper.c
int					ft_isupper(int c);

//ft_itoa.c
char				*ft_itoa(int n);

//ft_memccpy.c
void				*ft_memccpy(void *dest, const void *src, int c, size_t l);

//ft_memchr.c
void				*ft_memchr(const void *str, int c, size_t n);

//ft_memcmp.c
int					ft_memcmp(const void *str1, const void *str2, size_t n);

//ft_memcpy.c
void				*ft_memcpy(void *dest, const void *src, size_t n);

//ft_memmove.c
void				*ft_memmove(void *dest, const void *src, size_t n);

//ft_memset.c
void				*ft_memset(void *str, char c, size_t l);

//ft_putchar.c
void				ft_putchar(char c);

//ft_putstr.c
void				ft_putstr(char const *s);

//ft_realloc.c
void				*ft_realloc(void *ptr, size_t size);

//ft_str_offset.c
char				*ft_str_offset(char *str, int i);

//ft_stradd.c
void				ft_stradd(char **str, char *add);

//ft_strchr.c
char				*ft_strchr(const char *s, int c);

//ft_strclr.c
void				ft_strclr(char *s);

//ft_strcpy.c
char				*ft_strcpy(char *dst, const char *src);

//ft_strdup.c
char				*ft_strdup(char *str);

//ft_strexact.c
bool				ft_strexact(char *s1, char *s2);

//ft_strexist.c
int					ft_strexist(char *s, char c);

//ft_strfree.c
void				ft_strfree(char **str);

//ft_strjoin.c
char				*ft_strjoin(char *s1, char *s2);

//ft_strlen.c
int					ft_strlen(char *str);

//ft_strncmp.c
int					ft_strncmp(const char *str1, const char *str2, size_t n);

//ft_strnew.c
char				*ft_strnew(size_t size);

//ft_strnstr.c
char				*ft_strnstr(const char *str, const char *find, size_t len);

//ft_strsplit.c
char				**ft_strsplit(char const *str, char c);

//ft_substr.c
char				*ft_substr(char *str, int begg, int l);

//ft_tolower.c
int					ft_tolower(int c);

//ft_toupper.c
int					ft_toupper(int c);

//ft_validnl.c
int					has_newline(char *str);

//ft_wrdcnt.c
int					ft_wrdcnt(const char *str, char c);

//gnl.c
int					ft_gnl(int fd, char **line);
#endif
