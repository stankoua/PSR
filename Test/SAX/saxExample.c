#include <string.h>
#include <libxml/parser.h>

#include "workflow_struct.h"

#define psprs pstate->process


static xmlSAXHandler handler;

//static int elem_courant;


/**
 *  Un char a ete lu
 */
void caracteres(struct ParserStateType* pstate, const xmlChar *text, int length)
{
    printf("etat actuel character # %d #\n",pstate->state);
    int p = pstate->state;
    switch(p)
    {
        case DESCRIPTION:
            strcpy(pstate->buffer, text); 
            printf("caracteres:text:%s:\n",(char *)text);
            break;

        case ACT_DESCRIPTION:
            strcpy(pstate->buffer, text); 
            printf("caracteres:text:%s:\n",(char *)text);
            break;

        case ACT_PERFORMER:
            strcpy(pstate->buffer, text); 
            printf("caracteres:text:%s:\n",(char *)text);
            break;

        case TRANS_CONDITION:
            strcpy(pstate->buffer, text); 
            printf("caracteres:text:%s:\n",(char *)text);
            break;

        default:
            // ERREUR
            //printf("Erreur character: %s\n",text);
            //exit(-1);
            break;
    }
    pstate->state = p;          /* TROP BIZARRE */ // ajout du 21/12/2012
}


/**
 *  Une balise fermante a été lue
 */
void finElement(struct ParserStateType* pstate, const xmlChar *name)
{
    printf("fin de l'element:>>%s<<\n",(char *)name);
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
                pstate->state = 1;
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
                int i = pstate->process->nbvar;
                strcpy(pstate->process->variableList[i-1].name,pstate->bufatr[0]);
                strcpy(pstate->process->variableList[i-1].type,pstate->bufatr[1]);
                strcpy(pstate->process->variableList[i-1].initialvalue,pstate->bufatr[2]);
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
                int i = pstate->process->nbact;
                //strcpy(pstate->process->activityList[i-1].id,pstate->bufatr[0]);
                //strcpy(pstate->process->activityList[i-1].name,pstate->bufatr[1]);
                pstate->process->activityList[i-1].state = NOT_STARTED;

                pstate->process->nbact += 1; 
                pstate->state = ACTIVITY_LIST;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_DESCRIPTION:
            if(!strcmp(name,"tns:description")) {
                int i = pstate->process->nbact;
                //strcpy(pstate->process->activityList[i-1].description,pstate->buffer);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_PERFORMER:
            if(!strcmp(name,"tns:performer")) {
                int i = pstate->process->nbact;
                //strcpy(pstate->process->activityList[i-1].performer,pstate->buffer);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_INPUT:
            if(!strcmp(name,"tns:input")) {
                int i = pstate->process->nbact;
                //strcpy(pstate->process->activityList[i-1].input,pstate->buffer);
                pstate->state = ACTIVITY;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case ACT_OUTPUT:
            if(!strcmp(name,"tns:output")) {
                int i = pstate->process->nbact;
                //strcpy(pstate->process->activityList[i-1].output,pstate->buffer);
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
                int i = pstate->process->nbtrans;
                strcpy(pstate->process->transitionList[i-1].to,pstate->bufatr[0]);
                strcpy(pstate->process->transitionList[i-1].id,pstate->bufatr[1]);
                strcpy(pstate->process->transitionList[i-1].from,pstate->bufatr[2]);

                pstate->process->nbtrans += 1;
                pstate->state = TRANSITION_LIST;
            }else 
                { printf("Erreur element %s",name); exit(-1); }            
            break;

        case TRANS_CONDITION:
            if(!strcmp(name,"tns:condition")) {
                int i = pstate->process->nbtrans;
                //strcpy(pstate->process->transitionList[i-1].condition,pstate->buffer);
                // BUG ICI

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
    int i;
    printf("etat actuel debE # %d #\n",pstate->state);
    switch(pstate->state)
    {
        case START:
            if(!strcmp(name,"tns:process"))
            {
                pstate->state = PROCESS;
                printf("nouvel etat debE # %d #\n",pstate->state);
               
                // Lecture des attributs
                // Un attribut: name
                i = 0;
                pstate->bufatr[0] = (char*)malloc(SIZE*sizeof(char));
                strcpy(pstate->bufatr[i],attrs[i]);
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n", name);
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case PROCESS:
            if(!strcmp(name,"tns:description"))
            {
                pstate->state = DESCRIPTION;
                printf("nouvel etat debE # %d #\n",pstate->state);

                printf("Debut de l'element:%s:\n", name);
            }else if(!strcmp(name,"tns:variableList")){
                pstate->state = VARIABLE_LIST;
                printf("Debut de l'element:%s:\n",name);
            }else if(!strcmp(name,"tns:activityList")){
                pstate->state = ACTIVITY_LIST;
                printf("Debut de l'element:%s:\n",name);
            }else if(!strcmp(name,"tns:transitionList")){
                pstate->state = TRANSITION_LIST;
                printf("Debut de l'element:%s:\n",name);
            }else// ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case VARIABLE_LIST:
            if(!strcmp(name,"tns:variable"))
            {
                pstate->state = VARIABLE;
                printf("nouvel etat# %d #\n",pstate->state);

                // Lecture des attributs
                // 3 attributs: name, type, initialValue
                i = 0;
                while(attrs[i]!=NULL)
                {
                    pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                    strcpy(pstate->bufatr[i], attrs[i]);
                    i += 1;
                }
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n", name);
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }  
            break;

        case ACTIVITY_LIST:
            if(!strcmp(name,"tns:activity"))
            {
                pstate->state = ACTIVITY;
                // Lecture des attributs
                // 2 attributs: id, name
                i = 0;
                while(attrs[i]!=NULL)
                {
                    pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                    strcpy(pstate->bufatr[i], attrs[i]); 
                    i += 1;
                }
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n", name);
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case ACTIVITY:
            if(!strcmp(name,"tns:description")) {
                pstate->state = ACT_DESCRIPTION;
                printf("Debut de l'element:%s:\n", name);
            } else if(!strcmp(name,"tns:performer")) {
                pstate->state = ACT_PERFORMER;
                printf("Debut de l'element:%s:\n",name);
            } else if(!strcmp(name,"tns:input")) {
                pstate->state = ACT_INPUT;
                // Lecture des attributs
                // 1 attribut: nom
                i = 0;
                pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                strcpy(pstate->bufatr[i], attrs[i]); 
                
                printf("Debut de l'element:%s:\n",name);
            } else if(!strcmp(name,"tns:output")) {
                pstate->state = ACT_OUTPUT;
                // Lecture des attributs
                i = 0;
                pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                strcpy(pstate->bufatr[i], attrs[i]); 
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n",name);
            } else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case TRANSITION_LIST:
            if(!strcmp(name,"tns:transition"))
            {
                pstate->state = TRANSITION;
                // Lecture des attributs
                // 3 attributs id, to, from
                i = 0;
                while(attrs[i]!=NULL)
                {
                    pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                    strcpy(pstate->bufatr[i], attrs[i]); 
                    i += 1;
                }
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n", name);
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case TRANSITION:
            if(!strcmp(name,"tns:condition"))
            {
                pstate->state = TRANS_CONDITION;
                // Lecture des attributs
                // 1 attribut expr
                i = 0;
                pstate->bufatr[i] = (char*)malloc(SIZE*sizeof(char*));
                strcpy(pstate->bufatr[i], attrs[i]); 
                pstate->nbatr = i;

                printf("Debut de l'element:%s:\n", name);
            }else // ERREUR
                { printf("Erreur element %s",name); exit(-1); }
            break;

        case FINISH:
            // ERREUR
            //printf("Erreur element %s",name); 
            printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<FINISHH>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"); 
            exit(-1); 
            break;
        
        default:
            // ERREUR
            // balise de l'information
            // D'autres balises ne peuvent pas être lues à l'interieur
            // cas DESCRIPTION, TRANS_CONDITION,FINISH,VARIABLE
            // ACT_DESCRIPTION,ACT_PERFORMER,ACT_INPUT,ACT_OUTPUT
            //printf("Erreur %s \n",name); 
            exit(-1); 
            break;
    }
    
}


//void init_tab(void* tab, int size)
//{
//    int i;
//    for(i=0;i<size;i++)
//        tab[i] = NULL;
//}

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
    //pstate->state = START;    
}

void finDocument(struct ParserStateType* pstate)
{
    int i=0;
 //   while(pstate->bufatr[i]!=NULL)
 //   {
 //       free(pstate->bufatr[i]);
 //       i += 1;
 //   }
}

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
