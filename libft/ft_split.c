/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_split.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/18 14:12:22 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2018/11/01 01:06:19 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int		ft_count_words(char *s)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (s[i] != 32 && s[i] != 9 && s[i] != 10 && s[i] != 11)
		j++;
	while (s[i])
	{
		if (s[i] == 32 || s[i] == 9 || s[i] == 10 || s[i] == 11)
		{
			i++;
			if (s[i] && s[i] != 32 && s[i] != 9 && s[i] != 10 && s[i] != 11)
				j++;
			i--;
		}
		i++;
	}
	return (j);
}

int		ft_count_letters(char *s, int pos)
{
	int i;

	i = 0;
	while (s[pos] && s[pos] != 32 && s[pos] != 9 && s[pos] != 10 &&
			s[pos] != 11)
	{
		pos++;
		i++;
	}
	return (i);
}

char	*ft_strsub(char *s, int pos, int len)
{
	char	*ret;
	int		i;

	i = 0;
	if (!(ret = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i != len)
	{
		ret[i] = s[pos];
		i++;
		pos++;
	}
	ret[i] = '\0';
	return (ret);
}

char	**ft_split(char *str)
{
	int		i;
	int		j;
	int		k;
	char	**ret;

	i = ft_count_words(str);
	k = 0;
	if (!(ret = malloc(sizeof(char*) * (i + 1))))
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] != 32 && str[i] != 9 && str[i] != 10 && str[i] != 11)
		{
			j = ft_count_letters(str, i);
			ret[k++] = ft_strsub(str, i, j);
			i += j - 1;
		}
		i++;
	}
	ret[k] = NULL;
	return (ret);
}
