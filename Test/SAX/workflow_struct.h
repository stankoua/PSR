#define SIZE 20
#define LONGSIZE 200
#define SMALLSIZE 10


struct VariableType
{
    char name[SIZE];                            // Attribut
    char type[SIZE];                            // Attribut
    char initialvalue[SIZE];                    // Attribut
};

enum State {NOT_STARTED,RUNNING,COMPLETE};

struct ActivityType
{
    char id[SIZE];                              // Attribut
    char name[SIZE];                            // Attribut
    char description[LONGSIZE];
    char performer[SIZE];
    enum State state;
    char input[SIZE];
    char output[SIZE];
    //char input[SIZE][SIZE];
    //char ouput[SIZE][SIZE];
};

struct TransitionType
{
    char id[SIZE];                              // Attribute
    char from[SIZE];                            // Attribute
    char to[SIZE];                              // Attribute
    char condition[SIZE];
};

struct ProcessType
{
    char name[SIZE];                            // Attribute
    char description[SIZE];
    struct VariableType variableList[SIZE];
    int nbvar;
    struct ActivityType activityList[SIZE];
    int nbact;
    struct TransitionType transitionList[SIZE];
    int nbtrans;
};

enum PossibleParserStates {START,PROCESS,DESCRIPTION,VARIABLE_LIST,VARIABLE,ACTIVITY_LIST,ACTIVITY,ACT_DESCRIPTION,ACT_PERFORMER,ACT_INPUT,ACT_OUTPUT,TRANSITION_LIST,TRANSITION,TRANS_CONDITION,FINISH};

struct ParserStateType
{
    struct ProcessType* process;
    char buffer[LONGSIZE];            // buffer pour les valeurs lues
    char bufatr[4][SIZE];             // buffer pour les attributs
    int nbatr;
    enum PossibleParserStates state;
};

// START 0
// PROCESS 1
// DESCRIPTION 2
// VARIABLE_LIST 3
// VARIABLE 4
// ACTIVITY_LIST 5
// ACTIVITY 6
// ACT_DESCRIPTION 7
// ACT_PERFORMER 8
// ACT_INPUT 9
// ACT_OUTPUT 10
// TRANSITION_LIST 11
// TRANSITION 12
// TRANS_CONDITION 13
// FINISH 14
