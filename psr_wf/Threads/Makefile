#
# Makefile pour generer la bibliotheque de gestion de thread
#

#
# Constantes
#

SRC=libthrd.c
OBJ=$(SRC:.c=.o)
LIB_NAME=libthrd.a


#
# Cible principale
#

all: $(LIB_NAME)


#
# Cible debug
#

debug:


#
# Cible de nettoyage
#

clean:
	@echo
	@echo Debut du nettoyage Socket
	rm -f *.o $(LIB_NAME)
	@echo Fin du nettoyage Socket
	@echo


#
# Dependance
#

libthrd.o: $(SRC) libthrd.h

$(LIB_NAME): $(OBJ)
	@echo ==== LIBTHRD ====
	$(AR) $(LIB_NAME) $(OBJ)
	@echo ================
