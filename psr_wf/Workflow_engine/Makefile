#
# Makefile pour generer le moteur de workflows
#


#
# Constantes
#

SRC = main.c optionServer.c gestion_client.c parser.c workflow_struct.c
OBJS = $(SRC:.c=.o)
LIBS = -lsck -lthrd -lpthread -lxml2
LIB_DIR = -L ../Socket -L ../Threads -L/usr/include/libxml2/
INC_DIR = -I ../Socket -I ../Threads -I /usr/include/libxml2/
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

main.o: main.c optionServer.h workflow_struct.h
optionServer.o: optionServer.c optionServer.h
gestion_client.o: gestion_client.c gestion_client.h workflow_struct.h
parser.o: parser.c parser.h
workflow_struct.o: workflow_struct.c workflow_struct.h parser.h
