/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:30:32 by martalop          #+#    #+#             */
/*   Updated: 2024/10/10 23:00:01 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "struct.h"
#include "execution.h"
#include "expansion.h"

int	mult_child(t_cmd *cmd, t_info *info, t_exec *exec_info)
{
	expand_files(cmd->redirs, info->envp, info->prev_ex_stat);
	if (open_redir(cmd) == 1) 
	{
		free_child(info, cmd, exec_info);
		exit(1);
	}
	if (redirect(cmd) == 1)
	{
		free_child(info, cmd, exec_info);
		exit(1);
	}
	close(exec_info->pipe_end[0]); // cierro el fd de lectura de la pipe ACTUAL
	if (!cmd->arr_cmd)
	{
		free_child(info, cmd, exec_info);
		exit(0);
	}
	if (!find_cmd_type(cmd->arr_cmd[0]))
		return (exec_builtin(cmd->arr_cmd, info, cmd, exec_info));
	if (execve(cmd->path, cmd->arr_cmd, exec_info->env) == -1)
	{
		cmd_not_found(cmd->arr_cmd[0]);
		free_child(info, cmd, exec_info);
		exit(127);
	}
	return (0);
}

void	simp_child_cmd(t_cmd *cmd, t_info *info, t_exec *exec_info)
{
	expand_files(cmd->redirs, info->envp, info->prev_ex_stat);
	if (open_redir(cmd) == 1)
	{
	//	print_redirs_lst(cmd->redirs);
		free_child(info, cmd, exec_info);
		exit(1);
	}
	if (!cmd->arr_cmd)
	{
		free_child(info, cmd, exec_info);
		exit(0);
	}
	if (redirect(cmd) == 1)
	{
		free_child(info, cmd, exec_info);
		exit(1);
	}
	if (execve(cmd->path, cmd->arr_cmd, exec_info->env) == -1)
	{
		cmd_not_found(cmd->arr_cmd[0]);
		free_child(info, cmd, exec_info);
		exit(127);
	}
}

void	free_child(t_info *info, t_cmd *cmds, t_exec *exec_info)
{
	free_cmds(cmds);
	free_envlst(info->envp);
	free_exec_info(exec_info);
	clear_history();
	free(info->rl);
}

int	heredoc(char *lim)
{
	char	*str;
	int		pipe_here[2];

	str = NULL;
	if (pipe(pipe_here) == -1)
		return (write(2, "pipe failed\n", 12), -1);
	while (1)
	{
		str = readline("> ");
		if (!ft_strncmp(str, lim, ft_strlen(lim) + 1))
		{
			free(str);
			close(pipe_here[1]);
			break ;
		}
		//expand = expand_heredoc(str, envp);
		//while (expand[i])
		//{
		//	write(pipe_here[1], expand[i], ft_strlen(expand[i]));
		//	i++;
		//}
		write(pipe_here[1], str, ft_strlen(str));
		write(pipe_here[1], "\n", 1);
		free(str);
	}
	return (pipe_here[0]);
}
