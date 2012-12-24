#
# Makefile pour generer le moteur de workflows
#


#
# Constantes
#

SRC = main.c optionServer.c gestion_client.c
OBJS = $(SRC:.c=.o)
LIBS = -lsck -lthrd -lpthread
LIB_DIR = -L ../Socket -L ../Threads
INC_DIR = -I ../Socket -I ../Threads
CFLAGS += $(INC_DIR)

#
# Cible principale
#

all: main

#
# Cible debug
#

debug:	CFLAGS += $(CDEBUG)
debug: all


#
# Cible de nettoyage
#

clean:
	@echo
	@echo Debut du nettoyage Workflow_engine
	rm -f core *.o main
	@echo Debut du nettoyage Workflow_engine
	@echo


# 
# Executable target
#

main: $(OBJS)
	@echo ===== MAIN =====
	$(CC) -o main $(OBJS) $(LIB_DIR) $(LIBS)
	@echo ================

main.o: main.c optionServer.h
optionServer.o: optionServer.c optionServer.h
gestion_client.o: gestion_client.c gestion_client.h
