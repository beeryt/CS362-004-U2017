
SRCS := dominion rngs
UNIT_TESTS := unittest1.o unittest2.o unittest3.o unittest4.o \
			cardtest1.o cardtest2.o cardtest3.o cardtest4.o
RAND_TESTS := randomtestcard1.o randomtestcard2.o randomtestadventurer.o randomtest_helper.o

UNITY := Unity/src/unity.c Unity/extras/fixture/src/unity_fixture.c
SRC_OBJS := $(SRCS:=.o)
TEST_OBJS := unity.o unity_fixture.o

CC := gcc -std=gnu99
CFLAGS = -g -Wall -fpic -coverage -IUnity/src -IUnity/extras/fixture/src
LDLIBS := -lm

playdom: playdom.o $(SRC_OBJS)
	gcc $(CFLAGS) -o playdom $^ $(LDLIBS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testAll: dominion.o cardEffects.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o cardEffects.o rngs.o $(CFLAGS)

unity.o: Unity/src/unity.c
	$(CC) $(CFLAGS) -c $^

unity_fixture.o: Unity/extras/fixture/src/unity_fixture.c
	$(CC) $(CFLAGS) -c $^

randomtests.out: $(RAND_TESTS) $(TEST_OBJS) $(SRC_OBJS)
	gcc $(CFLAGS) -o randomtestcard1.exe randomtestcard1.o randomtest_helper.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o randomtestcard2.exe randomtestcard2.o randomtest_helper.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o randomtestadventurer.exe randomtestadventurer.o randomtest_helper.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)

	echo "Running Random Tests..." > randomtests.out

	echo "\n Running Random Test 1" >> randomtests.out
	@./randomtestcard1.exe >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out
	@gcov randomtestcard1.c $(SRCS:=.c) > randomtestcard1.tmp

	echo "\n Running Random Test 2" >> randomtests.out
	@./randomtestcard2.exe >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out
	@gcov randomtestcard2.c $(SRCS:=.c) > randomtestcard2.tmp

	echo "\n Running Random Test Adventurer" >> randomtests.out
	@./randomtestadventurer.exe >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out
	@gcov randomtestadventurer.c $(SRCS:=.c) > randomtestadventurer.tmp

	@echo "Coverages" >> randomtests.out
	@cat randomtestcard1.tmp >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out
	@cat randomtestcard2.tmp >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out
	@cat randomtestadventurer.tmp >> randomtests.out
	@echo "\n\n----------------------------------------------\n\n" >> randomtests.out

	@rm *.tmp

unittestresults.out: $(UNIT_TESTS) $(TEST_OBJS) $(SRC_OBJS)
	gcc $(CFLAGS) -o unittest1.exe unittest1.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest2.exe unittest2.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest3.exe unittest3.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest4.exe unittest4.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest1.exe cardtest1.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest2.exe cardtest2.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest3.exe cardtest3.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest4.exe cardtest4.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)

	echo "Running Unit Tests..." > unittestresults.out

	echo "\nRunning Unit Test 1" >> unittestresults.out
	@./unittest1.exe >> unittestresults.out
	@gcov unittest1.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 2" >> unittestresults.out
	@./unittest2.exe >> unittestresults.out
	@gcov unittest2.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 3" >> unittestresults.out
	@./unittest3.exe >> unittestresults.out
	@gcov unittest3.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 4" >> unittestresults.out
	@./unittest4.exe >> unittestresults.out
	@gcov unittest4.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 1" >> unittestresults.out
	@./cardtest1.exe >> unittestresults.out
	@gcov cardtest1.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 2" >> unittestresults.out
	@./cardtest2.exe >> unittestresults.out
	@gcov cardtest2.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 3" >> unittestresults.out
	@./cardtest3.exe >> unittestresults.out
	@gcov cardtest3.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 4" >> unittestresults.out
	@./cardtest4.exe >> unittestresults.out
	@gcov cardtest4.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out


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
	rm -f *.exe *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
