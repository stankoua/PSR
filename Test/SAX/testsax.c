#include <string.h>
#include <libxml/parser.h>

#include "workflow_struct.h"

#define psprs pstate->process


static xmlSAXHandler handler;

//static int elem_courant;
// Comment: implementer le type "typeS"

/**
 *  Un char a ete lu
 */
void caracteres(struct ParserStateType* pstate, const xmlChar *text, int length)
{
    char *tmp = NULL;
    char str[LONGSIZE];          // Utile pour strtok
    strncpy(str,text,LONGSIZE);
    printf("etat actuel character # %d #\n",pstate->state);
    switch(pstate->state)
    {
        case DESCRIPTION:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
	        //printf( "==============TEST Chaine Extraite: %s\n\n",pstate->buffer );
            break;

        case ACT_DESCRIPTION:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
	        //printf( "==============TEST Chaine Extraite: %s\n\n",pstate->buffer );
            break;

        case ACT_PERFORMER:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
	        //printf( "==============TEST Chaine Extraite: %s\n\n",pstate->buffer );
            break;

        default:
            // Ne nous interesse pas, donc on affiche pas
            // printf("Autre character:>>%s<<:\n",text);
            break;
    }
}


/**
 *  Une balise fermante a été lue
 */
void finElement(struct ParserStateType* pstate, const xmlChar *name)
{
    int i;
    printf("fin de l'element:>>%s<<:\n",(char *)name);
    printf("etat actuel finE # %d #\n",pstate->state);
    switch(pstate->state)
    {
        case PROCESS:
            if(!strcmp(name,"tns:process")) {
                // Fin de fichier atteint
                pstate->state = FINISH;
            }else 
                { printf("Erreur element %s\n",name); exit(-1); }            
            break;

        case DESCRIPTION:
            if(!strcmp(name,"tns:description")) {
                strcpy(pstate->process->description,pstate->buffer);
                //printf("========TEST Description du process: %s\n\n",pstate->process->description);
                // ================================== Pas géré for now ========================
                // On enregistre le nom du process en meme tps que la description
                // Pour eviter ne pas le faire en fin de fichier
                //strcpy(pstate->process->name,pstate->bufatr[0]);
                //printf("======== Nom du process-- %s --\n\n",pstate->process->name);
                pstate->state = PROCESS;
            }else 
                { printf("Erreur element %s\n",name); exit(-1); }            
            break;

        case VARIABLE_LIST:
            if(!strcmp(name,"tns:variableList")) {
                pstate->state = PROCESS;
            }else 
                { printf("Erreur element %s\n",name); exit(-1); }            
            break;

        case VARIABLE:
            if(!strcmp(name,"tns:variable")) {
                i = pstate->process->nbvar;
                //printf("======== test nbre variable: %d\n\n",pstate->process->nbvar);
                strcpy(pstate->process->variableList[i].name,pstate->bufatr[0]);
                //printf("======== test attribut variable: %s\n\n",pstate->bufatr[0]);
                //printf("========TEST variable process nom: %s\n\n",pstate->process->variableList[i].name);
                strcpy(pstate->process->variableList[i].type,pstate->bufatr[1]);
                //printf("======== test attribut variable: %s\n\n",pstate->bufatr[1]);
                //printf("========TEST variable process type: %s\n\n",pstate->process->variableList[i].type);
                if(pstate->nbatr == 3)
                {
                    strcpy(pstate->process->variableList[i].initialvalue,pstate->bufatr[2]);
                    //printf("========TEST variable process init value: %s\n\n",pstate->process->variableList[i].initialvalue);
                }
                pstate->process->nbvar += 1;
                pstate->state = VARIABLE_LIST;
            }else 
                { printf("Erreur element %s\n",name); exit(-1); }            
            break;

        case ACTIVITY_LIST:
            if(!strcmp(name,"tns:activityList")) {
                pstate->state = PROCESS;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACTIVITY:
            if(!strcmp(name,"tns:activity")) {
                pstate->process->nbact += 1;
                pstate->state = ACTIVITY_LIST;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_DESCRIPTION:
            if(!strcmp(name,"tns:description")) {
                // On enregistre d'abord les attributs de l'actvité
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].id,pstate->bufatr[0]);
                //printf("========TEST activity process nom: %s\n\n",pstate->process->activityList[i].id);
                strcpy(pstate->process->activityList[i].name,pstate->bufatr[1]);
                //printf("========TEST activity process type: %s\n\n",pstate->process->activityList[i].name);
                // Ensuite la description
                strcpy(pstate->process->activityList[i].description,pstate->buffer);
                //printf("========TEST activity process description: %s\n\n",pstate->process->activityList[i].description);
                // Fini 
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_PERFORMER:
            if(!strcmp(name,"tns:performer")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].performer,pstate->buffer);
                //printf("========TEST activity process performer: %s\n\n",pstate->process->activityList[i].performer);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_INPUT:
            if(!strcmp(name,"tns:input")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].input,pstate->bufatr[0]);
                //printf("========TEST activity process input: %s\n\n",pstate->process->activityList[i].input);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_OUTPUT:
            if(!strcmp(name,"tns:output")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].output,pstate->bufatr[0]);
                //printf("========TEST activity process output: %s\n\n",pstate->process->activityList[i].output);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case TRANSITION_LIST:
            if(!strcmp(name,"tns:transitionList")) {
                pstate->state = PROCESS;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case TRANSITION:
            if(!strcmp(name,"tns:transition")) {
                i = pstate->process->nbtrans;
                strcpy(pstate->process->transitionList[i].id,pstate->bufatr[0]);
                //printf("========TEST transition process id: %s\n\n",pstate->process->transitionList[i].id);
                strcpy(pstate->process->transitionList[i].from,pstate->bufatr[1]);
                //printf("========TEST transition process from: %s\n\n",pstate->process->transitionList[i].from);
                strcpy(pstate->process->transitionList[i].to,pstate->bufatr[2]);
                //printf("========TEST transition process to: %s\n\n",pstate->process->transitionList[i].to);
                pstate->state = TRANSITION_LIST;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case TRANS_CONDITION:
            if(!strcmp(name,"tns:condition")) {
                // On ne sauve pas comme précédement les attributs de transition
                // On le fera quand on trouvera transition
                // Par contre on commence a remplir la transition ici
                i = pstate->process->nbtrans;
                strcpy(pstate->process->transitionList[i].condition,pstate->bufatr[3]);
                //printf("========TEST transition process condition: %s\n\n",pstate->process->transitionList[i].condition);
                pstate->state = TRANSITION;
            }else 
                { printf("Erreur element %s",name); exit(-1); }
            break;

        default:
            // ERREUR
            //printf("Erreur fin element: %s\n",name);
            //exit(-1);
            break;
    }
    printf("nouvel etat finE # %d #\n",pstate->state);
}


/**
 *  Une balise ouvrante a été lue
 *  
 */
void debutElement(struct ParserStateType* pstate, const xmlChar *name, const xmlChar **attrs)
{
    int i,j;
    char* tmp = NULL;
    printf("Debut de l'element:>>%s<<:\n", name);
    printf("etat actuel debE # %d #\n",pstate->state);
    switch(pstate->state)
    {
        case START:
            if(!strcmp(name,"tns:process")) {
                pstate->state = PROCESS;
                /* Lecture des attributs
                 * Un attribut: name
                 */
                i = 0;
                //strcpy( pstate->bufatr[i], tmp);
                //pstate->nbatr = i;
		        //=====================================================         
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case PROCESS:
            if(!strcmp(name,"tns:description")) {
                pstate->state = DESCRIPTION;
            }else if(!strcmp(name,"tns:variableList")){
                pstate->state = VARIABLE_LIST;
            }else if(!strcmp(name,"tns:activityList")){
                pstate->state = ACTIVITY_LIST;
            }else if(!strcmp(name,"tns:transitionList")){
                pstate->state = TRANSITION_LIST;
            }else// ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case VARIABLE_LIST:
            if(!strcmp(name,"tns:variable")) {
                pstate->state = VARIABLE;
                // Lecture des attributs
                // 3 attributs: name, type, initialValue
                i = 0; j = 0;
                while(attrs[i]!=NULL)
                {
                    strcpy(pstate->bufatr[j], attrs[i+1]);
                    //printf("======== test attribut variable %d: %s\n\n",j,pstate->bufatr[j]);
                    i += 2; j += 1;
                }
                pstate->nbatr = j;
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }  
            break;

        case ACTIVITY_LIST:
            if(!strcmp(name,"tns:activity")) {
                pstate->state = ACTIVITY;
                // Lecture des attributs
                // 2 attributs: id, name
                i = 0; j = 0;
                while(attrs[i]!=NULL)
                {
                    strcpy(pstate->bufatr[j], attrs[i+1]); 
                    i += 2; j += 1;
                }
                pstate->nbatr = j;
            } else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case ACTIVITY:
            if(!strcmp(name,"tns:description")) {
                pstate->state = ACT_DESCRIPTION;
            } else if(!strcmp(name,"tns:performer")) {
                pstate->state = ACT_PERFORMER;
            } else if(!strcmp(name,"tns:input")) {
                // Lecture des attributs
                // 1 attribut: name
                strcpy(pstate->bufatr[0], attrs[1]); 
                pstate->nbatr = 1;
                pstate->state = ACT_INPUT;               
            } else if(!strcmp(name,"tns:output")) {
                // Lecture des attributs
                // 1 attribut: name
                strcpy(pstate->bufatr[0], attrs[1]); 
                pstate->nbatr = 1;
                pstate->state = ACT_OUTPUT;
            } else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case TRANSITION_LIST:
            if(!strcmp(name,"tns:transition")) {
                pstate->state = TRANSITION;
                // Lecture des attributs
                // 3 attributs: id, from, to
                i = 0; j = 0;
                while(attrs[i]!=NULL)
                {
                    strcpy(pstate->bufatr[j], attrs[i+1]);
                    //printf("======== test attribut variable %d: %s\n\n",j,pstate->bufatr[j]);
                    i += 2; j += 1;
                }
                pstate->nbatr = j;
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case TRANSITION:
            if(!strcmp(name,"tns:condition")) {
                pstate->state = TRANS_CONDITION;
                // On ne fait pas comme d'habitude
                // On copie l'attribut à la fin du tableau
                j = pstate->nbatr;
                strcpy(pstate->bufatr[j], attrs[0]);
                pstate->nbatr += 1;
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case FINISH:
            // ERREUR
            //printf("Erreur element %s",name); 
            printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<FINISH>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"); 
            //exit(-1); 
            break;
        
        default:
            // ERREUR
            // balise ne contenant pas d'autre balises
            // On ne change pas l'état courant
            // cas DESCRIPTION, TRANS_CONDITION,FINISH,VARIABLE
            // ACT_DESCRIPTION,ACT_PERFORMER,ACT_INPUT,ACT_OUTPUT
            //printf("Erreur %s \n",name); 
            //exit(-1); 
            break;
    }
    printf("nouvel etat debE # %d #\n",pstate->state);    
}


void debutDocument(struct ParserStateType* pstate)
{
    // Initialisation de process et de ses attributs
    psprs = (struct ProcessType *)malloc(sizeof(struct ProcessType));
    psprs->nbvar = psprs->nbact = psprs->nbtrans = 0;
    int i;
    for(i=0;i<SIZE;i++)
        psprs->variableList[i];
    for(i=0;i<SIZE;i++)
        psprs->activityList[i];
    for(i=0;i<SIZE;i++)
        psprs->transitionList[i];

    // Initialisation de l'etat de depart
    pstate->state = START;    
}



void finDocument(struct ParserStateType* pstate)
{}

int main(int argc, char *argv[])
{
    struct ParserStateType pstate;

    handler.startDocument = (startDocumentSAXFunc)debutDocument;
    handler.endDocument = (endDocumentSAXFunc)finDocument;
    handler.startElement = (startElementSAXFunc)debutElement;
    handler.endElement = (endElementSAXFunc)finElement;
    handler.characters = (charactersSAXFunc)caracteres;

    if(xmlSAXUserParseFile(&handler, &pstate, "process1.xml") <0) {perror("oups parser");}

    return 0;
}
