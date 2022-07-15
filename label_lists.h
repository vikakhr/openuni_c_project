
enum {STRUCT, EXTERN, ENTRY}; /*label's type number, data type includes .data and .string*/



typedef struct node_label{/*linked list of labels*/
	char* label;
	int line_number;
	int label_type;
	int memory_number;
	struct node_label* next;
}node_label; 


/*
struct entry_label{/*linked list of entry labels
	char* label;
	int line_number;
	int memory_num;
	struct entry_label* next;
}entry_label; 

struct extern_label{/*linked list of extern
	char* label;
	int line_number;
	int memory_num;
	struct extern_label* next;
}extern_label; 

struct node_struct{/*linked list of structs
	char* struct_name;
	int line_number;
	struct node_struct* next;
}node_struct; 

*/



typedef struct node_label labels;/*lines with labels*/

/*typedef struct entry_label entryLabels;/*lines with structs
typedef struct extern_label externLabels;/*lines with structs
typedef struct node_struct structs;/*lines with structs*/





int check_repeated_labels(char* name, labels* head);
void add_node_label(labels** head, labels** tail, char* name, int line, int label_type);
int check_label_positioning(labels** head, labels** tail, char* label, int label_type, int line_num);


/*void add_node_struct(structs** head, structs** tail, char* name, int line);
void add_node_entry_label(entryLabels** head, entryLabels** tail, char* name, int line);
void add_node_extern_label(externLabels** head, externLabels** tail, char* name, int line);*/



void print_label_list(labels* head);


