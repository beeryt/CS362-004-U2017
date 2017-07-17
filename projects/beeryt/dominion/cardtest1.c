#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(cardEffectAdventurer);
static struct gameState *G;
static int k_cards[10] = {adventurer, gardens,  embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
static int seed = 42;
static int p;
static int handPos;
#define SUPPLY_SIZE sizeof(int)*(treasure_map + 1)
#define HAND_SIZE sizeof(int)*MAX_HAND
#define DECK_SIZE sizeof(int)*MAX_DECK

// Test Setup and Teardown
TEST_SETUP(cardEffectAdventurer) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
    // add adventurer to player's hand
    p = G->whoseTurn;
    gainCard(adventurer, G, 2/*to hand*/, p); // Add the card under test
    // Insert some extra coppers to ensure enough treasures
    gainCard(copper, G, 1/*to deck*/, p); // using coppers for simplicity
    gainCard(copper, G, 1/*to deck*/, p);
    for (handPos = 0; handPos < G->handCount[p]; handPos++) {
        if (G->hand[p][handPos] == adventurer) break;
    }
}
TEST_TEAR_DOWN(cardEffectAdventurer) {
	free(G);
}

// Individual Unit Tests

// Tests that cardEffectAdventurer failes given invalid player counts
TEST(cardEffectAdventurer, deckHasTwoFewerCards)
{
    int initialDeckCount = G->deckCount[p];
    int expectedDeckCount = initialDeckCount - 2;

    playCard(handPos, 0, 0, 0, G);

    int actualDeckCount = G->deckCount[p];

    TEST_ASSERT_EQUAL(expectedDeckCount, actualDeckCount);
}

// Tests that cardEffectAdventurer succeeds and properly sets numPlayers
TEST(cardEffectAdventurer, twoAdditionalTreasures)
{
    int initialTreasureCount = 0;
    for (size_t i = 0; i < G->handCount[p]; i++) {
        if (G->hand[p][i] == copper) initialTreasureCount++;
    }

    int expectedTreasureCount = initialTreasureCount + 2;

    playCard(handPos, 0, 0, 0, G);

    int actualTreasureCount = 0;
    for (size_t i = 0; i < G->handCount[p]; i++) {
        if (G->hand[p][i] == copper) actualTreasureCount++;
    }

    TEST_ASSERT_EQUAL(expectedTreasureCount, actualTreasureCount);
}

// Tests for correct cards in supply after cardEffectAdventurer
TEST(cardEffectAdventurer, handIncreasedByTwo)
{
    int initialCount = G->handCount[p];
    int expectedCount = initialCount + 2;

    playCard(handPos, 0, 0, 0, G);

    int actualCount = G->handCount[p];

    TEST_ASSERT_EQUAL(expectedCount, actualCount);
}

// Tests for correct cards in players' decks after cardEffectAdventurer
TEST(cardEffectAdventurer, supplyNotAffected)
{
    int initialSupplyCount[treasure_map + 1];
    memcpy(initialSupplyCount, G->supplyCount, SUPPLY_SIZE);

    playCard(handPos, 0, 0, 0, G);

    int actualSupplyCount[treasure_map + 1];
    memcpy(actualSupplyCount, G->supplyCount, SUPPLY_SIZE);

    TEST_ASSERT_TRUE(!memcmp(initialSupplyCount, actualSupplyCount, SUPPLY_SIZE));
}

TEST(cardEffectAdventurer, otherPlayersNotAffected)
{
    int hand1[MAX_PLAYERS][MAX_DECK];
    int handCount1[MAX_PLAYERS] = {0};
    int deck1[MAX_PLAYERS][MAX_DECK];
    int deckCount1[MAX_PLAYERS] = {0};
    int discard1[MAX_PLAYERS][MAX_DECK];
    int discardCount1[MAX_PLAYERS] = {0};

    // Copy initial conditions for each player
    for (size_t i = 0; i < G->numPlayers; i++) {
        if (p == i) continue; // ignore current player
        handCount1[i] = G->handCount[i];
        memcpy(hand1[i], G->hand[i], HAND_SIZE);

        deckCount1[i] = G->deckCount[i];
        memcpy(deck1[i], G->deck[i], DECK_SIZE);

        discardCount1[i] = G->discardCount[i];
        memcpy(discard1[i], G->discard[i], DECK_SIZE);
    }

    playCard(handPos, 0, 0, 0, G);

    int hand2[MAX_PLAYERS][MAX_DECK];
    int handCount2[MAX_PLAYERS] = {0};
    int deck2[MAX_PLAYERS][MAX_DECK];
    int deckCount2[MAX_PLAYERS] = {0};
    int discard2[MAX_PLAYERS][MAX_DECK];
    int discardCount2[MAX_PLAYERS] = {0};

    // Copy initial conditions for each player
    for (size_t i = 0; i < G->numPlayers; i++) {
        if (p == i) continue; // ignore current player
        handCount2[i] = G->handCount[i];
        memcpy(hand2[i], G->hand[i], HAND_SIZE);

        deckCount2[i] = G->deckCount[i];
        memcpy(deck2[i], G->deck[i], DECK_SIZE);

        discardCount2[i] = G->discardCount[i];
        memcpy(discard2[i], G->discard[i], DECK_SIZE);

        TEST_ASSERT_EQUAL(handCount1[i], handCount2[i]);
        TEST_ASSERT_EQUAL(deckCount1[i], deckCount2[i]);
        TEST_ASSERT_EQUAL(discardCount1[i], discardCount2[i]);

        TEST_ASSERT_TRUE(!memcmp(hand1, hand2, HAND_SIZE));
        TEST_ASSERT_TRUE(!memcmp(deck1, deck2, DECK_SIZE));
        TEST_ASSERT_TRUE(!memcmp(discard1, discard2, DECK_SIZE));
    }
}

// Tests that cardEffectAdventurer fails if a kingdom card is reapeated
TEST(cardEffectAdventurer, buysNotAffected)
{
    int expectedBuys = G->numBuys;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedBuys, G->numBuys);
}

// Test that cardEffectAdventurer fails if a non-kingdom card is provided
TEST(cardEffectAdventurer, numActionsDecremented)
{
    int expectedActions = G->numActions - 1;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedActions, G->numActions);
}

TEST(cardEffectAdventurer, playerTurnContinues)
{
    int expectedPlayer = G->whoseTurn;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedPlayer, G->whoseTurn);
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(cardEffectAdventurer);
static void RunAllTests(void)
{
	RUN_TEST_CASE(cardEffectAdventurer, deckHasTwoFewerCards);
	RUN_TEST_CASE(cardEffectAdventurer, twoAdditionalTreasures);
	RUN_TEST_CASE(cardEffectAdventurer, handIncreasedByTwo);
	RUN_TEST_CASE(cardEffectAdventurer, supplyNotAffected);
	RUN_TEST_CASE(cardEffectAdventurer, otherPlayersNotAffected);
	RUN_TEST_CASE(cardEffectAdventurer, buysNotAffected);
	RUN_TEST_CASE(cardEffectAdventurer, numActionsDecremented);
	RUN_TEST_CASE(cardEffectAdventurer, playerTurnContinues);
}

int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
