#include "main.h" /*for constants*/

/*Header contains data tables*/








struct node_opcode{/*linked list to save binary representation??????*/
	char* cmd_line[LINESIZE];
	char* label[LABELSIZE+1];
	int opcode_index;
	int line_num;
	struct node_opcode* next;
}node_opcode; 

struct node_directive{/*linked list to save binary representation??????*/
	char* drctv_line[LINESIZE]
	char* label[LABELSIZE+1];
	int directive_type; 
	struct node_directive* next;
}node_directive; 


typedef struct node_opcode opcodeLine;/*lines with opcode*/
typedef struct node_directive directiveLine;/*lines with directive*/

void add_opcode_node(struct opcodeLine **head, struct opcodeLine **tail, char *line, char *label, int opcode_index);
void add_directive_node(struct node_directive **head, struct node_directive **tail, char *line, char *label, int directive_type);



/*
int check_repeated_labels(char* name, node_label* head);
void add_node(node_label** head, node_label** tail, char* name, int memory_num);
int count_memory_num(char* line, int num);
void free_label_list(node_label* head);
*/


