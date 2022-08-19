#include "minishell.h"

int	ft_check_n(char *str)
{
	int	i;

	i = 0;
    if (str && str[i] && str[i] == '-')
    {
        i++;
        while (str[i] && str[i] =='n')
            i++;
        if (i == (int)ft_strlen(str))
            return (1);
        else
            return (0);
    }
    else
        return (0);
}

int    ft_echo(t_node *node)
{
    int i;
    int nl;

    i = 1;
    nl = 0;
    if (node->cmd)
    {
        while (ft_check_n(node->cmd[i]))
        {
            nl = 1;
            i++;
        }
    	while (node->cmd[i])
    	{
	    	ft_putstr_fd(node->cmd[i], node->fd_out);
    		if (node->cmd[i + 1])
			    ft_putchar_fd(' ', node->fd_out);
		    i++;
	    }
        if (nl == 0)
            ft_putchar_fd('\n', node->fd_out);
    }
    return (1);
}