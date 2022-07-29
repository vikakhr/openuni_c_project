#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FOREVER for(;;)
#define LINESIZE 81
#define ERROR -1
#define OK 1
#define LABELSIZE 30 /*max length of label name*/
#define CMDLENGTH 16 /*number of opcodes*/
#define REGLENGTH 8 /*number of registers*/
#define DRCTVNUM 5 /*number of directives*/


typedef struct node_label{/*linked list of labels*/
	char* label;
	int line_number;
	int label_type;
	int memory_count;
	struct node_label* next;
}node_label; 

typedef struct node_ext{/*linked list of labels*/
	char* ext_label;
	struct node_ext* next;
}node_ext; 

struct node_cmd{/*linked list to save binary representation??????*/
	int cmd_index;	
	int args;
	char* source;
	char* destination;
	int line_num;
	int is_label;
	struct node_cmd* next;
}node_cmd; 

struct node_directive{/*linked list to save binary representation??????*/
	int isLabel;
	short int arg; 
	int line_num;
	int memory_count;
	struct node_directive* next;
}node_directive; 

struct node_code{
	int memory_count;
	short int code;
	char *literal;
	int addressing_type;
	struct node_code* next;
}node_code;


typedef struct node_directive directiveLine;/*linked list to save operands of data commands*/
typedef struct node_cmd cmdLine;/*linked list to save lines of operands of instruction command*/
typedef struct node_label labels;/*linked list to save labels*/
typedef struct node_ext externs;/*linked list to save extern labels*/
typedef struct node_code codeWords; /*linked list to save command binary machine code*/

void free_directive_list(directiveLine **head_drctv, directiveLine **tail_drctv);
void free_cmd_list(cmdLine **head_cmd);
void free_ext_list(externs **head_extern, externs **tail_extern);
void free_code_list(codeWords *head_code);
void free_labels_list(labels *head_lbl);
