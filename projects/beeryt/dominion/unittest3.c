#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(buyCard);
static struct gameState *G;
static int k_cards[10] = {adventurer, gardens,  embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
static int seed = 42;

int buyCard(int supplyPos, struct gameState *state);
/* Buy card with supply index supplyPos */

// Test Setup and Teardown
TEST_SETUP(buyCard) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
}
TEST_TEAR_DOWN(buyCard) {
	free(G);
}

// Individual Unit Tests

// Tests that buyCard failes given invalid player counts
TEST(buyCard, notInGame)
{
	G->coins = 8;
	int ret = buyCard(outpost, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

// Tests that buyCard succeeds and properly sets numPlayers
TEST(buyCard, tooExpensive)
{
	G->coins = 7;
	int ret = buyCard(province, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

// Tests for correct cards in supply after buyCard
TEST(buyCard, noBuysLeft)
{
	G->numBuys = 0;
	int ret = buyCard(copper, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

// Tests for correct cards in players' decks after buyCard
TEST(buyCard, noCardsLeft)
{
	G->coins = 8;
	G->supplyCount[adventurer] = 0;
	int ret = buyCard(adventurer, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

TEST(buyCard, supplyCountDecreases)
{
	int expected = G->supplyCount[copper] - 1;
	buyCard(copper, G);
	int actual = G->supplyCount[copper];
	TEST_ASSERT_EQUAL(expected, actual);
}

// Tests that buyCard fails if a kingdom card is reapeated
TEST(buyCard, cardEnteredDiscard)
{
	int p = G->whoseTurn;
	int originalCount = 0; // sum of discard
	for (size_t i = 0; i < G->discardCount[p]; i++) {
		if (G->discard[p][i] == copper) originalCount++;
	}

	buyCard(copper, G);

	int copperCount = 0; // sum of discard
	for (size_t i = 0; i < G->discardCount[p]; i++) {
		if (G->discard[p][i] == copper) copperCount++;
	}

	TEST_ASSERT_EQUAL(originalCount + 1, copperCount);
}

// Test that buyCard fails if a non-kingdom card is provided
TEST(buyCard, victoryCardIncreaseScore)
{
	int p = G->whoseTurn;
	int originalScore = scoreFor(p, G);

	G->coins = 8;
	buyCard(province, G);

	int actualScore = scoreFor(p, G);

	TEST_ASSERT_EQUAL(originalScore + 6, actualScore);
}

// Tests that subsequent calls to buyCard with the same seed match
TEST(buyCard, multipleBuys)
{
	int p = G->whoseTurn;
	int originalCount = fullDeckCount(p, smithy, G);

	G->coins = 8;
	G->numBuys = 2;
	buyCard(smithy, G);
	buyCard(smithy, G);

	int actualCount = fullDeckCount(p, smithy, G);

	TEST_ASSERT_EQUAL(originalCount + 2, actualCount);
}

// Tests that subsequent calls to buyCard with different seeds differ
TEST(buyCard, numBuysDecreases)
{
	int expected = G->numBuys - 1;
	buyCard(copper, G);
	int actual = G->numBuys;

	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(buyCard, coinsDecrease)
{
	int expected = 0;

	G->coins = 4;
	buyCard(smithy, G);

	int actual = G->coins;

	TEST_ASSERT_EQUAL(expected, actual);
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(buyCard);
static void RunAllTests(void)
{
	RUN_TEST_CASE(buyCard, notInGame);
	RUN_TEST_CASE(buyCard, tooExpensive);
	RUN_TEST_CASE(buyCard, noBuysLeft);
	RUN_TEST_CASE(buyCard, noCardsLeft);
	RUN_TEST_CASE(buyCard, supplyCountDecreases);
	RUN_TEST_CASE(buyCard, cardEnteredDiscard);
	RUN_TEST_CASE(buyCard, victoryCardIncreaseScore);
	RUN_TEST_CASE(buyCard, multipleBuys);
	RUN_TEST_CASE(buyCard, numBuysDecreases);
	RUN_TEST_CASE(buyCard, coinsDecrease);
}
int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
