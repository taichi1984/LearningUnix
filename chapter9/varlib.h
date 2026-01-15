#include<stdio.h>

int VLstore(char *var, char *val);
char *VLlookup(char *name);
int VLexport(char *var);
void VLlist();
int VLenviron2table(char *env[]);
char **VLtable2environ();
struct var;

