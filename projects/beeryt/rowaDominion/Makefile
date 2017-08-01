CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion_helpers.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	
unittestsmithy.out: cardtest1
	./cardtest1 >> unittestsmithy.out
	echo "-------------------------------------------------------------------" >> unittestsmithy.out
	gcov -b -c dominion.c >> unittestsmithy.out
	cat dominion.c.gcov >> unittestsmithy.out

unittestadventurer.out: cardtest2
	./cardtest2 >> unittestadventurer.out
	echo "-------------------------------------------------------------------" >> unittestadventurer.out
	gcov -b -c dominion.c >> unittestadventurer.out
	cat dominion.c.gcov >> unittestadventurer.out

unittestresults.out: dominion.o unittests cardtests
	./unittest1 &> unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out
	echo "-------------------------------------------------------------------" >> unittestresults.out
	./cardtest1 >> unittestresults.out
	./cardtest2 >> unittestresults.out
	./cardtest3 >> unittestresults.out
	./cardtest4 >> unittestresults.out
	echo "-------------------------------------------------------------------" >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g dominion.o rngs.o interface.o $(CFLAGS)

testing_helpers.o: testing_helpers.c testing_helpers.h
	gcc -c testing_helpers.c -g $(CFLAGS)

unittest1: unittest1.c testing_helpers.o dominion.o rngs.o
	gcc -o unittest1 unittest1.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

unittest2: unittest2.c testing_helpers.o dominion.o rngs.o
	gcc -o unittest2 unittest2.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

unittest3: unittest3.c testing_helpers.o dominion.o rngs.o
	gcc -o unittest3 unittest3.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

unittest4: unittest4.c testing_helpers.o dominion.o rngs.o
	gcc -o unittest4 unittest4.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

unittests: unittest1 unittest2 unittest3 unittest4

cardtest1: cardtest1.c testing_helpers.o dominion.o rngs.o
	gcc -o cardtest1 cardtest1.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

cardtest2: cardtest2.c testing_helpers.o dominion.o rngs.o
	gcc -o cardtest2 cardtest2.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

cardtest3: cardtest3.c testing_helpers.o dominion.o rngs.o
	gcc -o cardtest3 cardtest3.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

cardtest4: cardtest4.c testing_helpers.o dominion.o rngs.o
	gcc -o cardtest4 cardtest4.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS) 

cardtests: cardtest1 cardtest2 cardtest3 cardtest4

randomtestadventurer: randomtestadventurer.c dominion.o testing_helpers.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o testing_helpers.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o testing_helpers.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c -g testing_helpers.o dominion.o rngs.o $(CFLAGS)

randomtests: randomtestadventurer randomtestcard1 randomtestcard2

randomtestadventurer.out: randomtestadventurer
	./randomtestadventurer >> randomtestadventurer.out
	echo "-------------------------------------------------------------------" >> randomtestadventurer.out
	gcov -b -c dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard1.out: randomtestcard1
	./randomtestcard1 >> randomtestcard1.out
	echo "-------------------------------------------------------------------" >> randomtestcard1.out
	gcov -b -c dominion.c >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out

randomtestcard2.out: randomtestcard2
	./randomtestcard2 >> randomtestcard2.out
	echo "-------------------------------------------------------------------" >> randomtestcard2.out
	gcov -b -c dominion.c >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out

all: playdom player testDrawCard testBuyCard badTestDrawCard cardtests unittests unittestresults.out randomtestadventurer randomtestcard1 randomtestcard2

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out badTestDrawCard testDrawCard testDrawCard.exe cardtest1 cardtest2 cardtest3 cardtest4 unittest1 unittest2 unittest3 unittest4 randomtestadventurer randomtestcard1 randomtestcard2
