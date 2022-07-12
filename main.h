#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FOREVER for(;;)
#define LINESIZE 81
#define MAXMEMORYSIZE 256

typedef short int opcode_line[MAXMEMORYSIZE]; /*array of binary representation of opcode lines*/
typedef short int directive_line[MAXMEMORYSIZE]; /*array of binary representation of directives lines*/
