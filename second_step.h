

/*Header contains data tables*/








struct node_opcode{/*linked list to save binary representation??????*/
	char* label;
	char* opcode;
	char* source;
	char* destination;
	struct node_opcode* next;
}node_opcode; 

struct node_directive{/*linked list to save binary representation??????*/
	char* label;
	char* directive;
	char* argument;
	struct node_directive* next;
}node_directive; 


typedef struct node_opcode opcodeLine;/*lines with opcode*/
typedef struct node_directive directiveLine;/*lines with directive*/






/*
int check_repeated_labels(char* name, node_label* head);
void add_node(node_label** head, node_label** tail, char* name, int memory_num);
int count_memory_num(char* line, int num);
void free_label_list(node_label* head);
*/


