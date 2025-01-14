/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpassin <tpassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:22:28 by luctan            #+#    #+#             */
/*   Updated: 2024/10/12 20:01:18 by tpassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>

# define GET_HEREDOC	"/tmp/heredoc"

extern int				g_var;

typedef enum s_token_type
{
	WORD,
	PIPE,
	REDIRECTION,
}						t_token_type;

typedef enum s_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	HERE_DOC,
	APPEND,
}						t_redir_type;

typedef struct s_token
{
	char				*str;
	int					nb_quotes;
	t_token_type		type;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_redir
{
	char				*file;
	char				*heredoc_name;
	t_redir_type		type;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	pid_t				pid;
	char				*command;
	char				**arguments;
	t_redir				*redirection;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char				*key;
	char				*value;
	int					equal;
	struct s_env		*next;
}						t_env;

typedef struct s_data
{
	int			fd[2];
	int			heredoc;
	int			prev;
	int			exit_status;
	int			count;
	int			save;
	int			in_heredoc;
	char		cquote;
	char		*new;
	char		*value;
	char		**path;
	char		*s2;
	char		*prompt;
	t_token		*head;
	t_env		*get_env;
	t_command	*cmd;
}				t_data;

int			check_input(char *str);
void		ft_signal(void);
t_env		*init_env(char **envp, int ac);
void		init_data(t_data *data);
int			is_space(char c);
int			inquotes(char c, int i);
void		ft_exit(char *str, t_data *data);
void		free_tab(char **str);
long		ft_atol(char **nb);
t_token		*tokenizer(t_data *data, char *input);
void		add_token(t_token **token_head, t_token_type type,
				char *str, int nb_quotes);
void		free_array(char *str);
void		char_redir(char *str, int *redir_out, int *redir);
int			redirection_input(char *s, int in, int out);
t_env		*copy_env(char **envp);
t_env		*lst_env(t_env *env);
void		ft_clean(t_data *data);
int			lst_size(t_env *lst);
void		free_env(t_env *get_env);
char		**env_to_tab(t_data *data);
t_token		*last_node(t_token *token);
int			wordinquote(char c, t_data *data);
char		**ft_join_tab(char **oldtab, char *str);
void		command_addback(t_command **command, t_command *new);
void		clean_all(t_data *data);
t_command	*parsing(t_data *data);
char		*ft_expand(t_data *data, char *str, int heredoc,
				int nb_quotes);
void		clean_cmd(t_command *cmd);
int			is_quotes(int c);
void		process_string(char *str, t_data *data, int *pos,
				int *i);
void		loop_len(char *str, t_data *data, int *len, int *i);
char		*get_value(char *str, int *i, t_data *data);
int			wquote(char c, int *i, t_data *data);
void		redirection_addback(t_redir **redir, t_redir *new);
int			ft_exec(t_command *cmd, t_data *data);
void		ft_executor(t_command *cmd, t_data *data, char **env,
				int i);
void		free_node(t_token *head);
void		clean_cmd(t_command *cmd);
char		*get_varenv(char *str, t_data *data, int TYPE);
void		free_redir(t_redir *redirection);
void		fork_redir_free(t_data *data, char **env, char **path);
void		fork_clean(t_data *data, char **envp);
void		child_signals(void);
void		ft_exit_code(int code, t_data *data, t_command *cmd, char **envp);
char		*get_cmd(t_data *data, char *command);
void		free_exec(char **env, char **path);
void		init_exec(t_data *data, char ***env, char ***path);
char		**find_path(t_data *data);
int			nb_cmd(t_command *cmd);
void		sig_heredoc(t_data *data);
int			run_heredoc(t_command *cmd, t_data *data);
#endif
