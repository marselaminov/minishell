/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mryan <mryan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 09:58:34 by mryan             #+#    #+#             */
/*   Updated: 2021/07/17 11:21:55 by mryan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_get_args(t_msh *msh, t_com *cmd, int *i)
{
	int				j;
	int				num;
	t_line_symbol	*line2;

	num = 0;
	line2 = ft_get_clean_line(msh->line);
	if (line2 == NULL)
		return (1);
	free(msh->line);
	msh->line = line2;
	while (msh->line[*i].symb == ' ' && msh->line[*i].flag == 0)
		*i = *i + 1;
	while (num < cmd->num_args)
	{
		j = ft_pass_str(i, msh);
		cmd->args[num] = ft_mshsubstr2(msh, *i, j - *i);
		if (!cmd->args[num])
			return (1);
		*i = j;
		while (msh->line[*i].symb == ' ' && msh->line[*i].flag == 0)
			*i = *i + 1;
		num++;
	}
	return (0);
}

char	*ft_get_str_from_struct(t_line_symbol *line, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	if (!line)
		return (NULL);
	if (line[0].symb == '\0')
		return (ft_strdup(""));
	if (0 > ft_mshstrlen(line))
		return (ft_strdup(""));
	tmp = malloc(len + 1);
	if (!tmp)
		return (NULL);
	while (i < len)
	{
		tmp[j] = line[i].symb;
		i++;
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

t_line_symbol	*ft_mshdup(t_line_symbol *line)
{
	int				i;
	int				len;
	t_line_symbol	*dup_line;

	len = ft_mshstrlen(line);
	if (!line)
		return (NULL);
	dup_line = malloc((len + 1) * sizeof(t_line_symbol *));
	if (!dup_line)
		return (NULL);
	i = 0;
	while (line[i].symb)
	{
		dup_line[i] = line[i];
		i++;
	}
	dup_line[i].symb = '\0';
	return (dup_line);
}

int	ft_update_line(t_line_symbol ***str, t_com *cmd, int num, int its_cmd)
{
	int	n;
	int	i;

	n = 0;
	i = 0;
	while (n < cmd->num_args)
	{
		if (n == num && its_cmd == 1)
		{
			n++;
			continue ;
		}
		(*str)[i] = ft_mshdup(cmd->args[n]);
		if (!(*str)[i])
			return (1);
		free(cmd->args[n]);
		i++;
		n++;
	}
	free(cmd->args);
	cmd->args = *str;
	return (0);
}

int	ft_get_command(t_com *command)
{
	int				args_num;
	int				its_cmd;
	char			*tmp;
	t_line_symbol	**str;

	its_cmd = 0;
	args_num = 0;
	while (args_num < command->num_args)
	{
		tmp = ft_get_line_from_struct(command->args[args_num], 0,
				ft_mshstrlen(command->args[args_num]));
		if (!tmp)
			return (1);
		if (ft_strcmp(tmp, ">>") && ft_strcmp(tmp, "<") && ft_strcmp(tmp, ">")
			&& ft_strcmp(tmp, "<<"))
		{
			command->com = tmp;
			free(command->args[args_num]);
			its_cmd = 1;
			break ;
		}
		free(tmp);
		args_num = args_num + 2;
	}
	if (its_cmd == 1)
		str = malloc(sizeof (t_line_symbol *) * (command->num_args - 1));
	else
		str = malloc(sizeof (t_line_symbol *) * (command->num_args));
	if (!str)
		return (1);
	if (ft_update_line(&str, command, args_num, its_cmd))
		return (1);
	if (its_cmd)
	{
		command->num_args = command->num_args - 1;
		if (command->num_args == 0)
		{
			free(command->args);
			command->args = NULL;
		}
	}
	return (0);
}

int	ft_get_separator(t_msh *msh, t_com *command, int separ, int i)
{
	if (separ == 1)
	{
		if (((msh->line[i].symb == '|') || (msh->line[i].symb == ';'))
			&& msh->line[i].flag == 0)
			command->pipe_in = msh->line[i].symb;
	}
	if (separ == 0)
	{
		if (((msh->line[i].symb == '|') || (msh->line[i].symb == ';'))
			&& msh->line[i].flag == 0)
		{
			command->pipe_out = msh->line[i].symb;
			i++;
		}
	}
	return (i);
}

void	ft_handle_process(t_msh *msh)
{
	t_list	*new_list;
	t_com	*command;
	int		i;

	if (ft_preparser(msh))
		return ;
	i = 0;
	while (msh->line[i].symb != '\0')
	{
		command = malloc(sizeof(t_com));
		if (!command)
			close_prog("malloc error\n");
		new_list = ft_lstnew(command);
		if (!new_list)
			close_prog("malloc error\n");
		command->pipe_out = '-';
		command->pipe_in = '-';
		command->com = NULL;
		command->args = NULL;
		command->args_new = NULL;
		while (msh->line[i].symb == ' ' && msh->line[i].flag == 0)
			i++;
		i = ft_get_separator(msh, command, 0, i);
		while (msh->line[i].symb == ' ' && msh->line[i].flag == 0)
			i++;
		command->num_args = ft_get_num_of_args(&msh->line[i]);
		if (command->num_args == 0)
		{
			free(command);
			free(new_list);
			continue ;
		}
		else if (command->num_args > 0)
		{
			command->args = malloc(command->num_args
					* sizeof (t_line_symbol *));
			if (!command->args)
				close_prog("malloc error\n");
		}
		if (ft_get_args(msh, command, &i))
			close_prog("arguments error\n");
		if (ft_get_command(command))
			close_prog("command not found\n");
		i = ft_get_separator(msh, command, 1, i);
		ft_lstadd_back(&msh->com, new_list);
	}
	ft_command_manage(msh);
}