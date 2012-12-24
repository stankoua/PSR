/** fichier libsck.c **/

/* Fichier d'inclusion */
#include <sys/types.h>
#include <sys/socket.h>
#include "libsck.h"
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>


/* Constantes internes */



/* Initialisation de la communication réseau */
int initialisationServeur(short int *port)
{
	int df;
	struct sockaddr_in adresse;
	int statut, val=1;
	socklen_t taille=sizeof adresse;

	/* Creation d'une socket */
	df=socket(PF_INET,SOCK_STREAM,0);
	if(df<0)
        {perror("initialisationServeur.socket");
		 return -1;
		 }
        if(setsockopt(df,SOL_SOCKET,SO_REUSEADDR,(char*)&val,sizeof(char*))==-1) 
                {perror("Echec setsockopt: "); return -1;}

	/* On fixe l'adresse de la socket */
	adresse.sin_family = AF_INET;
	adresse.sin_addr.s_addr = INADDR_ANY;
	adresse.sin_port = htons(*port);
	statut = bind(df,(struct sockaddr *)&adresse,sizeof(adresse));
	
	if(statut<0) return -1;

	/* On recupere le numero du port utilise */
	statut = getsockname(df,(struct sockaddr *)&adresse,&taille);
	if(statut<0){
		 perror("initialisationServeur.getsockname");
		 return -1;
		 }
	*port=ntohs(adresse.sin_port);
	
	/* Initialisation de l'ecoute */
	#ifdef DEBUG
    printf("================== Initialisation de l'ecoute ===========");
    #endif	
	statut=listen(df,2);		//ARBITRAIREMENT: 2 connexions
	if(statut<0) 
            return -1;

	return df;
}


/* boucle d'attente des connexions */
/* a chaque connexion, lancment de la fonction "fonction" avec comme paramètre, socket et client */
int boucleServeur(int socket, void (*fonction)(void *, void* (*)(void*)), void* (*action)(void*))
{
	struct sockaddr_in adresse;
	socklen_t taille=sizeof adresse;
	int dialogue, *p_dialogue;
	
	while(1)
	{	/* Attente d'une connexion */
	    #ifdef DEBUG
	    printf("==================Attente d'une connexion===========");
	    #endif	
		dialogue = accept(socket,(struct sockaddr *)&adresse,&taille);		
		if(dialogue<0) return -1;
		
		p_dialogue = (int *)malloc(sizeof(int));
		*p_dialogue = dialogue;

		/* Passage de la socket de dialogue a la fonction de traitement */
		#ifdef DEBUG
	    printf("==================Passage de la socket de dialogue a la fonction de traitement===========");
	    #endif	
            (*fonction)(p_dialogue,*action);
        }

        return 0;
}

/* 
 * Devenue  inutil e a cause de lanceClientLeger
 */
void dummy (void *dialogue, void* (*action)(void *))
{
#ifdef DEBUG
    printf("==================Fonction dummy===========");
#endif	
	action(dialogue);
}

char* socketVersNom(int sd)
{
    char* name;
    struct sockaddr_in adresse;           // pour obtenir le nom du client
    socklen_t adr_len = sizeof adresse;
    struct hostent* host;

    if(getpeername(sd,(struct sockaddr*)&adresse,&adr_len)==-1) {perror("Echec getpeername: "); exit (-1);}
    
    if((host = gethostbyaddr(&(adresse.sin_addr),sizeof(struct in_addr),AF_INET))==NULL) {perror("Echec gethostbyaddr: "); exit(-1);}

    name = (char*)malloc(sizeof host->h_name );

    strcpy(name,host->h_name);

    return name;
}
