#include "main.h"
#include "preprocessor.h"
#include "first_step.h"
#include "second_step.h"
#include "cmd_check.h"




int main(int argc, char *argv[]){
FILE *ifp;
char *file_name, *ptr;



if(argc == 1)
	return 1;
while(--argc>0){
	file_name = (char*)malloc(strlen(*++argv)+4);/*makes pointer to memory for string with place for extention .as*/
	if(file_name==NULL)
		return 1;
	
	sprintf(file_name,"%s.as", *argv);/*writes a full name of file*/

	if((ifp = fopen(file_name, "r")) == NULL){/*cannot open file, go to the next one*/
		printf("Can't open %s\n", *argv);
	}
	else {
		preprocessor(file_name);/*preprocessor function*/
		ptr = strchr(file_name, '.');
		sprintf(file_name,"%s.am", *argv);/*writes a full name of file*/

		check_cmd_line(file_name); /*check errors*/
		

		fclose(ifp);

		}
}

	free(file_name);

return 0;
}
