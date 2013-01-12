/* Programme principal du moteur */

#include <libsck.h>
#include <libthrd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// stdio.h amené par gestion_client.h

#include "optionServer.h"
#include "gestion_client.h"
#include "workflow_struct.h"


int main(int argc, char *argv[])
{
  short int nport=4000;          // nport num port utilisé pour la connexion
  int ecoute;                    // Descripteur de la socket d'ecoute
  char *ufname;
  char nomfichier[SMALLSIZE];

  ufname = (char*)malloc(sizeof "users");
  strcpy(ufname,"users");

  /* Lecture des arguments de la commande */
#ifdef DEBUG
  printf("======= Lecture ======");
#endif

  // On verifie que le arguments sont bons
  if((analyseArg(argc,argv,&nport,ufname))==-1) {display_right_syntax(); return -1;}

  // Test sur l'ouverture du fichier des logins
  if((users = fopen(ufname,"r"))==NULL) {perror("Echec fopen users:"); return -1;}

#ifdef DEBUG
  printf("======= Fin Lecture ======");
#endif

  // fichier à parser
  printf("Entrez le fichier à parser: ");
  scanf("%s",nomfichier);
  initTabUsers(&t_users);
  initTabProcess(&t_process);
  addProcess(&t_process, nomfichier);

  /* Initialisation du serveur */
  ecoute = initialisationServeur(&nport);

  /* Lancement de la boucle d'ecoute */
  //if(boucleServeur(ecoute,dummy,gestionClient)==-1) perror("Echec serveur");
  if(boucleServeur(ecoute,lanceClientLeger,gestionClient)==-1) perror("Echec serveur");

  fclose(users);
  free(ufname);
  return 0;
}
