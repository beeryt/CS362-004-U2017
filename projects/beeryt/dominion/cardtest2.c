#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(cardEffectSmithy);
static struct gameState *G;
static int k_cards[10] = {adventurer, smithy,  steward, gardens, feast, mine, cutpurse, sea_hag, tribute, treasure_map};
static int seed = 42;
static int p;
static int handPos;
static int card_under_test = smithy;
#define SUPPLY_SIZE sizeof(int)*(treasure_map + 1)
#define HAND_SIZE sizeof(int)*MAX_HAND
#define DECK_SIZE sizeof(int)*MAX_DECK

// Test Setup and Teardown
TEST_SETUP(cardEffectSmithy) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
    // add smithy to player's hand
    p = G->whoseTurn;
    gainCard(card_under_test, G, 2/*to hand*/, p); // Add the card under test
    for (handPos = 0; handPos < G->handCount[p]; handPos++) {
        if (G->hand[p][handPos] == card_under_test) break;
    }
}
TEST_TEAR_DOWN(cardEffectSmithy) {
	free(G);
}

// Individual Unit Tests

// Tests that cardEffectSmithy failes given invalid player counts
TEST(cardEffectSmithy, deckHasThreeFewerCards)
{
    int initialDeckCount = G->deckCount[p];
    int expectedDeckCount = initialDeckCount - 3;

    playCard(handPos, 0, 0, 0, G);

    int actualDeckCount = G->deckCount[p];

    TEST_ASSERT_EQUAL(expectedDeckCount, actualDeckCount);
}

// Tests that cardEffectSmithy succeeds and properly sets numPlayers
TEST(cardEffectSmithy, handIncreasedByThree)
{
    int initialCount = G->handCount[p];
    int expectedCount = initialCount + 3 - 1; // minus 1 for the played card

    playCard(handPos, 0, 0, 0, G);

    int actualCount = G->handCount[p];

    TEST_ASSERT_EQUAL(expectedCount, actualCount);
}

// Tests for correct cards in players' decks after cardEffectSmithy
TEST(cardEffectSmithy, supplyNotAffected)
{
    int initialSupplyCount[treasure_map + 1];
    memcpy(initialSupplyCount, G->supplyCount, SUPPLY_SIZE);

    playCard(handPos, 0, 0, 0, G);

    int actualSupplyCount[treasure_map + 1];
    memcpy(actualSupplyCount, G->supplyCount, SUPPLY_SIZE);

    TEST_ASSERT_TRUE(!memcmp(initialSupplyCount, actualSupplyCount, SUPPLY_SIZE));
}

TEST(cardEffectSmithy, otherPlayersNotAffected)
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

// Tests that cardEffectSmithy fails if a kingdom card is reapeated
TEST(cardEffectSmithy, buysNotAffected)
{
    int expectedBuys = G->numBuys;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedBuys, G->numBuys);
}

// Test that cardEffectSmithy fails if a non-kingdom card is provided
TEST(cardEffectSmithy, numActionsDecremented)
{
    int expectedActions = G->numActions - 1;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedActions, G->numActions);
}

// Tests that subsequent calls to cardEffectSmithy with the same seed match
TEST(cardEffectSmithy, playerTurnContinues)
{
    int expectedPlayer = G->whoseTurn;
    playCard(handPos, 0, 0, 0, G);
    TEST_ASSERT_EQUAL(expectedPlayer, G->whoseTurn);
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(cardEffectSmithy);
static void RunAllTests(void)
{
	RUN_TEST_CASE(cardEffectSmithy, deckHasThreeFewerCards);
	RUN_TEST_CASE(cardEffectSmithy, handIncreasedByThree);
	RUN_TEST_CASE(cardEffectSmithy, supplyNotAffected);
	RUN_TEST_CASE(cardEffectSmithy, otherPlayersNotAffected);
	RUN_TEST_CASE(cardEffectSmithy, buysNotAffected);
	RUN_TEST_CASE(cardEffectSmithy, numActionsDecremented);
	RUN_TEST_CASE(cardEffectSmithy, playerTurnContinues);
}

int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
