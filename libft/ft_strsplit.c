/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit2.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/27 14:28:30 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2018/11/04 19:28:20 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static int		ft_count_words(const char *s, char c)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (s[i] != c)
		j++;
	while (s[i])
	{
		if (s[i] != c && s[i - 1] == c)
			j++;
		i++;
	}
	return (j);
}

static int		ft_count_letters(const char *s, char c, int i)
{
	int pos;

	pos = 0;
	while (s[i] != c && s[i])
	{
		pos++;
		i++;
	}
	return (pos);
}

char			**ft_strsplit(const char *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	if (s == 0)
		return (NULL);
	i = -1;
	j = ft_count_words(s, c);
	k = 0;
	if (!(tab = (char**)malloc(sizeof(tab) * (j + 1))))
		return (NULL);
	while (s[++i])
		if (s[i] != c && s[i])
		{
			j = ft_count_letters(s, c, i);
			tab[k++] = ft_strsub(s, i, j);
			i += j - 1;
		}
	tab[k] = NULL;
	return (tab);
}
