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




typedef short int opcode_line[MAXMEMORYSIZE]; /*array of binary representation of opcode lines*/
typedef short int directive_line[MAXMEMORYSIZE]; /*array of binary representation of directives lines*/
