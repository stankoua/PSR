#ifndef __OPTIONSERVER_H__
#define __OPTIONSERVER_H__


/* Affichage des options */
void display_right_syntax();

/* Analyse des options 
 * Renvoie le numero de port
 */
int analyseArg(int argc,char * argv[],short int *nport,char* ufname);

#endif
