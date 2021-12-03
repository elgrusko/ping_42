/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strjoin.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/27 21:46:31 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2017/12/22 15:06:13 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	i;
	int		j;
	char	*tab;

	if (s1 == NULL || s2 == NULL)
		return (0);
	i = ft_strlen(s1) + ft_strlen(s2);
	if (!(tab = (char*)malloc(sizeof(tab) * i)))
		return (0);
	i = -1;
	j = -1;
	if (s1 && s2)
	{
		while (s1[++i])
			tab[i] = s1[i];
		while (s2[++j])
		{
			tab[i] = s2[j];
			i++;
		}
	}
	tab[i] = '\0';
	return (tab);
}
