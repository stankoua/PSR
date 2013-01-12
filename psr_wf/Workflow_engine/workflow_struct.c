#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "workflow_struct.h"
#include "parser.h"


/**
 * Fonction init table process
 */

void initTabProcess(struct TabProcessType* t_process)
{
  int i;
  for(i=0;i<SMALLSIZE;i++)
    t_process->tab_process[i] = NULL;

  t_process->nbprocess = 0;
}


/**
 * Fonction init table users
 */

void initTabUsers(struct TabUsersType* t_users)
{
  int i;
  for(i=0;i<SMALLSIZE;i++)
    t_users->tab_users[i] = NULL;

  t_users->nbusers = 0;
}


/**
 * Fonction ajout table process
 */

int addProcess(struct TabProcessType* t_process, char* fichier)
{
  int i = t_process->nbprocess;
  struct ProcessType *process;

  printf("======= addProcess before parsing \n"); 
  // Parse le fichier du process
  //if((process = parseFile(fichier))==NULL) 
    //{perror("Erreur parser fichier:"); return 1;}

  process = parseFile(fichier); 

  printf("======= addProcess end parsing\n"); 

  t_process->tab_process[i] = process;
  t_process->nbprocess += 1;

  printf("======= FIN addProcess\n"); 
  return 0;
}


/**
 * Fonction ajout table users
 */

int addUser(struct TabUsersType* t_users, int socket, char* login, char* nom, char* prenom)
{
  int i = t_users->nbusers;
  struct UserType *user;
  
  // Allocation 
  if( (user = (struct UserType*)malloc(sizeof(struct UserType)))==NULL)
    { perror("Erreur malloc: "); return -1;}
  user->socket = socket;
  strcpy(user->login,login);
  strcpy(user->nom,nom);
  strcpy(user->prenom,prenom);
  
  t_users->tab_users[i] = user;
  t_users->nbusers += 1;
  
  return 0;
}

void freeProcess(struct TabProcessType* t_process)
{
  int i;
  for(i=0;i<t_process->nbprocess;i++)
    free(t_process->tab_process[i]);
  
  t_process->nbprocess = 0;
}

void freeUsers(struct TabUsersType* t_users,int i)
{
  int last = t_users->nbusers-1;
  free(t_users->tab_users[i]);
  t_users->tab_users[i] = t_users->tab_users[last];
  t_users->tab_users[last] = NULL;
  t_users->nbusers -= 1;
}
