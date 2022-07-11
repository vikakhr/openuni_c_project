
typedef struct node_label{/*linked list to save binary representation??????*/
	char* label;
	int line_number;
	int memory_num;
	struct node_label* next;
}node_label; 



struct entry_label{/*linked list to save binary representation??????*/
	char* label;
	int line_number;
	int memory_num;
	struct entry_label* next;
}entry_label; 

struct extern_label{/*linked list to save binary representation??????*/
	char* label;
	int line_number;
	int memory_num;
	struct extern_label* next;
}extern_label; 

struct node_struct{/*linked list to save binary representation??????*/
	char* label;
	char* struct_name;
	char* argument;
	int line_number;
	struct node_struct* next;
}node_struct; 



typedef struct node_struct structs;/*lines with structs*/
typedef struct node_label labels;/*lines with structs*/
typedef struct entry_label entryLabels;/*lines with structs*/
typedef struct extern_label externLabels;/*lines with structs*/


int check_repeated_labels(char* name, labels* head);
void add_node(labels** head, labels** tail, char* name, int line);
