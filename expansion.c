/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:58:48 by martalop          #+#    #+#             */
/*   Updated: 2024/10/10 15:52:09 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "struct.h"
#include "expansion.h"
#include "libft/libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char	*search_in_envp(char *str, t_envp *envp, int prev_ex_stat)
{
	char	*new_value;
	char	*aux_key;

//	printf("input: %s\n", str);
	while (envp)
	{
		aux_key = ft_substr(envp->key, 0, (ft_strlen(envp->key) - 1));
		if (!ft_strcmp(str, aux_key))
		{
			new_value = ft_strdup(envp->value);
			free(aux_key);
			aux_key = NULL;
			return (new_value);
		}
		free(aux_key);
		aux_key = NULL;
		envp = envp->next;

	}
	if (str[0] == '?' && !str[1])
		return (ft_itoa(prev_ex_stat)); //deberia protegerla
	return (NULL);
}

char	*actual_expansion(char *str, t_envp *envp, int quote_type, int prev_ex_stat)
{
	char	*new_value;
	char	*res;

	res = NULL;
	new_value = NULL;
	if (!quote_type || quote_type == 2) 
	{
		if (str[0] == '$' && !str[1])
			return (ft_strdup("$"));
		new_value = search_in_envp(str + 1, envp, prev_ex_stat); 
		if (!new_value)
		{
			// la variable NO existe, asi que dejo todo lo que habia en str, incluido el '$'
			// es ambiguous redirect!!
			res = ft_strdup("");
			return (res);
		}
		return (new_value);
	}
	res = ft_strdup(str); // no existe la variable o no tengo que expandir(prq venia de comillas simples)
						  	// asi que devuelvo lo mismo que me ha llegado
	return (res);
}

char	**full_expansion(char *str, t_envp *envp, int prev_ex_stat)
{
	char 	**arr;
	char 	**divided_arr;
	char 	**final_arr;
	char 	*tmp_expanded;
	char 	*tmp_str;
	int		i;
	int		j;
	int		quote_type;
	int		x;
	
	i = 0;
	j = 0;
	quote_type = 0;
	tmp_str = NULL;
	final_arr = NULL;
	tmp_expanded = NULL;
	arr = break_down_str(str); // en sin comillas, con dobles o con simples
	if (!arr)
		return (NULL);
//	print_char_arr(arr);
	while (arr && arr[i])
	{
		x = 0;
		quote_type = find_quote_type(arr[i]);
//		fprintf(stderr, "arr[%d]: ", i);
//		write(2, arr[i], ft_strlen(arr[i]));
//		write(2, "\n", 1);
		divided_arr = divide_str(arr[i]); // en partes expandibles y no expandibles
		while (x < find_arr_size(divided_arr) && divided_arr && divided_arr[x])
		{
//			fprintf(stderr, "divided_arr[%d] before expansion: ", x);
//			write(2, divided_arr[x], ft_strlen(divided_arr[x]));
//			write(2, "\n", 1);
			if (ft_strchr(divided_arr[x], '$'))
			{
//				printf("encuentro $\n");
				tmp_expanded = actual_expansion(divided_arr[x], envp, quote_type, prev_ex_stat);
				if (!tmp_expanded)
					return (write(2, "malloc error\n", 13), NULL);
			}
			else
				tmp_expanded = ft_strdup(divided_arr[x]);
			free(divided_arr[x]);
			divided_arr[x] = tmp_expanded;
//			fprintf(stderr, "divided_arr[%d] after expansion: ", x);
//			write(2, divided_arr[x], ft_strlen(divided_arr[x]));
//			write(2, "\n", 1);
		//	fprintf(stderr, "address of empty string: %p\n", divided_arr[i]);
//			write(2, "\n", 1);
			x++;
		}
		tmp_str = join_back_expanded(divided_arr); // de div_arr (segmentos expandibles y no expandibles)
		free_array(divided_arr);
		free(arr[i]);
		if (!tmp_str)
			return (NULL);
		arr[i] = tmp_str;
		i++;
	}
	final_arr = join_all(arr); // de arr (comillas simpl/dobl y sin)
	if (!final_arr)
		return (NULL);
	free_array(arr);
	return (final_arr);
}

void	handle_expanded_redir(char **aux, t_redir *tmp_red)
{
	if (!aux[0] || aux[1])
		tmp_red->amb_red = 1;
	else
	{
		free(tmp_red->file_name);
		tmp_red->file_name = ft_strdup(aux[0]);
	}
	free_array(aux);
}
int	expand_files(t_redir *redirs, t_envp *envp, int prev_ex_stat)
{
	t_redir	*tmp_red;
	char	**aux;
	int		i;


	i = 0;
	tmp_red = redirs;
	while (tmp_red)
	{
		tmp_red->file_name = ft_strdup("\"$test'\"hol\"$bla\"");
		printf("\ntmp_red->file_name: %s\n", tmp_red->file_name);
		aux = NULL;
		aux = full_expansion(tmp_red->file_name, envp, prev_ex_stat);
		if (!aux)
		{
			printf("expansion returns NULL\n");
			return (1);
		}
		while (aux && aux[i])
		{
			printf("FULL EXPANSION RES: arr[%d]: %s\n", i, aux[i]);
			i++;
		}
		if (!aux[i])
			printf("FULL EXPANSION RES: arr[%d]: %p\n", i, aux[i]);
		printf("\n");
		handle_expanded_redir(aux, tmp_red); 
		tmp_red = tmp_red->next;
	}
	return (0);
}

int	expand_cmds(t_cmd *cmd, t_envp *envp, int prev_ex_stat)
{
	t_cmd	*tmp_cmd;
	char	**aux;
	char	**aux2;
	int		i;
	int		x;

	x = 0;
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		aux = NULL;
		while (tmp_cmd->arr_cmd && tmp_cmd->arr_cmd[x])
		{
			i = 0;
			aux = full_expansion(tmp_cmd->arr_cmd[x], envp, prev_ex_stat);
	//		aux2 = split_dup(aux);
	//		good_arr = add_array(aux, aux2);
			
			while (aux && aux[i])
			{
				printf("FULL EXPANSION RES: arr[%d]: %s\n", i, aux[i]);
				i++;
			}
			if (!aux[i])
				printf("FULL EXPANSION RES: arr[%d]: %p\n", i, aux[i]);
			printf("\n");
		//	free_array(tmp_cmd->arr_cmd);
		//	tmp_cmd->arr_cmd = aux;
			x++;
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (0);
}
