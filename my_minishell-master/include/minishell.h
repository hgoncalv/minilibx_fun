/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 10:56:06 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:54:42 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_file	t_file;
typedef struct s_pipe	t_pipe;
typedef struct s_cmd	t_cmd;
typedef enum e_type		t_type;

enum					e_type
{
	NADA,
	R_OUT,
	R_IN,
	R_APP,
	HEREDOC,
};

struct					s_file
{
	char				*filename;
	char				*heredoc;
	int					fd;
	t_type				type;
	t_file				*next;
};

struct					s_cmd
{
	char				**args;
	t_type				type;
	t_file				*file;
	t_file				*file_in;
	t_cmd				*next;
	t_cmd				*prev;
	id_t				flag;
	int					fd_in;
};

typedef struct s_envp
{
	char				**envp;
	t_cmd				*head;
	int					exit_code;
	int					exit;
	int					leave_heredoc;
	bool				valid_input;
	char				***p2matrix;
	char				**p2line;
}						t_envp;

typedef struct s_vars
{
	int					i;
	int					j;
	int					k;
	int					l;
	int					m;
	int					start;
	int					end;
	int					size;
	int					inner_quote;
	int					outer_quote;
	char				*ptr2str;
	char				*ptr2char;
	char				***p2p2p;
	char				*str;
	char				*str2;
	char				*tmp_str;
	char				*tmp_str2;
	char				**matrix;
	char				**matrix2;
	char				**tmp_matrix;
	char				**tmp_matrix2;

}						t_vars;

//builtins.c
int						ft_cd(char **argv);
int						ft_echo(char **argv);
int						ft_pwd(char **argv);
int						ft_export_loop(char **argv);
int						ft_export(char **argv, int index);

//builtins_background.c
char					*ft_setenv_str(char *name, char *value, char *str);
int						set_cd_folder_return_if_free_folder_or_not(char **argv,
							char **ptr2folder);
void					ft_export_no_args(void);
void					unset_loop(char *argv);
int						ft_unset(char **argv);

//builtins_background2.c
int						ft_exit(char **argv);
int						ft_setenv(char *name, char *value, int overwrite);

//builtins_checkers.c
int						check_if_builtin(t_cmd *cmd);
int						check_if_builtin_not_pipe(t_cmd *cmd);
int						check_if_builtin_2pipe(t_cmd *cmd);
int						is_valid_env_var_name(char **name_value);

//builtins_runners.c
int						run_builtin(t_cmd *cmd);

//cmd_build.c
void					expand_cmd_inplace(t_vars *v, char ***p2cmdargs,
							char ***args_expanded);
void					expand_cmd_args(char ***p2cmdargs);
t_cmd					*init_tcmd(char ***p2matrix, int i);
bool					add_cmds(char **matrix);

//cmd_build2.c
void					set_redir(t_cmd *cmd, char ****str);
void					build_cmds_delete_matrix(char **matrix);

//concat.c
char					*ft_concat3(char *s1, char *s2, char *s3);
char					*ft_concat_string_between_chars(char c_start, char *s,
							char c_end);
char					*ft_concat_multi_getsize_n_alloc(char **s, char *glue);
char					*ft_concat_multi(char **s, char *glue);

//create_files.c
void					create_n_clean_file(t_file *file);
void					create_file(char *str, t_cmd *cmd);
void					create_infile(char *str, t_cmd *cmd);

//error.c
void					print_error(char *error_str1, char *error_str2);
void					print_perror(char *error_str1);
void					ft_chk_n_exit_if_null_ppchar(char **matrix,
							char *error_str);
void					ft_chk_n_exit_if_null_pchar(char *str, char *error_str);
void					ft_chk_n_exit_if_null(void *str, char *error_str);

//free.c
int						free_files(t_file *file);
void					free_cmds(t_cmd *head);
void					minishell_clean(char **p2line);

//heredoc.c
void					init_heredoc(void);

//hg_loop.c
int						set_exit_code(t_cmd **curr, char **path, int *i);
int						run_if_first_level_builtins_set_path(t_cmd **curr,
							char **path, int *num_commands, int *i);
void					execute_commands(pid_t pid, t_cmd *curr, char *path);
void					pipe_commands_init(char **path, int **pipes, int *i,
							t_cmd *cmd);
void					pipe_commands(t_cmd *cmd);

//hg_loop_2.c
void					pipe_commands_cleanup(int num_commands, int *pipes,
							char *path);
void					pipe_commands_build_pipes(int *pipes, int num_commands);
void					pipe_commands_dup_n_close_pipes(t_cmd *curr, int *pipes,
							int num_commands, int *i);
int						check_permission_denied(t_cmd **curr, char **path);
int						check_command_not_found(t_cmd **curr, char **path);

//hg_loop_3.c
int						count_cmds(t_cmd *cmd);
int						open_output_file(t_file *file);
void					redirect_output(t_cmd *cmd);
int						open_input_file(t_file *ptr);
void					redirect_input(t_cmd *cmd);

//hg_loop_4.c
void					pipe_get_return_from_child(int status);
void					create_empty_files(t_cmd *cmd);

//initializers.c
void					ft_inicialize_vars(t_vars *vars);
t_cmd					*ft_inicialize_cmd(char **matrix);

//main.c
int						init_remove_qt(void);
void					minishell(void);
int						main(int ac, char **av, char **ev);

//matrix_fts.c
int						ft_matrix_get_num_col(char **matrix);
char					**ft_matrix_dup(char **matrix, int push);
char					**ft_matrix_push(char **matrix, char *str_getsdeleted);
void					ft_matrix_free(char **matrix);

//matrix_utils.c
char					**ft_matrix_remove_col_by_index(char **matrix,
							int index);
void					ft_print_matrix_add_str2line_start(char **matrix,
							char *str, char *glue);

//parse.c
char					*buffer_scan_for_quotes(char *str);
char					**line_to_matrix(char **p2line);

//parse2.c
int						ft_do_quote(char **output);
void					remove_starting_spaces(char **line);
void					remove_ending_spaces(char **line);
void					remove_start_end_spaces(char **line);
int						readline_check(char **p2line);

//parse_clean.c
char					*parse_clean(char **p2str);
char					check_if_str_need_quotes(char *str);
void					check_rowquotes_free_if_needed(char **row);

//parse_env.c
int						ft_env(char **argv);

//path.c
int						ft_get_exec_path_chk_access(char *path, char **env_path,
							char ***matrix);
char					*get_exec_path_all_getenv(t_vars *v, char **env_path);
char					*ft_get_exec_path(char **argv);
char					*ft_getenv(char *name, int trimmed);

//shell.c
t_envp					*_shell(void);
void					init_shell(char **ev);
void					close_shell(void);

//signals.c
void					ft_handler(int signum);
void					signal4main(void);
void					signal_default(void);

//utils2.c
void					ft_swap2str(char **str1, char **str2);

//utils3.c
void					remove_char(char **str, char c);

//utils_array.c
int						is_new_redir(int c);
void					print_tcmd(t_cmd *cmd);
void					free_arrays(char ***p2matrix);
void					print_arrays(char **a);

//utils_fd.c
void					ft_putnbr_fd(int n, int fd);
void					ft_putstr_fd(char *s, int fd);
void					ft_putchar_fd(char c, int fd);

//utils_parsing.c
int						ft_strcount_char(char *str, char l);
int						is_redir_non_pipe(int c);
int						is_redir(int c);
int						r_size(char *s);

//validate_matrix.c
void					validate_matrix(char **matrix);
void					validate_quotes(char **matrix, int *i, int *j, char c);
void					validate_pipe(char **matrix, int *i, int *j);
void					validate_redir(char **matrix, int *i, int *j);

//validation.c
void					ft_validate_pipe_matrix(char **line, int *i);
void					ft_validate_redir_output_matrix(char **line, int *i);
void					ft_validate_redir_input_matrix(char **line, int *i);

//var_expansion.c
int						ft_is_var_from_expansion(char *str, t_vars *vars);
t_vars					ft_exp_helper(t_vars v, char *str);
t_vars					ft_nexp_helper(t_vars v, char *str);
char					*ft_var_expansion(char *str);
void					ft_get_quotes_inner_outer(char c, t_vars *vars);

//var_expansion_2.c
//No functions found
#endif