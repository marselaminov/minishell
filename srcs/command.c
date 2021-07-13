#include "../include/minishell.h"

char	*ft_struct_to_str(t_line_symbol *struc, int start, int len)
{
    int		i;
    int		j;
    char	*str;

    if (!struc)
        return (NULL);
	if (struc[0].c == '\0')
		return (ft_strdup(""));
	if (start > ft_mshstrlen(struc))
		return (ft_strdup(""));
    str = malloc(len + 1);
    if (!str)
        return (NULL);
    i = start;
    j = 0;
    while (i < (len + start))
    {
        str[j] = struc[i].c;
        i++;
        j++;
    }
    str[j] = '\0';
    return (str);
}

int	ft_args_new(t_com *com)
{
    int	i;

    if (com->num_args > 0)
    {
        com->args_new = calloc(com->num_args, sizeof(char *));
        if (!com->args_new)
            return (0);
    }
    i = 0;
    while (i < com->num_args)
    {
        com->args_new[i] = ft_struct_to_str(com->args[i], 0, ft_mshstrlen(com->args[i]));
        free(com->args[i]);
		com->args[i] = NULL;
        i++;
    }
    if (com->num_args > 0)
    	com->args_new[i] = NULL;
    free(com->args);
    com->args = NULL;
    return (1);
}

void    ft_exec_all(t_msh *msh, t_com *com)
{
    if (ft_redir_checker(com))
        ft_redir_mng(com, msh);
    else
    {
        if (!ft_args_new(com))
            close_prog(msh, "error memory\n");
        ft_builtin(msh, com);
    }
}

void    ft_lauch_pipe(t_msh *msh, t_com *com, pid_t *pidpipe)
{
    if (com->pipe_in == '|')
    {
        ft_pipe_out(msh, com);
        msh->numwaits_pipe++;
        if (msh->pipe_read_fd)
            close(msh->pipe_read_fd);
        msh->pipe_read_fd = msh->pipe_fd[0];
        close(msh->pipe_fd[1]);
    }
    else if (com->pipe_out == '|')
    {
        ft_pipe_in(msh, com, pidpipe);
        close(msh->pipe_read_fd);
        msh->pipe_read_fd = 0;
    }
}

void	ft_command_manage(t_msh *msh)
{
    t_list	*list;
    pid_t   pidpipe;

    list = msh->com;
    while (list)
    {
        pidpipe = 0;
        if (((t_com *)list->content)->pipe_out != '|'
            && ((t_com *)list->content)->pipe_in != '|')
            ft_exec_all(msh, (t_com *)list->content);
        ft_lauch_pipe(msh, (t_com *)list->content, &pidpipe);
        ft_check_wait(msh, (t_com *)list->content, &pidpipe);
        list = list->next;
    }
}