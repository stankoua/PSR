#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 1024
#define SIZE 20

int main()
{
    char buffer[MAXSIZE], *ufname;
    int taille = sizeof "users";
    FILE* users;
    
    ufname = (char*)malloc(sizeof "users");
    strcpy(ufname,"users");
    
    printf("fichier: %s \n",ufname);
    printf("taille: %d \n",taille);                 // 6
    printf("dernier char: %c \n",ufname[taille-2]); // dernier car cad "s"

    if((users = fopen(ufname,"r"))==NULL)
        {perror("Echec fopen:"); return -1;}
    fgets(buffer,MAXSIZE,users);
    printf(">>>> %s",buffer);

    fgets(buffer,MAXSIZE,users);
    printf(">>>> %s",buffer);

    if(ufname[taille-1]=='\0')
        printf("\\0 est dans la chaine \n");
    else
        printf("\\0 n est pas dans la chaine \n");

    return 0;
}
