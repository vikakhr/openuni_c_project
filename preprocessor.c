/*This file contains preprocessor functions*/

#include "main.h" /*main libraries*/
#include "preprocessor.h" /*functions*/

void preprocessor(char *sourceFileName){/*receives name of source file*/
	FILE *sfp, *dfp;/*sfp is .as source file, dfp is .am destination file*/
	node_macro *head = NULL, *tail = NULL;/*linked list nodes for macroes*/		
	char* macro = "macro";
	char* endmacro = "endmacro";
	char* command, *commandCopy;
	int cmdLength = 0;  /*command length for realloc*/
	int isInside = 0;/*flag if inside macro*/
	int isMacroInFile = 0;

	char* destFileName = (char*)malloc(strlen(sourceFileName));/*allocates memory for new .am file*/
	if(destFileName==NULL)
		return;
	
	strcpy(destFileName, sourceFileName);
	destFileName[strlen(destFileName)-1] = 'm';/*change extension to .am*/
	
	if((sfp = fopen(sourceFileName, "r")) == NULL){/*cannot open source file, exit*/
		printf("Cannot open %s\n", sourceFileName);
		free(destFileName);
		return;
	}

	if((dfp = fopen(destFileName, "w"))==NULL){/*cannot read/create destination file, exit*/
		printf("Cannot open %s\n", destFileName);
		free(destFileName);
		return;
	}

	FOREVER {
		command = (char*)malloc(sizeof(char)*LINESIZE);
		if(command==NULL)
			return;

		if(fgets(command+cmdLength, LINESIZE, sfp)==NULL){/*reads a line of LINESIZE length, checks if empty*/
			break;	
		}	

		commandCopy = (char *)malloc(strlen(command)+1);
		if(commandCopy==NULL)
			break;
		strcpy(commandCopy, command);/*makes a copy of original command*/
		

		if(isInside){/*if inside macro add this line to macro data*/
			if(strstr(command, endmacro)){/*if endmacro found*/
				isInside = 0;
				continue;
			}
			put_macro_data(command, &tail);/*add new data to this macro*/
			continue;
		}
		else if(strstr(command, macro)==NULL){/*line without macro*/
			if(is_one_word(command))/*if line contains one word*/
				if(isMacroInFile)/*if macro already exists in file*/
					if(write_macro_data(command, head, dfp))/*if this is macro name write and go to next line*/
						continue;
			fprintf(dfp, "%s", command); /*write inside .am file*/
			continue;/*go to read next line*/
		}

		else{   				
			if(if_is_macro(commandCopy)){/*if new macro was found*/	
				isMacroInFile = 1;/*turn on macro exists in file flag*/
				isInside = 1; /*turn on inside macro flag*/
				add_node_macro(&head, &tail, take_macro_name(command));
					continue;	
			}
		}
		free(command);
	}	

	fclose(sfp);
	fclose(dfp);
	free(destFileName);
	if(isMacroInFile){/*if linked list contains some nodes*/
		free_list(head);

	}
}

/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list*/
void add_node_macro(node_macro** head, node_macro** tail, char* name){
	node_macro *new = malloc(sizeof(node_macro));
	new->name = (char*)malloc(strlen(name)+1);
	new->data = NULL;
	new->next = NULL;
	strcpy(new->name, name);
	if(*head==NULL){/*if this is first node*/
		*head = new;
		*tail = new;		
		return;
	}		
	else if(*tail == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = new;
	}
	else {	
		(*tail)->next = new;
		*tail = new;
	}
}

/*Function receives line contains macro name and tail and adds a data to list*/
void put_macro_data(char* string, node_macro** tail){
	if(((*tail)->data) == NULL){
		(*tail)->data = (char*)malloc(strlen(string)+1);
		if(((*tail)->data)==NULL)
			return;
		strcpy((*tail)->data, string);
	}
	else {
		(*tail)->data = (char*)realloc((*tail)->data, strlen((*tail)->data)+1+strlen(string)+1);
		if(((*tail)->data)==NULL)
			return;
		strcat((*tail)->data, string);
	}
}

/*Function receives line, returns 1 if it contains one word, return 0 if it empty or contains more than one word*/
int is_one_word(char* string){
	char *p;	
	char *white_space = " \t\v\f\r";
	if((p=strtok(string, white_space)) == NULL)/*empty*/
		return 0;
	if((p=strtok(NULL, white_space)) == NULL)/*one word line*/
		return 1;
	return 0;	
}

/*Function checks if this is macro command*/
int if_is_macro(char* string){
	char *white_space = " \t\v\f\r";
	char* str = "macro";
	char *p;
	p = strtok(string, white_space);
	printf("Inside: %s\n", p);
	if(strcmp(p, str)){/*if not macro*/
		printf("Inside2: %s\n", p);
		return 0;
		
	}
	printf("Inside3: %s\n", p);
	return 1;
}

/*Function checks if this is endmacro command*/
int if_is_endmacro(char* string){
	char *white_space = " \t\v\f\r";
	char *p;
	p = strtok(string, white_space);
	if(strcmp(p, "endmacro"))/*if not endmacro*/
		return 0;		
	return 1;
}

/*Function receives first line and returns name of macro*/
char* take_macro_name(char* string){
	char* p;
	char *white_space = " \t\v\f\r";
	p = strtok(string, white_space);
	printf("%s\n", p);
	p = strtok(NULL, white_space);
	printf("%s\n", p);
	return p;
}

/*writes data into file*/
int write_macro_data(char* string, node_macro* head, FILE *dfp){
	node_macro* ptr = head;
	char* p;
	char *white_space = " \t\v\f\r";
	p = strtok(string, white_space);
	while(ptr!=NULL){
		if(!strcmp(ptr->name, p)){/*search name of macro in the list*/
			fprintf(dfp, "%s", ptr->data);/*if found write data*/
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}


/*Function receives a head of linked list and frees memory of each node*/
void free_list(node_macro* head){
	node_macro* ptr;

	while(head!=NULL){
		ptr = head;
		head = head->next;
		free(ptr->name);/*free memory of name*/
		free(ptr->data);/*free memory of data*/
		free(ptr);/*free memory of node*/
	}
}

