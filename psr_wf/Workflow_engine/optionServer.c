/* optionServer.c contiendra les fonctions: 
 * d'analyse des arguments et d'affichage 
 * de la syntaxe correcte 
 */

#include "optionServer.h"
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

void display_right_syntax() {
	printf("Syntax error. You should try :\n \tmain [-p | --port] <numport>\n \tnumport should be different from 4000, which is the default value\n \tmain [-u | --user] <numport>\n \tuser file\n");
}

int analyseArg(int argc,char * argv[],short int *nport,char* ufname) {
	int c;
	
	#ifdef DEBUG
	printf("==================analyseAgruments=================");
	#endif	
        
	//by carter
	while (1) {
		const char *shortopts = "p:u:";	//declaration des options courtes
		const struct option longopts[] = {	//declaration des options longues
			{"port",required_argument,NULL,'p'},
                        {"users_file",required_argument,NULL,'u'},
			{0, 0, 0, 0}	// {NULL,0,NULL,\0}
		};

       	        c = getopt_long(argc, argv, shortopts,longopts, NULL);
       	        if (c == -1)
                    // pas ou plus d'argument
                    break;

       	        switch (c) {
                    case 'p':
                        *nport = atoi(optarg);
                        break;
                    case 'u':
                        ufname = (char*)realloc(ufname, sizeof optarg);
                        strcpy(ufname,optarg);
                        break;
	            default:
	        	return -1;
	        	break;
		}
	}
	
	#ifdef DEBUG
	printf("==================Fin Analyse===========");
	#endif	
	return 0;
}
