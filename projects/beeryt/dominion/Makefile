CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c cardEffects.h cardEffects.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o cardEffects.o playdom.c
	gcc -o playdom playdom.c -g dominion.o cardEffects.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testAll: dominion.o cardEffects.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o cardEffects.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o cardEffects.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
