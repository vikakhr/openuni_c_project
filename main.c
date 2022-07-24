#include "main.h"
#include "preprocessor.h"
#include "first_step.h"
#include "translator.h"
#include "label_lists.h"
#include "cmd_check.h"




int main(int argc, char *argv[]){
	FILE *ifp;
	char *file_name, *copy_file_name;
	labels *head_lbl = NULL,  *tail_lbl = NULL; /*list of labels*/
	externs *head_extern = NULL, *tail_extern = NULL; /*list of extern labels*/
	directiveLine *head_drctv = NULL, *tail_drctv = NULL; /*head and tail of directives list*/
	cmdLine *head_cmd = NULL, *tail_cmd = NULL; /*head and tail of instructions list*/
	codeWords *head_code = NULL, *tail_code = NULL; /*head and tail of machine code list*/

	char *line = (char*) malloc(sizeof(char)*LINESIZE);
	if(line==NULL)
		exit(0);


	if(argc == 1)
		return 1;
	while(--argc>0){
		file_name = (char*)malloc(strlen(*++argv)+4);/*makes pointer to memory for string with place for extention .as*/
		if(file_name==NULL)
			return 1;
	
		copy_file_name = (char*)malloc(strlen(file_name)+1);/*copies original file name without extension*/
		if(copy_file_name==NULL)
					return 1;
		strcpy(copy_file_name, *argv);

		sprintf(file_name,"%s.as", *argv);/*writes a full name of file*/

		if((ifp = fopen(file_name, "r")) == NULL){/*cannot open file, go to the next one*/
		printf("Can't open %s\n", *argv);
		}
	else {
		preprocessor(file_name);/*preprocessor function*/
		sprintf(file_name,"%s.am", *argv);/*writes a full name of file*/

		read_cmd_line(file_name, &head_lbl, &tail_lbl, &head_drctv, &tail_drctv, &head_cmd, &tail_cmd, &head_extern, &tail_extern); /*check errors*/		
		check_label_defined(&head_lbl, &head_extern, &head_cmd);
		translate_lines(copy_file_name, &head_code, &tail_code, &head_cmd, &tail_cmd, &head_drctv, &head_lbl, &head_extern, &tail_extern);

		print_extlabel_list(&(*head_extern));/*CHECK*/



		fclose(ifp);
		}
}

	free(file_name);
	free(copy_file_name);
return 0;
}
