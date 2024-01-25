CC=clang -Wall

PROGRAMMES=test_terrain test_robot robot_terrain curiosity test_e2 test_generation_terrains curiosity-perf curiosity-obs curiosity-obs2

all: $(PROGRAMMES)

######################################################################
#                       Règles de compilation                        #
######################################################################

%.o: %.c
	$(CC) -c $<

generation_terrains.o: generation_terrains.c generation_terrains.h terrain.h

test_generation_terrains.o: test_generation_terrains.c generation_terrains.h

test_terrain.o: test_terrain.c terrain.h

test_robot.o: test_robot.c robot.h

robot_terrain.o: robot_terrain.c terrain.h robot.h

robot.o: robot.c robot.h

terrain.o: terrain.c terrain.h

environnement.o: environnement.c environnement.h robot.h terrain.h

programme.o: programme.c programme.h type_pile.h

interprete.o: interprete.c interprete.h environnement3.h \
	programme.h type_pile.h robot.h terrain.h

interprete%.o: interprete%.c interprete.h environnement.h \
	programme.h type_pile.h robot.h terrain.h

type_pile.o: type_pile.c type_pile.h

test_e2.o: test_e2.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

curiosity.o: curiosity.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

curiosity-test.o: curiosity-test.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

curiosity-perf.o: curiosity-perf.c generation_terrains.o generation_terrains.h \
    terrain.h terrain.o environnement.h interprete.h robot.h type_pile.h programme.h

environnement2.o: environnement2.c environnement2.h robot.h terrain.h observateur.h

environnement3.o: environnement3.c environnement2.h robot.h terrain.h observateur.h

observateur.o: observateur.c observateur.h

curiosity-obs.o: environnement2.h programme.h \
    interprete.h robot.h terrain.h type_pile.h 

curiosity-obs2.o: environnement3.h programme.h \
    interprete.h robot.h terrain.h type_pile.h 

######################################################################
#                       Règles d'édition de liens                    #
######################################################################

test_generation_terrains: test_generation_terrains.o generation_terrains.o terrain.o
	$(CC) $^ -o $@

test_terrain: test_terrain.o terrain.o
	$(CC) $^ -o $@

test_robot: test_robot.o robot.o
	$(CC) $^ -o $@

robot_terrain: robot_terrain.o terrain.o robot.o
	$(CC) $^ -o $@

curiosity: curiosity.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

test_e2: test_e2.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-test: curiosity-test.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-test%: curiosity-test.o environnement.o programme.o interprete%.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-perf: curiosity-perf.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o generation_terrains.o
	$(CC) $^ -o $@

curiosity-obs: curiosity-obs.o environnement2.o observateur.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-obs2: curiosity-obs2.o environnement3.o observateur.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

clean:
	rm -f $(PROGRAMMES) *.o
