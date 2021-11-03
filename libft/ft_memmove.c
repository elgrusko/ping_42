/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memmove.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/27 21:42:57 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2017/11/29 12:27:14 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *tmp_src;
	unsigned char *tmp_dest;

	tmp_src = (unsigned char*)src;
	tmp_dest = (unsigned char*)dest;
	if (dest <= src && src <= dest + n + 1)
		while (n--)
			*tmp_dest++ = *tmp_src++;
	else
	{
		tmp_dest += n - 1;
		tmp_src += n - 1;
		while (n--)
			*tmp_dest-- = *tmp_src--;
	}
	return (dest);
}
