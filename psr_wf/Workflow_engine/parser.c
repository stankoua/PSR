#include <string.h>
#include <libxml/parser.h>

#include "parser.h"
#include "workflow_struct.h"


static xmlSAXHandler handler;


/**
 *  Un char a ete lu
 */
void caracteres(struct ParserStateType* pstate, const xmlChar *text, int length)
{
    char *tmp = NULL;
    char str[LONGSIZE];          // Utile pour strtok
    strncpy(str,text,LONGSIZE);
#ifdef DDEBUG
    printf("etat actuel character # %d #\n",pstate->state);
#endif
    switch(pstate->state)
    {
        case DESCRIPTION:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
            break;

        case ACT_DESCRIPTION:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
            break;

        case ACT_PERFORMER:
            tmp = strtok( str, "<" );
            tmp[strlen(tmp)] = '\0';
            strcpy( pstate->buffer, tmp );
            break;

        default:
            // Ne nous interesse pas, donc on affiche pas
            break;
    }
#ifdef DDEBUG
    printf( "==============TEST Chaine Extraite: %s\n\n",pstate->buffer );
#endif
}


/**
 *  Une balise fermante a été lue
 */
void finElement(struct ParserStateType* pstate, const xmlChar *name)
{
    int i;
#ifdef DDEBUG
    printf("fin de l'element:>>%s<<:\n",(char *)name);
    printf("etat actuel finE # %d #\n",pstate->state);
#endif
    switch(pstate->state)
    {
        case PROCESS:
            if(!strcmp(name,"tns:process")) {
                // Fin de fichier atteint
                pstate->state = FINISH;
                printf("======== fin fichier \n\n");
            }else 
                { printf("Erreur element %s\n",name); exit(-1); }            
            break;

        case DESCRIPTION:
            if(!strcmp(name,"tns:description")) {
                strcpy(pstate->process->description,pstate->buffer);
                // On enregistre le nom du process en meme tps que la description
                // Pour eviter ne pas le faire en fin de fichier
                strcpy(pstate->process->name,pstate->bufatr[0]);
                pstate->state = PROCESS;
#ifdef DDEBUG
                printf("========TEST Description du process: %s\n\n",pstate->process->description);
                printf("======== Nom du process-- %s --\n\n",pstate->process->name);
#endif      
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
                strcpy(pstate->process->variableList[i].name,pstate->bufatr[0]);
                strcpy(pstate->process->variableList[i].type,pstate->bufatr[1]);
                if(pstate->nbatr == 3)
                    { strcpy(pstate->process->variableList[i].initialvalue,pstate->bufatr[2]); }
                
                pstate->process->nbvar += 1;
                pstate->state = VARIABLE_LIST;
#ifdef DDEBUG
                printf("======== test nbre variable: %d\n\n",pstate->process->nbvar);
                printf("======== test attribut variable: %s\n\n",pstate->bufatr[0]);
                printf("========TEST variable process nom: %s\n\n",pstate->process->variableList[i].name);
                printf("======== test attribut variable: %s\n\n",pstate->bufatr[1]);
                printf("========TEST variable process type: %s\n\n",pstate->process->variableList[i].type);
                printf("========TEST variable process init value: %s\n\n",pstate->process->variableList[i].initialvalue);
#endif             
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
                strcpy(pstate->process->activityList[i].name,pstate->bufatr[1]);
                // Ensuite la description
                strcpy(pstate->process->activityList[i].description,pstate->buffer);
                // L'activité n'a pas encore commencé
                pstate->process->activityList[i].state = NOT_STARTED;

                // Fini 
                pstate->state = ACTIVITY;
#ifdef DDEBUG
                printf("========TEST activity process nom: %s\n\n",pstate->process->activityList[i].id);
                printf("========TEST activity process type: %s\n\n",pstate->process->activityList[i].name);
                printf("========TEST activity process description: %s\n\n",pstate->process->activityList[i].description);
#endif            
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_PERFORMER:
            if(!strcmp(name,"tns:performer")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].performer,pstate->buffer);
                pstate->state = ACTIVITY;
#ifdef DDEBUG
                printf("========TEST activity process performer: %s\n\n",pstate->process->activityList[i].performer);
#endif        
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_INPUT:
            if(!strcmp(name,"tns:input")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].input,pstate->bufatr[0]);
                pstate->state = ACTIVITY;
#ifdef DDEBUG
                printf("========TEST activity process input: %s\n\n",pstate->process->activityList[i].input);
#endif        
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_OUTPUT:
            if(!strcmp(name,"tns:output")) {
                i = pstate->process->nbact;
                strcpy(pstate->process->activityList[i].output,pstate->bufatr[0]);
                pstate->state = ACTIVITY;
#ifdef DDEBUG
                printf("========TEST activity process output: %s\n\n",pstate->process->activityList[i].output);
#endif        
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
                strcpy(pstate->process->transitionList[i].from,pstate->bufatr[1]);
                strcpy(pstate->process->transitionList[i].to,pstate->bufatr[2]);
                pstate->state = TRANSITION_LIST;
#ifdef DDEBUG
                printf("========TEST transition process id: %s\n\n",pstate->process->transitionList[i].id);
                printf("========TEST transition process from: %s\n\n",pstate->process->transitionList[i].from);
                printf("========TEST transition process to: %s\n\n",pstate->process->transitionList[i].to);
#endif             
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
                pstate->state = TRANSITION;
#ifdef DDEBUG
                printf("========TEST transition process condition: %s\n\n",pstate->process->transitionList[i].condition);
#endif        
            }else 
                { printf("Erreur element %s",name); exit(-1); }
            break;

        default:
            // ERREUR
            printf("Erreur fin element: %s\n",name);
            exit(-1);
            break;
    }
#ifdef DDEBUG
    printf("nouvel etat finE # %d #\n",pstate->state);
#endif
}


/**
 *  Une balise ouvrante a été lue
 *  
 */
void debutElement(struct ParserStateType* pstate, const xmlChar *name, const xmlChar **attrs)
{
    int i,j;
    char* tmp = NULL;
#ifdef DDEBUG
    printf("Debut de l'element:>>%s<<:\n", name);
    printf("etat actuel debE # %d #\n",pstate->state);
#endif
    switch(pstate->state)
    {
        case START:
            if(!strcmp(name,"tns:process")) {
                pstate->state = PROCESS;
                /* Lecture des attributs
                 * Un attribut: name
                 */
                i = 0;
                while(attrs[i]!=NULL && strcmp(attrs[i],"xmlns:name")!=0)     
                    i += 2;

                if(attrs[i]!=NULL)
                    strcpy(pstate->bufatr[0], attrs[i+1]);
                pstate->nbatr += 1;
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
#ifdef DDEBUG
                    printf("======== test attribut variable %d: %s\n\n",j,pstate->bufatr[j]);
#endif
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
#ifdef DDEBUG
                    printf("======== test attribut variable %d: %s\n\n",j,pstate->bufatr[j]);
#endif
                    i += 2; j += 1;
                }
                pstate->nbatr = j;
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case TRANSITION:
            if(!strcmp(name,"tns:condition")) {
                pstate->state = TRANS_CONDITION;
#ifdef DDEBUG
                printf("================TEST etat # %d #\n",pstate->state);
#endif
                pstate->state = 13;
                // On ne fait pas comme d'habitude
                // On copie l'attribut à la fin du tableau
                j = pstate->nbatr;
                strcpy(pstate->bufatr[j], attrs[0]);
                pstate->nbatr += 1;
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case FINISH:
            exit(-1); 
            break;
        
        default:
            // ERREUR
            // balise ne contenant pas d'autre balises
            // On ne change pas l'état courant
            // cas DESCRIPTION, TRANS_CONDITION,FINISH,VARIABLE
            // ACT_DESCRIPTION,ACT_PERFORMER,ACT_INPUT,ACT_OUTPUT
            break;
    }
#ifdef DDEBUG
    printf("nouvel etat debE # %d #\n",pstate->state);    
#endif 
}


void debutDocument(struct ParserStateType* pstate)
{
    // Initialisation de process et de ses attributs
    pstate->process = (struct ProcessType *)malloc(sizeof(struct ProcessType));
    pstate->process->nbvar = pstate->process->nbact = pstate->process->nbtrans = 0;

    // Initialisation de l'etat de depart
#ifdef DDEBUG
    printf("================ INTITIALISATION ==========================\n");
#endif
    pstate->state = START;    
}


void finDocument(struct ParserStateType* pstate)
{
  pstate->process->etat = RUNNING;
}


/**
 * Fonction qui parse le fichier
 * => valeur de retour: le ptr sur le process
 *    
 */

struct ProcessType* parseFile(char* name)
{
  //xmlSAXHandler handler;
  struct ParserStateType pstate;

  handler.startDocument = (startDocumentSAXFunc)debutDocument;
  handler.startElement = (startElementSAXFunc)debutElement;
  handler.endElement = (endElementSAXFunc)finElement;
  handler.characters = (charactersSAXFunc)caracteres;

  if(xmlSAXUserParseFile(&handler, &pstate, name) <0) {perror("oups parser"); return NULL;}

  printf("======== FIN fonction parsefile \n\n");
  //printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<FINISH>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"); 
  return pstate.process;
}
