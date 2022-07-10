all:main
main: main.c preprocessor.c first_step.c second_step.c cmd_check.c helper_func.c main.h preprocessor.h first_step.h second_step.h cmd_check.h
	gcc -g -ansi -pedantic -Wall main.c preprocessor.c first_step.c second_step.c cmd_check.c helper_func.c -o main
clean: rm *~
