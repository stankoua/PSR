#ifndef __LIBSCK_H__
#define __LIBSCK_H__

#include <netinet/in.h>

/* initalise la socket */
/* Param: numero de port */
/* Return: socket de lecture */
extern int initialisationServeur(short int *);

/* boucle d'attente des connexions */
extern int boucleServeur(int, void (*)(void*, void*(*)(void*)), void*(*)(void*));

/* Lancement de la gestion des clients */
extern void dummy (void *dialogue, void* (*action)(void *));

/* Fonction donnant le nom de l'hôte */
extern char* socketVersNom(int sd);

#endif
