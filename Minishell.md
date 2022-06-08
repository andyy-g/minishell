tags: #42school #pedagogy #42meta #projectresources
date: 02/05/2022
***

# Minishell
Développez un shell.

## Définition des termes
### CLI et GUI
Il existe deux moyens principaux pour intéragir avec un ordinateur :
* l'interface graphique, ou graphical user interface (GUI) en anglais,
* et l'interface en ligne de commande, ou command line interface (CLI) en anglais.

Toutes les actions simples effectuées à l'aide d'une interface graphique peuvent être effectuées à l'aide d'une ligne de commande, et inversement. Par exemple, pour créer un dossier depuis l'interface graphique, il vous faut faire un clic droit, et cliquer sur Nouveau dossier dans le menu contextuel qui s'affiche. De façon équivalente vous pouvez utiliser la commande `mkdir foldername` dans votre **terminal**.
Certaines tâches complexes ne sont cependant possible que depuis l'interface en ligne de commande.

### Terminal et console
A l'origine les terminaux et les consoles étaient des périphériques informatiques qui permettaient de communiquer avec un ordinateur central ou un réseau d'ordinateurs.

![terminal](https://upload.wikimedia.org/wikipedia/commons/6/6f/Terminal-dec-vt100.jpg)

Dans le monde logiciel et plus spécifiquement sous UNIX, un terminal (ou terminal émulateur), est un programme qui émule un terminal physique dans une **interface graphique**.

Quelques exemples de terminaux populaires : GNOME terminal, Guake, Terminator, Xterm.

Une console, quant à elle, désigne couramment une interface en ligne de commande qui occupe tout l'écran et qui est généralement accessible directement sur un serveur sans interface graphique (ex : dans le projet born2beroot on nous demande de créer une VM sans interface graphique, et d'ensuite faire la configuration de celle-ci par ligne de commande via la console).

De nos jours, un terminal et une console sont, dans une large mesure, synonymes.

NB : sous Windows, l'interface textuelle dans l'environnement graphique qui permet d'intéragir avec l'ordinateur est appelé une console et non un terminal.

Pour aller plus loin et mieux comprendre le fonctionnement d'un terminal, voici un articles sur les ttys : http://www.linusakesson.net/programming/tty

### Shell
Maintenant que les termes annexes ont été éclaircis, passons à la définition qui nous intéresse dans le cadre de notre projet.

Le shell est un interpréteur de commande qui permet de communiquer avec le système d'exploitation via un langage spécifique. Plus concrètement, c'est un programme accessible depuis la console ou un terminal qui prend en entrée des commandes et retourne une sortie.
Il existe plusieurs shells. Le premier shell UNIX était le Thompson shell. Il a ensuite été remplacé quelques années plus tard par le Bourne shell (sh) qui a ensuite donné naissance à de nombreuses variantes parmis lequelles :
- le Bourne Again shell (bash),
- le C shell (csh) et sa version plus moderne le tcsh,
- le Korn shell (ksh),
- le Z shell (zsh) qui reprend les fonctions les plus pratiques de bash, csh et tcsh.

Le shell le plus populaire sur les sytèmes UNIX est le bash.

Commandes utiles :
Connaître le shell utilisé par défault dans votre terminal : 
`echo $SHELL`
Lancer temporairement un autre shell, par exemple bash :
`bash`
Changer votre shell par défaut, par exemple pour bash :
`chsh -s /path/to/bash`
-> `whereis bash` pour trouver le path 

NB : sous Windows, les shells les plus populaires sont le Command Shell et le Power Shell.

## Overview
Rapide vue d'ensemble du fonctionnement du projet et des concepts qui vont y être abordés.

| ![bash architecture](http://www.aosabook.org/images/bash/bash-article-diagram.png) |
|:--:|
| *Architecture Bash* |

Ton programme va devoir générer une **interface**. Cette dernière doit afficher une invite de commade (prompt) pour indiquer que le programme est prêt à recevoir une saisie au clavier. Elle doit également posséder les fonctionnalités basiques d'édition (raccourcis clavier du mode d'édition Emacs), et d'historique (possibilité de naviguer dans les inputs précédents avec les flèches du haut `↑` et du bas `↓`).

Une fois que l'utilisateur a tapé une ligne de commande et appuyer sur Entrée, l'input est récupéré par le programme et envoyé au **lexer** qui va le décomposer en une suite tokens.

La suite de tokens est ensuite transmise au **parser** qui va créer un **AST** (abstract syntax tree). C'est une structure de données qui permet de représenter la syntaxe d'un langage de programmation.

On repasse alors dans notre arbre pour l'**expansion**, notamment des variables d'environnement. Le résultat des expansions est ensuite splitté (word splitting). 

Enfin, l'AST est envoyé à la fonction d'**exécution** ou seront gérés les redirections, l'exécution des builtins et des commandes simples, et les pipes.

L'ouput de la commande est affiché dans le terminal et l'**exit status** est renvoyé. 

Input -> Lexer -> Syntax Tree Parser -> Expansion -> Execution -> Output + exit status

## Ressources
Documentation de bash : [📎](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html)
Man de bash : [📎](https://www.man7.org/linux/man-pages/man1/bash.1.html)

Bash n'est pas compatible avec le standard POSIX. Cependant il est possible de lancer bash en mode posix avec la commande `bash --posix`. Son comportement est alors modifié pour correspondre au standard POSIX.
Avant de commencer votre projet, vous devez décider quel est votre shell de référence, `bash` ou `bash --posix`.
Comprendre ce qu'est le standard POSIX : [📎](obsidian://open?vault=Second%20Brain&file=POSIX.md)
Liste des changements avec le mode POSIX dans bash : [📎](https://www.gnu.org/software/bash/manual/html_node/Bash-POSIX-Mode.html)

### Interface
**Bibliothèque Readline**
Vue d'ensemble du fonctionnement de la bibliothèque readline : [📎](obsidian://open?vault=Second%20Brain&file=readline.md)
Man de readline : [📎](https://www.man7.org/linux/man-pages/man3/readline.3.html)
Documentation de readline : [📎](https://tiswww.case.edu/php/chet/readline/readline.html)
Documentation de history : [📎](https://tiswww.case.edu/php/chet/readline/history.html)

**Signaux**
Vue d'ensemble du fonctionnement de sigaction : [📎](obsidian://open?vault=Second%20Brain&file=sigaction.md)
Man de sigaction : [📎](https://man7.org/linux/man-pages/man2/sigaction.2.html)
Exemple d'implémentation des signaux dans un shell : [📎](https://indradhanush.github.io/blog/writing-a-unix-shell-part-3/)
Ressource sur la gestion des signaux avec une globale : [📎](https://sgeos.github.io/unix/c/signals/2016/02/24/passing-values-to-c-signal-handlers.html)

NB : Pensez bien à tous les cas où les signaux doivents être gérés (sans process en cours, pendant l'exécution d'une commande, dans un heredoc, dans un subshell...).

### Parsing (lexer, parser, AST)
Reponse Stack Overflow qui explique la différence entre tokenizer, lexer et parser : [📎](https://stackoverflow.com/a/380487)
Vidéo pour comprendre le parsing : [📎](https://www.youtube.com/watch?v=bxpc9Pp5pZM&ab_channel=Computerphile)
Article sur le fonctionnement d'un parser : [📎](https://tomassetti.me/guide-parsing-algorithms-terminology/)

**Lexer - Analyse lexical**
Ressource hand-written lexer (jusqu'à Pros and cons of hand-written lexer code) : [📎](https://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html)
Grammaire du shell : [📎](https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/V3_chap02.html#tag_18_10)
Readme d'un stud 42 pour comprendre la grammaire du shell et l'adapter à minishell : [📎](https://github.com/Batche-Hub/parser-bison)

**Parser - Analyse syntaxique**
Dans l'article sur le fonctionnement d'un parser mentionné plus haut, de nombreux algorithmes de parsing sont présentés : les algorithmes top-down (LL Parser, Earley Parser, Recursive Descent Parser, ...), et les algorithmes bottom-up (CYK parser, LR Parser, ...).
Dans le cas d'un parsing codé à la main comme dans notre cas (c'est-à-dire sans parsing generaor), la liste des parsers implémentable sans trop de difficultés se reduit considérablement.
Les algorithmes top-down semblent largement plus populaires chez les gens qui codent un interpréteur de commande, notament le Recursive Descent parser. Cependant, libre à chacun.e de faire ses propres recherches et d'implémenter le type de parser qu'il ou elle souhaite, ou de créer le sien en s'inspirant librement ou pas des algorithmes de parsing existant.
Quelques ressources intéressantes pour faire son choix :
- le tableau d'algorithmes de cet article est un bon point de départ pour découvrir les principaux algrithmes : [📎](https://tomassetti.me/guide-parsing-algorithms-terminology/#chapter44)
- comparaison de LL et Recursive Descent sur Stack Overflow : [📎](https://stackoverflow.com/questions/1044600/difference-between-an-ll-and-recursive-descent-parser/1044678#1044678)
- comparaison de LL et LR sur Stack Overflow : [📎](https://stackoverflow.com/questions/5975741/what-is-the-difference-between-ll-and-lr-parsing/6824545#6824545)
- réponses qui indiquent que les Recursive Descent parsers sont plus adaptés pour un parsing à la main : [📎](https://stackoverflow.com/a/531736) [📎](https://www.reddit.com/r/programming/comments/p8vv1l/comment/h9udzf6/?utm_source=share&utm_medium=web2x&context=3)

Page wikipedia du parser Recursive Descent (avec exemple d'implémentation en C) : [📎](https://en.wikipedia.org/wiki/Recursive_descent_parser)
Vidéo d'une implémentation d'un Recursive Descent parser pour parser des expressions arithmétiques : [📎](https://www.youtube.com/watch?v=qlqF1tdMQ0o)


**AST**
Une fois que vous avez tokenizer votre input grâce à votre lexer et que vous avez choisi votre algorithme de parsing, la dernière étape est d'implémenter votre parser qui va générer un AST.

Schéma pour visualiser un AST pour une ligne de commande bash (point n°2): [📎](https://files.gogaz.org/42sh-appsinges.html)
Article sur la création d'un AST en python : [📎](https://ruslanspivak.com/lsbasi-part7/)
-> les articles précédents (part 1 à 6) sont également très intéressants

### Exécution
Maintenant que le parsing est terminé et que nous avons un AST avec toutes les informations nécessaires, on peut passer à l'exécution.

**Parcours d'arbre**
La première étape est de trouver la façon la plus adaptée de visiter les noeuds de votre AST.
Page wikipedia pour comprendre le principe de parcours d'arbre et les découvrir les différents algo : [📎](https://en.wikipedia.org/wiki/Tree_traversal)
Cours sur le parcours d'arbre d'un AST :  [📎](https://www.cs.cornell.edu/courses/cs2110/2018fa/L14-Trees/L14-ParsingTrees.pdf)

**Environnement**
Comment accéder aux variables d'environnement grâce aux arguments de votre programme : [📎](https://www.gnu.org/software/libc/manual/html_node/Program-Arguments.html)

**Exécuter des binaires**
Article sur l'utilisation des fonctions execve, fork, wait, pipe, dup et dup2 : [📎](https://n-pn.fr/t/2318-c--programmation-systeme-execve-fork-et-pipe)

Ressource pour bien comprendre le fonctionnement de fork : [📎](https://www.geeksforgeeks.org/fork-system-call/)

**Pipe et redirection**
Ressource sur les pipes dans un processus ayant un fils et la manipulation des file descriptors : [📎](http://www.zeitoun.net/articles/communication-par-tuyau/start)

https://www.geeksforgeeks.org/dup-dup2-linux-system-call/
-> dup, dup2

https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Redirections
-> redirection
https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/V3_chap02.html#tag_18_07_01
-> redirections

https://www.oilshell.org/blog/2016/10/18.html
-> heredoc

**...**
https://stackoverflow.com/questions/13098620/using-stat-to-check-if-a-file-is-executable-in-c
-> stat

https://c-for-dummies.com/blog/?p=3246
-> reading a dir

**Builtins**
https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
-> builtins
https://www.man7.org/linux/man-pages/man1/bash.1.html#SHELL_BUILTIN_COMMANDS
-> builtins

**Exit code**
https://shapeshed.com/unix-exit-codes/
-> exit code
https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html#Exit-Status
-> exit code

TODO :

Rajouter les quotes dans le parsing ?

Check yout understanding : *grammaire d'um langage de programmation, notation BNF/EBNF, arbre syntaxique, finite-state machine, top-down parser, terminal, non-terminal*

Faire les highlights sur Chrome
https://www.denisbouquet.com/send-url-specific-text-highlighted-google-chrome/

Bonnes pratiques generales :
proteger ses appels systeme