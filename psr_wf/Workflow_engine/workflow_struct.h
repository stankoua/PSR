#define SIZE            20
#define LONGSIZE        200
#define SMALLSIZE       10
#define SMALLEST_SIZE   4


/**
 * Types pour le process
 */

struct UserType
{
  int socket;
  char login[SMALLSIZE];
  char nom[SMALLSIZE];
  char prenom[SMALLSIZE];
};

struct TabUsersType				// Type Table d'utilisateur
{
  struct UserType* tab_users[SMALLSIZE];
  int nbusers;
};

/**
 * Types pour le process
 */
 
struct VariableType
{
    char name[SIZE];                            // Attribut
    char type[SIZE];                            // Attribut
    char initialvalue[SIZE];                    // Attribut
};

enum State {NOT_STARTED,RUNNING,COMPLETED};

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

// Comment: implementer le type "typeS"
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
    enum State etat;				// Pas dans le xml
    struct VariableType variableList[SIZE];
    int nbvar;
    struct ActivityType activityList[SIZE];
    int nbact;
    struct TransitionType transitionList[SIZE];
    int nbtrans;
};

struct TabProcessType				// Type Table de process
{
  struct ProcessType* tab_process[SMALLSIZE];
  int nbprocess;
};


/**
 * Variables 
 */

struct TabProcessType t_process;	// Table des instances de process
struct TabUsersType t_users;		// Table des utilisateurs connectÃ©s


/**
 * Fonctions
 */

void initTabProcess(struct TabProcessType* t_process);

void initTabUsers(struct TabUsersType* t_users);

int addProcess(struct TabProcessType* t_process, char* fichier);

int addUser(struct TabUsersType* t_users, int socket, char* login, char* nom, char* prenom);

void freeProcess(struct TabProcessType* t_process);

void freeUsers(struct TabUsersType* t_users, int i);

