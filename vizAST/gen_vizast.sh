#!/bin/bash

# usage (add rule astviz in Makefile + inc le hfile + ajouter fct dans code et lui envoyer pointeur sur ast)

CURR_DIR=$(pwd)

if [ ! -f "${CURR_DIR}/config.sh" ]; then
	read -p 'Quel est le path de ton projet ? ' PROJECT_PATH
	read -p 'QUel est le path de ton fichier header depuis la racine de ton projet ? (ex : includes/header.h) ' INCL
	read -p 'Quel argument veux-tu envoyer à ton programme ? ' ARGS
	read -p 'Quel est le nom de la struct qui représente tes noeuds ? ' NODE
	read -p 'Quel est le nom de la variable que tu veux print dans ta struct ? ' CONTENT
	read -p 'Quel est le type de cette variable ? (s/c/d) ' TYPE
	read -p 'Quel est le nom de la variable qui pointe sur ton noeud enfant gauche ? ' LEFT
	read -p 'Quel est le nom de la variable qui pointe sur ton noeud enfant right ? ' RIGHT

	echo -e "PROJECT_PATH=\"${PROJECT_PATH}\"" > config.sh
    echo -e "INCL=\"${INCL}\"" >> config.sh
    echo -e "ARGS=\"${ARGS}\"" >> config.sh
    echo -e "NODE=\"${NODE}\"" >> config.sh
    echo -e "CONTENT=\"${CONTENT}\"" >> config.sh
    echo -e "LEFT=\"${LEFT}\"" >> config.sh
    echo -e "RIGHT=\"${RIGHT}\"" >> config.sh
	echo -e "TYPE=\"${TYPE}\"" >> config.sh
else
	source ./config.sh 2> /dev/null
fi
arrINCL=(${INCL//// })
HFILE="${arrINCL[1]}"
HDIR="${arrINCL[0]}"

sed -i '12s/^/# include "'"$HFILE"'"\n/' vizast.h
sed -i '4s/^/void\tvizAST('"$NODE"' *ast);\n/' fct_vizast.h

gcc -o vizast.o -c vizast.c -Wall -Wextra -Werror -Wno-format -g3 -I. -I$PROJECT_PATH/$HDIR -D NODE=$NODE \
																				-D CONTENT=$CONTENT \
																				-D LEFT=$LEFT \
																				-D RIGHT=$RIGHT \
																				-D TYPE="$TYPE"
ar rcs libvizast.a vizast.o
mv libvizast.a ..
cd $PROJECT_PATH
make vizast
eval ./vizast $ARGS
i=0
for dotfile in ast*.dot; do
	while [ $i -ge 0 ]; do
		stat $CURR_DIR/ast$i.png > /dev/null 2>&1
		ret=$?
		if [ $ret -eq 1 ]; then
			break
		fi
		i=$(( $i + 1 ))
	done
	dot -Tpng -o $CURR_DIR/ast$i.png $dotfile
done
cd $CURR_DIR
sed -i '12d' vizast.h
sed -i '4d' fct_vizast.h
rm vizast.o $PROJECT_PATH/libvizast.a $PROJECT_PATH/vizast 
for dotfile in $PROJECT_PATH/ast*.dot; do
	rm $dotfile
done
