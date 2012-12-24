#define SIZE 20


struct VariableType
{
    char name[SIZE];
    char type[SIZE];
    char initialvalue[SIZE];
};

struct IOtype
{
    char name[SIZE];
};

struct ActivityType
{
    char id[SIZE];
    char name[SIZE];
    char description[SIZE];
    char performer[SIZE];
    struct IOtype input[SIZE];
    struct IOtype ouput[SIZE];
};

struct TransitionType
{
    char to[SIZE];
    char id[SIZE];
    char from[SIZE];
    char condition[SIZE];
};

struct ProcessType
{
    char description[SIZE];
    struct VariableType variableList[SIZE];
    struct ActivityType activityList[SIZE];
    struct TransitionType transitionList[SIZE];
};

enum State {process,variableList,variable,activityList,activite,transitionList,transition};
