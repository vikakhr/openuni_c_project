

typedef struct node_1{/*linked list to save labels*/
	char* name;
	int line_number;
	struct node* next;
}node_1;

typedef struct node_1 node_label;

typedef struct node_2{/*linked list to save errors*/
	int line_number;
	char error_name[MAX_ERROR_NAME];
	struct node2* next;
}node_2;

typedef struct node_2 node_error;




int check_repeated_labels(char* name, node_label* head);

void add_node(node_label** head, node_label** tail, char* name, int memory_num);


int count_memory_num(char* line, int num);
void free_label_list(node_label* head);



