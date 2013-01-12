#include <unistd.h>
#include <stdlib.h>
#include <libsck.h>
#include <string.h>
#include "gestion_client.h"
#include "workflow_struct.h"

#define SIZE 20
#define MAXSIZE 1024


FILE* users;            // Variabe globale externe


/* Fonction de connnexion du client
 *     verifie le login et le mot de passe
 */
static int connexionClient(FILE* dialogue,int socket,char* log, char* mdp)
{
    char rlog[SIZE],rmdp[SIZE],rnom[SIZE],rpnom[SIZE];
    char buffer[MAXSIZE];
    int trouve=0, fin=0; 


    while(!trouve && !fin)
    {
        if(fgets(buffer,MAXSIZE,users)!=NULL)
        {
            sscanf(buffer,"%[a-z]:%[a-z]:%[a-zA-Z] %[a-zA-Z]",rlog,rmdp,rnom,rpnom);

            if(!strcmp(log,rlog) && !strcmp(mdp,rmdp))
              { trouve = 1; addUser(&t_users, socket, rlog, rnom, rpnom);}
        }else fin = 1;
    }
    
    return trouve;
}


/* Fonction qui met fin a la connexion du client
 *     ferme le descripteur de fichier dialogue
 *     et le descripteur de socket sd
 */
static void deconnexionClient(FILE* dialogue, void* sd)
{
    fprintf(stdout,"Fin connexion client\n");
    fprintf(dialogue,"Fin connexion\n");
    fclose(dialogue);
    free(sd);
}


void* gestionClient(void* sd)
{
        int tmp  = *((int*)sd),fin=0;
        char* name, log[SIZE],mdp[SIZE];
        char buffer[MAXSIZE];
        FILE *dialogue = fdopen(tmp, "a+");

#ifdef DEBUG
        printf("==== Gestion Client===");
#endif

        // On affiche le nom du client
        if((name = socketVersNom(tmp))==NULL) 
        {
            deconnexionClient(dialogue,sd); 
            perror("Echec socketversNom: "); 
            exit(-1); 
        }

        fprintf(dialogue,"Login: "); fscanf(dialogue,"%s",log);
        fprintf(dialogue,"Mot de passe: "); fscanf(dialogue,"%s",mdp);

        if(connexionClient(dialogue,tmp,log,mdp))
        {
            fprintf(stdout,"Connexion client\n");
            fprintf(stdout, "nom du client: %s\n", name);
            free(name);

            fprintf(dialogue, "HELLO Vous etes connecte sur notre serveur\n");                    

            while(!fin)
            {
                fscanf(dialogue,"%s",buffer);
                if(strncmp(buffer,"exit",4)==0)
                    fin = 1;
                else {
                    fprintf(dialogue,"Vous avez tape: %s\n",buffer);
                    fprintf(dialogue,"Tapez exit pour terminer\n");   
                }
            }
            deconnexionClient(dialogue,sd);
        }else{
            fprintf(stdout,"Echec connexion client\n");
            fprintf(stdout,"Deconnexion client\n");
            fprintf(dialogue,"Echec connexion\n");
            fprintf(dialogue,"Deconnexion\n");
            deconnexionClient(dialogue,sd);
        }

        return NULL;
}
