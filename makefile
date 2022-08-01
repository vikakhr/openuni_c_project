all:main
main: main.c preprocessor.c first_step.c cmd_check.c helper_func.c label_lists.c translator.c main.h preprocessor.h first_step.h cmd_check.h helper_func.h label_lists.h  translator.h
	gcc -g -ansi -pedantic -Wall main.c preprocessor.c first_step.c cmd_check.c helper_func.c label_lists.c translator.c -o main
clean: rm *~
