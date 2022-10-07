#include "minishell.h"

char	*ft_strdup_two(char *var, char *pwd)
{
	char	*full;
	int	i;
	int	j;

	if (!var || !pwd)
		return (NULL);
	full = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(pwd) + 2));
	if (!full)
		return (NULL);
	i = 0;
	while (var[i])
	{
		full[i] = var[i];
		i++;
	}
	full[i++] = '=';
	j = 0;
	while (pwd[j])
	{
		full[i + j] = pwd[j];
		j++;
	}
	full[i + j] = '\0';
	return (full);
}

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

int		ft_update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	t_env	*env;
	t_env	*tmp;
	t_env	*new;

	env = singleton_env(1, NULL, NULL);
	tmp = env;
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (0);
	while (tmp)
	{
		if (tmp->var && ft_strcmp("OLDPWD", tmp->var))
		{
			oldpwd = tmp->value;
			tmp->value = ft_strdup(cwd);
			free(oldpwd);
			oldpwd = tmp->full;
			tmp->full = ft_strdup_two(new->var, new->value);
			free(oldpwd);
			free(cwd);
			ft_env_sort();
			return (1);
		}
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	if (new)
	{
		new->var = ft_strdup("OLDPWD");
		new->value = ft_strdup(cwd);
		new->full = ft_strdup_two(new->var, new->value);
		new->pos = 0;
		new->prev = NULL;
		new->next = NULL;
		free(cwd);
		add_back_env(&env, new);
		ft_env_sort();
		return (1);
	}
	return (0);
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

int				ft_cd(t_node *node)
{
    int		cd_ret;
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
    if (node && node->cmd[1] && node->cmd[2])
	{
		display_error(ERR_CD_NBARG, node->cmd[1]);
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