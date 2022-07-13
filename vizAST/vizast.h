#ifndef __VIZAST__H
# define __VIZAST_H__

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

# define AST		"ast"
# define DOT		".dot"
# define HEADER 	"digraph {\n\tnode [shape=oval, fontsize=12, fontname=\"Courier\", height=.1];\n\tranksep=.3;\n\tedge [arrowsize=.5]\n"

#endif
