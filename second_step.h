

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

struct node_struct{/*linked list to save binary representation??????*/
	char* label;
	char* struct_name;
	char* argument;
	struct node_struct* next;
}node_struct; 


struct label{/*linked list to save binary representation??????*/
	char* label;
	int memory_num;
	struct entry_label* next;
}label; 



struct entry_label{/*linked list to save binary representation??????*/
	char* label;
	int memory_num;
	struct entry_label* next;
}entry_label; 

struct extern_label{/*linked list to save binary representation??????*/
	char* label;
	int memory_num;
	struct extern_label* next;
}extern_label; 


typedef struct node_opcode opcodeLine;/*lines with opcode*/
typedef struct node_directive directiveLine;/*lines with directive*/
typedef struct node_struct structLine;/*lines with structs*/
typedef struct label labels;/*lines with structs*/
typedef struct entry_label entryLabels;/*lines with structs*/
typedef struct extern_label externLabels;/*lines with structs*/





/*
int check_repeated_labels(char* name, node_label* head);
void add_node(node_label** head, node_label** tail, char* name, int memory_num);
int count_memory_num(char* line, int num);
void free_label_list(node_label* head);
*/


