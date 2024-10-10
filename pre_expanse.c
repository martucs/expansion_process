/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expanse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:50:25 by martalop          #+#    #+#             */
/*   Updated: 2024/10/10 13:48:38 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "struct.h"
#include "execution.h"
#include "expansion.h"
#include <stdio.h>

int		count_sections(char *s)
{
	int	i;
	int	sect;

	i = 0;
	sect = 0;
	while (s && s[i])
	{
		sect++;
		if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] == '\'')
				i++;	
		}
		else if (s[i] == '\"')
		{
			i++;
			while (s[i] && s[i] != '\"')
				i++;
			if (s[i] == '\"')
				i++;	
		}
		else
		{
			while (s[i] && s[i] != '\'' && s[i] != '\"') // al salir de aqui será o '\0' o comillas
				i++;
		}
	}
	return (sect);
}

char	**break_down_str(char *s)
{
	int		i;
	int		x;
	int		sect;
	int		ctr;
	char	**arr;

	i = 0;
	x = 0;
	ctr = 0;
	sect = count_sections(s);
	arr = malloc(sizeof(char *) * (sect + 1));
	if (!arr)
		return (NULL);
	// averiguar medida de cada segmento y poner en cada string del split a devolver
	while (s && s[i])
	{
		ctr = 0;
		if (s[i] == '\'')
		{
			i++;
			ctr++;
			while (s[i] && s[i] != '\'')
			{
				i++;
				ctr++;
			}
			if (s[i] == '\'')
			{
				i++;		 
				ctr++;
			}
		}
		else if (s[i] == '\"')
		{
			i++;
			ctr++;
			while (s[i] && s[i] != '\"')
			{
				i++;
				ctr++;
			}
			if (s[i] == '\"')
			{
				i++;
				ctr++;
			}	
		}
		else
		{
			while (s[i] && s[i] != '\'' && s[i] != '\"')
			{
				i++;
				ctr++;
			}
		}
		arr[x] = ft_substr(s, (i - ctr), ctr);
		if (!arr[x])
		{
			free_array(arr);
			return (NULL);
		}
		x++;
	}
	arr[x] = NULL;
	return (arr);
}

/*int	main(void)
{
	char	*str;
	char	**arr;
	int		i;

	i = 0;
//	str = "\"$USER\".hola\"$\'US\"ER\'$USjas ghdER\'\'\'kajshd";
	str = "\"hola\"bla";
	printf("input: %s\n", str);
	arr = break_down_str(str);
	if (!arr)
		printf("malloc err\n");
	while (arr && arr[i])
	{
		printf("arr[%d]: %s\n", i, arr[i]);
		i++;
	}
	if (!arr[i])
		printf("%s\n", arr[i]);
	free_array(arr);
	return (0);
}*/

int	count_divisions(char *str)
{
	int	i;
	int	div;

	i = 0;
	div = 0;
	if (!ft_strchr(str, '$'))
		return (1);
	while (str && str[i])
	{
	//	if (str[i] == '\'' || str[i] == '\"')
	//			i++;
		if (str[i] && str[i] != '$')
		{
			div++;
			while (str[i] != '$' && str[i])
				i++;
		}
		if (str[i] && str[i++] == '$') // me salto el '$'
		{
			div++;
			while (ft_isalnum(str[i]) || str[i] == '_')
				i++;
		}
	}
	return (div);
}

int	find_quote_type(char *str)
{
	if (str[0] == '\'')
		return (1);
	else if (str[0] == '\"')
		return (2);
	else
		return (0);
}

/*int	main(void)
{
	char	*str;
	int		res;

	str = "blaUSER_$holaUSER.tt\'";
	printf("input: %s\n", str);
	res = find_quote_type(str);
	if (!res)
		printf("no comillas\n");
	else if (res == 1)
		printf("comillas simples\n");
	else
		printf("comillas dobles\n");
	return (0);
}*/

char	**divide_str(char *str)
{
	int		i;
	int		x;
	int		divs;
	int		ctr;
	char	**div_arr;

	i = 0;
	x = 0;
	divs = count_divisions(str);
	div_arr = malloc(sizeof(char *) * (divs + 1));
	if (!div_arr)
		return (NULL);
	while (str && str[i])
	{
		ctr = 0;
//		if ((i == 0 || i == (ft_strlen(str) - 1)) && (str[i] == '\'' || str[i] == '\"'))
//				i++;
		if (str[i] && str[i] != '$')
		{
			while (str[i] != '$' && str[i])
			{
				i++;
				ctr++;
			}
//			if (!str[i] && (str[i - 1] == '\'' || str[i - 1] == '\"'))
//			{
//				i--;
//				ctr--;
//			}
		//	printf("i = %d, ctr = %d\n", i, ctr);
			div_arr[x] = ft_substr(str, i - ctr, ctr);
			if (!div_arr[x])
				return (free_array(div_arr), NULL);
			x++;
		}
		ctr = 0;
		if (str[i] && str[i++] == '$')
		{
			ctr++;
			if (str[i] != '?')
			{
				while (ft_isalnum(str[i]) || str[i] == '_')
				{
					i++;
					ctr++;
				}
			}
			else
			{
				i++;
				ctr++;
			}
			div_arr[x] = ft_substr(str, i - ctr, ctr);
			if (!div_arr[x])
				return (free_array(div_arr), NULL);
			x++;
		}
	}
	div_arr[x] = NULL;
	return (div_arr);
}

/*int	main(void)
{
	char	*str;
	char	**arr;
	int		i;

	i = 0;
	str = "\"$hola chao\"";
	arr = NULL;
	printf("input: %s\n", str);
	printf("res = %d\n", count_divisions(str));
	arr = divide_str(str);
	if (!arr)
		printf("malloc err\n");
	while (arr && arr[i])
	{
		printf("arr[%d]: %s\n", i, arr[i]);
		i++;
	}
	if (!arr[i])
		printf("%s\n", arr[i]);
//	free_array(arr);
	return (0);
}*/
