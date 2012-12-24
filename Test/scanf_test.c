#include <stdio.h>
#include <string.h>

#define SIZE 20
#define MAXSIZE 1024

int main()
{
    FILE* ufile;
    char log[SIZE], mdp[SIZE],rlog[SIZE],rmdp[SIZE];
    char buffer[MAXSIZE];
    int trouve=0, fin=0, i=0; 

    ufile = fopen("users","r");
    printf("Login: "); scanf("%s",log);
    printf("Mot de passe: "); scanf("%s",mdp);

    while(!trouve && !fin)
    {
        if(fgets(buffer,MAXSIZE,ufile)!=NULL)
        {
            printf("Ligne: %d \n",i);
            i += 1;

            sscanf(buffer,"%[a-z]:%[a-z]:%*[a-zA-Z] %*[a-zA-Z]",rlog,rmdp);
            printf("rlog: %s \n",rlog);
            printf("rmdp: %s \n",rmdp);
            printf("buffer> %s",buffer);
            if(!strcmp(log,rlog) && !strcmp(mdp,rmdp))
                trouve = 1;
        }else fin = 1;
    }
    
    if (trouve) printf("Trouve \n");
    else printf("Non trouve \n");

    return 0;
}
