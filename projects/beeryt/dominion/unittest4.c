#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(isGameOver);
static struct gameState *G;
static int k_cards[10] = {adventurer, gardens,  embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
static int seed = 42;


// Test Setup and Teardown
TEST_SETUP(isGameOver) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
}
TEST_TEAR_DOWN(isGameOver) {
	free(G);
}

// Individual Unit Tests

// Tests that isGameOver fails on newgame
TEST(isGameOver, newGame)
{
    TEST_ASSERT_EQUAL(0, isGameOver(G));
}

// Tests that isGameOver succeeds when there are no provinces
TEST(isGameOver, noProvinces)
{
    G->supplyCount[province] = 0;
    TEST_ASSERT_EQUAL(1, isGameOver(G));
}

TEST(isGameOver, supplyDown3)
{
    G->supplyCount[k_cards[0]] = 0;
    G->supplyCount[k_cards[1]] = 0;
    G->supplyCount[k_cards[2]] = 0;
    TEST_ASSERT_EQUAL(1, isGameOver(G));
}

TEST(isGameOver, supplyDownEdgeCase)
{
    G->supplyCount[treasure_map] = 0; //top most card in supply
    G->supplyCount[curse] = 0; // bottom most card in supply
    G->supplyCount[estate] = 0; // any other card
    TEST_ASSERT_EQUAL(1, isGameOver(G));
}

TEST(isGameOver, noCoins)
{
    G->supplyCount[copper] = 0;
    G->supplyCount[silver] = 0;
    G->supplyCount[gold] = 0;
    TEST_ASSERT_EQUAL(1, isGameOver(G));
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(isGameOver);
static void RunAllTests(void)
{
	RUN_TEST_CASE(isGameOver, newGame);
	RUN_TEST_CASE(isGameOver, noProvinces);
	RUN_TEST_CASE(isGameOver, supplyDown3);
	RUN_TEST_CASE(isGameOver, supplyDownEdgeCase);
	RUN_TEST_CASE(isGameOver, noCoins);
}
int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
