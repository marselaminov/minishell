#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include "fcntl.h"
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# define MINISHELL_LOOP 1

typedef struct      s_env
{
    char            *key;
    char            *val;
}                   t_env;

typedef struct		s_line_symbol
{
	char			c;
	int 			flag;
}					t_line_symbol;

typedef struct      s_com
{
    int 			num_args;
    int 			num_redir;
	char            *com;
	char            **line_of_args;
	char 			separ;
	char 			separ2;
	t_line_symbol   **args;
}                   t_com;

typedef struct		s_msh
{
    t_list		    *env;
    t_list		    *com;
    t_line_symbol	*line;
	char			*str; // my input string
	char            **env_args;
	char 			*val_in_dlr;   // in this field environment after $
	int 			return_code;
	char 			token; // (; | > < >>) это для одной из внутренних функций , для дальнейшего не нужно , для удобства положил в структуру
	int 			fd_0;
	int 			fd_1;
}					t_msh;

t_line_symbol 	*ft_mshsubstr(t_msh *msh, size_t len);
int				ft_mshstrlen(t_line_symbol *line);
t_line_symbol	*ft_mshstrjoin(t_line_symbol *line1, t_line_symbol *line2);
void 	close_prog(t_msh *msh, char *err);
void	ft_clear_oldpwd(t_msh *msh);
void    ft_environment(t_msh *msh, char **env);
int     ft_get_quote_flag(t_msh *msh, const int *i, int q_flag);
int 	ft_check_open_quote(int q_flag);
int		ft_line_size(t_msh *msh);
int	ft_get_symbol_flag_utils(t_msh *msh, const int *i, int qte);
int		ft_get_symbol_flag(t_msh *msh, int *len, int *qte, int *dlr);
t_line_symbol	*ft_get_struct_line(t_msh *msh, int mlc_len);
int					ft_strcmp(const char *s1, const char *s2);
char 	*ft_get_env(t_msh *msh, char *val);
int 	ft_get_val_in_dlr(t_msh *msh, t_line_symbol *line);
int 	ft_notenv(t_msh *msh, int i, int j);
t_line_symbol	*ft_get_new_line(t_msh *msh);
int 	ft_env(t_msh *msh, int i, int j);
int 	ft_check(t_msh *msh, int begin_str, int end_str);
int		ft_get_dollar(t_msh *msh);
int     ft_preparser(t_msh *msh);
void     ft_parser(t_msh *msh);
int		ft_check_line_syntax(t_msh *msh);
int		ft_check_token_error(t_msh *msh);
int		ft_get_token(t_msh *msh, int *len);
int		ft_check_token3(t_msh *msh, int *len);
int		ft_check_token2(t_msh *msh, int *len);
int 	ft_check_token1(t_msh *msh, int *len);

# endif