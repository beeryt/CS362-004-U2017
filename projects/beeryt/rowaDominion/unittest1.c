#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(initializeGame);
static struct gameState *G;
static int k_cards[10] = {adventurer, gardens,  embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
static int seed = 42;


// Test Setup and Teardown
TEST_SETUP(initializeGame) {
	G = malloc(sizeof(struct gameState));
}
TEST_TEAR_DOWN(initializeGame) {
	free(G);
}

// Individual Unit Tests

// Tests that initializeGame failes given invalid player counts
TEST(initializeGame, playerCountInvalid)
{
	int badValues[4] = {0, 1, 5, 6};
	for (size_t i = 0; i < 4; i++) {
		int players = badValues[i];
		int ret = initializeGame(players, k_cards, seed, G);
		TEST_ASSERT_EQUAL(-1, ret);
	}
}

// Tests that initializeGame succeeds and properly sets numPlayers
TEST(initializeGame, playerCountValid)
{
	for (size_t i = MIN_PLAYERS; i <= MAX_PLAYERS; i++) {
		int ret = initializeGame(i, k_cards, seed, G);
		TEST_ASSERT_EQUAL(0, ret);
		TEST_ASSERT_EQUAL(i, G->numPlayers);
	}
}

// Tests for correct cards in supply after initializeGame
TEST(initializeGame, supplyCards)
{
	// Run this test for each player count combination
	for (size_t numPlayers = MIN_PLAYERS; numPlayers <= MAX_PLAYERS; numPlayers++) {
		// Holds expected card counts, -1 assumes card not in supply
		int expectedCard[treasure_map + 1];
		memset(expectedCard, -1, sizeof expectedCard);

		// Set known cards to expected results
		int victoryCards = (numPlayers == 2) ? 8 : 12;
		expectedCard[curse] = 10 * (numPlayers - 1);
		expectedCard[copper] = 60 - (7 * numPlayers);
		expectedCard[silver] = 40;
		expectedCard[gold] = 30;
		expectedCard[estate] = expectedCard[duchy] = expectedCard[province] = victoryCards;

		// For the specified kindom cards there are 10 each (except gardens which is a victory card)
		for (size_t i = 0; i < 10; i++) {
			expectedCard[k_cards[i]] = (gardens != k_cards[i]) ? 10 : victoryCards;
		}

		// Initialize the game
		int ret = initializeGame(numPlayers, k_cards, seed, G);
		TEST_ASSERT_EQUAL(0, ret);

		// Run the test
		int actualCard[treasure_map + 1];
		for (size_t i = 0; i < treasure_map + 1; i++) {
			actualCard[i] = G->supplyCount[i];
			// Prepare message to specify which card failed
			char msg[4096];
			sprintf(msg, "%ld card", i);
			TEST_ASSERT_EQUAL_MESSAGE(expectedCard[i], actualCard[i], msg);
		}
	}
}

// Tests for correct cards in players' decks after initializeGame
TEST(initializeGame, playerCards)
{
	// Run this test for each player count combination
	for (size_t i = MIN_PLAYERS; i <= MAX_PLAYERS; i++) {
		// Expected values for each player
		int expectedCopper = 7;
		int expectedEstate = 3;
		int expectedHand = 10;
		int expectedDiscard = 0;

		// Initialize the game
		int ret = initializeGame(i, k_cards, seed, G);
		TEST_ASSERT_EQUAL(0, ret);

		// Run this test for each player
		for (size_t p = 0; p < i; p++) {
			// Retrieve the actual counts of each card
			int actualCopper = fullDeckCount(p, copper, G);
			int actualEstate = fullDeckCount(p, estate, G);
			int actualHand = G->handCount[p] + G->deckCount[p];
			int actualDiscard = G->discardCount[p];

			// Assert the conditions
			TEST_ASSERT_EQUAL(expectedCopper, actualCopper);
			TEST_ASSERT_EQUAL(expectedEstate, actualEstate);
			TEST_ASSERT_EQUAL(expectedHand, actualHand);
			TEST_ASSERT_EQUAL(expectedDiscard, actualDiscard);
		}

	}
}

// Tests that initializeGame fails if a kingdom card is reapeated
TEST(initializeGame, repeatKingdomCard)
{
	int badKingdom[10];
	memcpy(badKingdom, k_cards, 10);
	badKingdom[7] = adventurer;
	badKingdom[1] = adventurer;

	int ret = initializeGame(MAX_PLAYERS, badKingdom, seed, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

// Test that initializeGame fails if a non-kingdom card is provided
TEST(initializeGame, invalidKingdomCard)
{
	int badKingdom[10];
	memcpy(badKingdom, k_cards, 10);
	badKingdom[3] = gold;

	int ret = initializeGame(MAX_PLAYERS, badKingdom, seed, G);
	TEST_ASSERT_EQUAL(-1, ret);
}

// Tests that subsequent calls to initializeGame with the same seed match
TEST(initializeGame, sameSeed)
{
	struct gameState G1, G2;
	memset(&G1, 0, sizeof(struct gameState));
	memset(&G2, 0, sizeof(struct gameState));

	initializeGame(MAX_PLAYERS, k_cards, seed, &G1);
	initializeGame(MAX_PLAYERS, k_cards, seed, &G2);

	TEST_ASSERT_EQUAL_MEMORY(&G1, &G2, sizeof(struct gameState));
}

// Tests that subsequent calls to initializeGame with different seeds differ
TEST(initializeGame, differentSeed)
{
	struct gameState G1, G2;
	memset(&G1, 0, sizeof(struct gameState));
	memset(&G2, 0, sizeof(struct gameState));

	initializeGame(MIN_PLAYERS, k_cards, 1, &G1);
	initializeGame(MAX_PLAYERS, k_cards, 2, &G2);

	TEST_ASSERT_FALSE(!memcmp(&G1,&G2,sizeof(struct gameState)));

}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(initializeGame);
static void RunAllTests(void)
{
	RUN_TEST_CASE(initializeGame, playerCountInvalid);
	RUN_TEST_CASE(initializeGame, playerCountValid);
	RUN_TEST_CASE(initializeGame, supplyCards);
	RUN_TEST_CASE(initializeGame, playerCards);
	RUN_TEST_CASE(initializeGame, repeatKingdomCard);
	RUN_TEST_CASE(initializeGame, invalidKingdomCard);
	RUN_TEST_CASE(initializeGame, sameSeed);
	RUN_TEST_CASE(initializeGame, differentSeed);
}
int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
