#
# Makefile pour generer la bibliotheque de fonctions reseau
#

#
# Constantes
#

SRC=libsck.c
OBJ=$(SRC:.c=.o)
LIB_NAME=libsck.a


#
# Cible principale
#

all: $(LIB_NAME)


#
# Cible debug
#

#debug: $(CFLAGS) += $(CDEBUG)
#debug: all


#
# Cible de nettoyage
#

clean:
	@echo
	@echo Debut du nettoyage Socket
	rm -f core *.o $(LIB_NAME)
	@echo Fin du nettoyage Socket
	@echo

#
# Dependance
#

libsck.o: $(SRC) libsck.h

$(LIB_NAME): $(OBJ)
	@echo ==== LIBSCK ====
	$(AR) $(LIB_NAME) $(OBJ)
	@echo ================
