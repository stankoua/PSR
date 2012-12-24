#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export AR = ar crus
export CDEBUG = -g -DDEBUG
MAKE = make 
export CFLAGS = -Wall

#
# Repertoires du projet
#

#DIRS = IPC Makefile Models Socket Threads Workflow_admin Workflow_engine
DIRS = Socket Threads Workflow_engine

#
# Cible generale
#
all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && $(MAKE)
	
#
# Cible debug
#
debug:	$(CFLAGS) += $(CDEBUG)
debug: all


#
# Cible de nettoyage
#

clean:
	@echo
	@echo Debut du nettoyage
	for dir in $(DIRS); do\
		$(MAKE) clean -C $$dir;\
	done
	@echo Fin du nettoyage
	@echo
