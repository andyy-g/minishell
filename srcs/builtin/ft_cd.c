#include "minishell.h"

void		print_error(t_node *node)
{
	ft_putstr_fd("cd: ", 2);
	if (node->cmd[2])
		ft_putstr_fd("string not in pwd: ", 2);
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(args[1], 2);
}

char		*get_env_path(t_env *env, const char *var, size_t len)
{
	char	*oldpwd;
	int		i;
	int		j;
	int		s_alloc;

	while (env && env->next != NULL)
	{
		if (ft_strncmp(env->value, var, len) == 0)
		{
			s_alloc = ft_strlen(env->value) - len;
			if (!(oldpwd = malloc(sizeof(char) * s_alloc + 1)))
				return (NULL);
			i = 0;
			j = 0;
			while (env->value[i++])
			{
				if (i > (int)len)
					oldpwd[j++] = env->value[i];
			}
			oldpwd[j] = '\0';
			return (oldpwd);
		}
		env = env->next;
	}
	return (NULL);
}

int		update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (ERROR);
	if (!(oldpwd = ft_strjoin("OLDPWD=", cwd)))
		return (ERROR);
	if (is_in_env(env, oldpwd) == 0)
		env_add(oldpwd, env);
	ft_memdel(oldpwd);
	return (SUCCESS);
}

int		go_to_path(int option, t_env *env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	if (option == 0)
	{
		update_oldpwd(env);
		env_path = get_env_path(env, "HOME", 4);
		if (!env_path)
        {
			printf("minishell : cd: HOME not set");
			return (0);
        }
	}
	else if (option == 1)
	{
		env_path = get_env_path(env, "OLDPWD", 6);
		if (!env_path)
        {
			printf("minishell : cd: OLDPWD not set");
			return (0);
        }
		update_oldpwd(env);
	}
	ret = chdir(env_path);
	//ft_memdel(env_path);
	return (ret);
}

int				ft_cd(t_node *node, t_env *env)
{
    int		cd_ret;

    if (node->cmd[1] && node->cmd[2])
	{
		printf("minishell: cd: too many arguments\n");
		g_exit_status = 1;
		return (1);
	}
    else
    {
        if (!(node->cmd[1]))
		    return (go_to_path(0, env));
    	if (ft_strcmp(node->cmd[1], "-") == 0)
	    	cd_ret = go_to_path(1, env);
    	else
	    {
		    update_oldpwd(env);
	    	cd_ret = chdir(node->cmd[1]);
		    if (cd_ret < 0)
			    cd_ret *= -1;
	    	if (cd_ret != 0)
		    	print_error(node);
    	}
    }
	return (cd_ret);
}