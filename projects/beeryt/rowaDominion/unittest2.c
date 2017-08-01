#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(shuffle);
static struct gameState *G;
static int k_cards[10] = {adventurer, gardens,  embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
static int seed = 42;


// Test Setup and Teardown
TEST_SETUP(shuffle) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
}
TEST_TEAR_DOWN(shuffle) {
	free(G);
}

// Individual Unit Tests

TEST(shuffle, shuffleChangesDeck)
{
	int p = G->whoseTurn;
	int expectedDeck[MAX_DECK];
	int expectedDeckCount = G->deckCount[p];
	memcpy(expectedDeck, G->deck[p], expectedDeckCount);

	shuffle(p, G);

	int actualDeck[MAX_DECK];
	int actualDeckCount = G->deckCount[p];
	memcpy(actualDeck, G->deck[p], actualDeckCount);

	TEST_ASSERT_EQUAL(expectedDeckCount, actualDeckCount);
	TEST_ASSERT_FALSE(!memcmp(expectedDeck, actualDeck, actualDeckCount));
}

// Tests that shuffle succeeds when there are no provinces
TEST(shuffle, differentSeeds)
{
	struct gameState G1, G2;
	memset(&G1, 0, sizeof(struct gameState));
	memset(&G2, 0, sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, 1, &G1);
	initializeGame(MAX_PLAYERS, k_cards, 2, &G2);

	int p = G1.whoseTurn;
	int deckCount = G1.deckCount[p];
	int initialDeck[MAX_DECK];
	int shuffleDeck1[MAX_DECK];
	int shuffleDeck2[MAX_DECK];

	// Start with same decks in each gameState
	memcpy(G2.deck[p], G1.deck[p], G1.deckCount[p]);
	TEST_ASSERT_TRUE(!memcmp(G2.deck[p], G1.deck[p], deckCount));
	memcpy(initialDeck, G1.deck[p], deckCount);


	// Shuffle the decks and store their memory
	shuffle(p, &G1);
	memcpy(shuffleDeck1, G1.deck[p], deckCount);
	shuffle(p, &G2);
	memcpy(shuffleDeck2, G2.deck[p], deckCount);

	// make sure everything is different
	TEST_ASSERT_FALSE(!memcmp(initialDeck, shuffleDeck1, deckCount));
	TEST_ASSERT_FALSE(!memcmp(initialDeck, shuffleDeck1, deckCount));
	TEST_ASSERT_FALSE(!memcmp(shuffleDeck1, shuffleDeck2, deckCount));
}

TEST(shuffle, sameSeeds)
{
	struct gameState G1, G2;
	memset(&G1, 0, sizeof(struct gameState));
	memset(&G2, 0, sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, &G1);
	initializeGame(MAX_PLAYERS, k_cards, seed, &G2);

	int p = G1.whoseTurn;
	int deckCount = G1.deckCount[p];
	int initialDeck[MAX_DECK];
	int shuffleDeck1[MAX_DECK];
	int shuffleDeck2[MAX_DECK];

	// Start with same decks in each gameState
	memcpy(G2.deck[p], G1.deck[p], G1.deckCount[p]);
	TEST_ASSERT_TRUE(!memcmp(G2.deck[p], G1.deck[p], deckCount));
	memcpy(initialDeck, G1.deck[p], deckCount);


	// Shuffle the decks and store their memory
	shuffle(p, &G1);
	memcpy(shuffleDeck1, G1.deck[p], deckCount);
	shuffle(p, &G2);
	memcpy(shuffleDeck2, G2.deck[p], deckCount);

	// Make sure shuffled decks match
	TEST_ASSERT_FALSE(!memcmp(initialDeck, shuffleDeck1, deckCount));
	TEST_ASSERT_FALSE(!memcmp(initialDeck, shuffleDeck1, deckCount));
	TEST_ASSERT_TRUE(!memcmp(shuffleDeck1, shuffleDeck2, deckCount));
}

TEST(shuffle, supplyDownEdgeCase)
{
}

TEST(shuffle, noCoins)
{
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(shuffle);
static void RunAllTests(void)
{
	RUN_TEST_CASE(shuffle, shuffleChangesDeck);
	RUN_TEST_CASE(shuffle, differentSeeds);
	RUN_TEST_CASE(shuffle, sameSeeds);
	RUN_TEST_CASE(shuffle, supplyDownEdgeCase);
	RUN_TEST_CASE(shuffle, noCoins);
}
int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
