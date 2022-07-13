#include "vizast.h"

int	size_num(int i)
{
	int	size;

	size = 1;
	while (i > 9)
	{
		i /= 10;
		size++;
	}
	return (size);
}

char	*itoa(int n)
{
	char	*num;

	num = malloc(sizeof(char) * (size_num(n) + 1));
	sprintf(num, "%d", n);
	return (num);
}

char	*concat_filename(int i)
{
	char	*filename;
	char	*num;
	int		size;

	size = strlen(AST) + strlen(DOT) + size_num(i);
	filename = malloc(sizeof(char) * (size + 1));
	num = itoa(i);
	strcpy(filename, AST);
	strcat(filename, num);
	strcat(filename, DOT);
	free(num);
	return (filename);
}

char	*get_filename(void)
{
	int			i;
	char		*filename;
	struct stat	buff;

	i = 0;
	while (!stat(filename = concat_filename(i), &buff))
	{
		free(filename);
		i++;
	}
	return (filename);
}

void	write_label(NODE *node, int fd)
{
	char *type = STR(TYPE);

	if (!strcmp(type, "c"))
		dprintf(fd, "[label=\"%c\"]\n", node->CONTENT);
	if (!strcmp(type, "s"))
		dprintf(fd, "[label=\"%s\"]\n", node->CONTENT);
	if (!strcmp(type, "d"))
		dprintf(fd, "[label=\"%d\"]\n", node->CONTENT);
}

void	create_node(NODE *ast, int fd, int *i, int root)
{
	int left;
	int	right;

	left = *i + 1;
	right = *i + 2;
	if (*i == 1)
	{
		dprintf(fd, "\t%d ", root);
		write_label(ast, fd);
	}
	if (ast->LEFT)
	{
		dprintf(fd, "\t%d ", left);
		write_label(ast->LEFT, fd);
		dprintf(fd, "\t%d -> %d\n", root, left);	
	}
	if (ast->RIGHT)
	{
		dprintf(fd, "\t%d ", right);
		write_label(ast->RIGHT, fd);
		dprintf(fd, "\t%d -> %d\n", root, right);	
	}
	if (ast->LEFT)
	{
		*i = right;
		create_node(ast->LEFT, fd, i, left);
	}
	if (ast->RIGHT)
		create_node(ast->right, fd, i, right);
}

void	vizAST(NODE *ast)
{
	int		fd;
	int		i;
	char	*filename;

	filename = get_filename();
	if ((fd = creat(filename, S_IRWXU)) == -1)
		perror("creat");
	free(filename);
	if (write(fd, HEADER, strlen(HEADER)) == -1)
		perror("write");
	i = 1;
	create_node(ast, fd, &i, 1);
	if (write(fd, "}", 1) == -1)
		perror("write");
}
