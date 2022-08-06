#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>/*for INT_MAX in numbers checks*/

#define PATH_MAX 4096 /*maximum length for a file name and path name*/
#define FOREVER for(;;)
#define ERROR -1


typedef struct node_label{
	char* label; /*label name*/
	int line_number;
	int label_type; /*struct, entry, extern*/
	int memory_count;
	struct node_label* next;
}node_label; 

typedef struct node_ext{
	char* ext_label;/*extern label name*/
	struct node_ext* next;
}node_ext; 

struct node_cmd{
	int cmd_index; /*opcode index*/
	int args; /*number of argumtns*/
	char* source; /*source if exists*/
	char* destination; /*dest if exists*/
	int line_num;
	int is_label; /*flag is label defined at the start of command*/
	struct node_cmd* next;
}node_cmd; 

struct node_directive{
	int isLabel; /*flag is label defined at the start of directive*/
	short int arg; /*binary representation of argument*/
	int line_num;
	int memory_count;
	struct node_directive* next;
}node_directive; 

struct node_code{
	int memory_count;
	short int code;
	char *literal; /*to save label name if memory count is unknown*/
	struct node_code* next;
}node_code;


typedef struct node_directive directiveLine;/*linked list to save operands of data commands*/
typedef struct node_cmd cmdLine;/*linked list to save lines of operands of instruction command*/
typedef struct node_label labels;/*linked list to save labels*/
typedef struct node_ext externs;/*linked list to save extern labels*/
typedef struct node_code codeWords; /*linked list to save command binary machine code*/

void free_directive_list(directiveLine **head_drctv);
void free_cmd_list(cmdLine **head_cmd);
void free_ext_list(externs **head_extern);
void free_code_list(codeWords *head_code);
void free_labels_list(labels *head_lbl);
