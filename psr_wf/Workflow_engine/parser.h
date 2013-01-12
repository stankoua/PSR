#define SIZE            20
#define LONGSIZE        200
#define SMALLSIZE       10
#define SMALLEST_SIZE   4


/**
 * Types pour le parseur
 */

enum PossibleParserStates {START,PROCESS,DESCRIPTION,VARIABLE_LIST,VARIABLE,
                           ACTIVITY_LIST,ACTIVITY,ACT_DESCRIPTION,ACT_PERFORMER,
                           ACT_INPUT,ACT_OUTPUT,TRANSITION_LIST,TRANSITION,TRANS_CONDITION,
                           FINISH};

struct ParserStateType
{
    struct ProcessType* process;
    char buffer[LONGSIZE];            		// buffer pour les valeurs lues
    char bufatr[SMALLEST_SIZE][SIZE];             		// buffer pour les attributs
    int nbatr;
    enum PossibleParserStates state;
};

// START 0  PROCESS 1  DESCRIPTION 2  VARIABLE_LIST 3 VARIABLE 4  
// ACTIVITY_LIST 5  ACTIVITY 6  ACT_DESCRIPTION 7
// ACT_PERFORMER 8  ACT_INPUT 9  ACT_OUTPUT 10  TRANSITION_LIST 11  
// TRANSITION 12  TRANS_CONDITION 13  FINISH 14


/**
 * Fonctions
 */
struct ProcessType* parseFile(char* name);
