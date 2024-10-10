#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include "struct.h"

int		expand_files(t_redir *redirs, t_envp *envp, int prev_ex_stat);
int		expand_cmds(t_cmd *cmd, t_envp *envp, int prev_ex_stat);
char	*join_back_expanded(char **divided_arr);
char	**break_down_str(char *s);
int		find_quote_type(char *str);
char	**divide_str(char *str);
char	**join_all(char **arr);

#endif
