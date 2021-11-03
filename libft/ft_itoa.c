/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/27 21:56:10 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2017/12/01 18:19:51 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		i;
	long	nb;
	char	*res;

	i = n <= 0 ? 1 : 0;
	nb = n < 0 ? -(long)n : (long)n;
	while (nb > 0 && ++i)
		nb /= 10;
	if (!(res = (char*)malloc(sizeof(char) * i + 1)))
		return (0);
	nb = n < 0 ? -(long)n : (long)n;
	res[i] = '\0';
	while (--i >= 0)
	{
		res[i] = (nb % 10) + 48;
		nb /= 10;
	}
	res[0] = n < 0 ? '-' : res[0];
	return (res);
}
