/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strncpy.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/27 21:47:16 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2018/06/06 20:02:16 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	int		i;
	size_t	j;

	i = 0;
	j = 0;
	while (src[i] && j < n)
	{
		dest[i] = src[i];
		i++;
		j++;
	}
	while (j < n)
	{
		dest[i] = '\0';
		i++;
		j++;
	}
	return (dest);
}
