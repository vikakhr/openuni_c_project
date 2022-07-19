#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FOREVER for(;;)
#define LINESIZE 81
#define MAXMEMORYSIZE 256
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
	int memory_number;
	struct node_label* next;
}node_label; 

typedef struct node_ext{/*linked list of labels*/
	char* ext_label;
	int line_number;
	struct node_label* next;
}node_ext; 


struct node_cmd{/*linked list to save binary representation??????*/
	int cmd_index;	
	int args;
	char* source;
	char* destination;
	int line_num;
	struct node_cmd* next;
}node_cmd; 


struct node_directive{/*linked list to save binary representation??????*/
	int isLabel;
	short int arg; 
	int line_num;
	struct node_directive* next;
}node_directive; 



typedef struct node_directive directiveLine;/*lines with directive*/
typedef struct node_cmd instructionLine;/*lines with instructions*/
typedef struct node_label labels;/*lines with labels*/
typedef struct node_ext externs;/*lines with labels*/
