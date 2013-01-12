/* Programme principal du moteur */

#include "optionServer.h"
#include "gestion_client.h"
#include <libsck.h>
#include <libthrd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// stdio.h amené par gestion_client.h


int main(int argc, char *argv[])
{
        short int nport=4000;          // nport num port utilisé pour la connexion
        int ecoute;                    // Descripteur de la socket d'ecoute
        char *ufname;
        

        ufname = (char*)malloc(sizeof "users");
        strcpy(ufname,"users");

	/* Lecture des arguments de la commande */
        #ifdef DEBUG
        printf("======= Lecture ======");
        #endif
        
        // On verifie que le arguments sont bons
        if((analyseArg(argc,argv,&nport,ufname))==-1)
                {display_right_syntax(); return -1;}
        
        // Test sur l'ouverture du fichier des logins
        if((users = fopen(ufname,"r"))==NULL)
                {perror("Echec fopen:"); return -1;}
        

        #ifdef DEBUG
        printf("======= Fin Lecture ======");
        #endif

	/* Initialisation du serveur */
	ecoute = initialisationServeur(&nport);
		
	/* Lancement de la boucle d'ecoute */
	//if(boucleServeur(ecoute,dummy,gestionClient)==-1) perror("Echec serveur");
	if(boucleServeur(ecoute,lanceClientLeger,gestionClient)==-1) perror("Echec serveur");

        fclose(users);
        free(ufname);

        return 0;
}
