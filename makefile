all:main
main: main.c preprocessor.c first_step.c second_step.c cmd_check.c helper_func.c label_lists.c main.h preprocessor.h first_step.h second_step.h cmd_check.h label_lists.h
	gcc -g -ansi -pedantic -Wall main.c preprocessor.c first_step.c second_step.c cmd_check.c helper_func.c label_lists.c -o main
clean: rm *~
