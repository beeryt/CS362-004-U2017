#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(cardEffectGardens);
static struct gameState *G;
static int k_cards[10] = {adventurer, smithy, steward, gardens, feast, mine, cutpurse, sea_hag, tribute, treasure_map};
static int seed = 42;
static int p;
static int handPos;
static int card_under_test = gardens;
#define SUPPLY_SIZE sizeof(int)*(treasure_map + 1)
#define HAND_SIZE sizeof(int)*MAX_HAND
#define DECK_SIZE sizeof(int)*MAX_DECK

int findFirstCard(int card, int array[MAX_DECK], int arrayCount)
{
    for (size_t i = 0; i < arrayCount; i++) {
        if (card == array[i]) return i;
    }
    return -1;
}

// Test Setup and Teardown
TEST_SETUP(cardEffectGardens) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
    // add smithy to player's hand
    p = G->whoseTurn;
    gainCard(card_under_test, G, 2/*to hand*/, p); // Add the card under test
    G->numActions = 1;
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
}
TEST_TEAR_DOWN(cardEffectGardens) {
	free(G);
}

// Individual Unit Tests

// Tests that gardens adds 1 to score for every ten cards (rounded down)
TEST(cardEffectGardens, victoryPointsForEveryTen)
{
    // get the initial score (11 cards, 1 garden)
    int initScore = scoreFor(p, G);

    // add 10 copper (21 cards, 1 garden)
    for (size_t i = 0; i < 10; i++)
        gainCard(copper, G, 0/* to discard*/, p); // add 10 copper to discard
    int tenExtra = scoreFor(p, G);

    // add 5 more copper (26 cards, 1 garden)
    for (size_t i = 0; i < 5; i++)
        gainCard(copper, G, 0, p);
    int fifteenExtra = scoreFor(p, G);

    // add 15 more copper (41 cards, 1 garden)
    for (size_t i = 0; i < 15; i++)
        gainCard(copper, G, 0, p);
    int thirtyExtra = scoreFor(p, G);

    // Assert test cases
    TEST_ASSERT_EQUAL(initScore + 1, tenExtra);
    TEST_ASSERT_EQUAL(initScore + 1, fifteenExtra); // this one is rounded down
    TEST_ASSERT_EQUAL(initScore + 3, thirtyExtra);
}

// test that multiple gardens add
TEST(cardEffectGardens, multipleCardsStack)
{
    // Get the initial score
    int initScore = scoreFor(p, G);

    // add 5 extra gardens (16 cards, 6 gardens)
    for (size_t i = 0; i < 5; i++) {
        gainCard(gardens, G, 0, p);
    }
    // 5 extra gardens adds 5 points while under 20 cards
    int expectedScore = initScore + 5;

    // assert test conditions
    TEST_ASSERT_EQUAL(expectedScore, scoreFor(p, G));
}

// test that cards in all fields are counted
TEST(cardEffectGardens, includesDiscardDeckHand)
{
    // add 10 copper to hand, deck, discard
    int initScore = scoreFor(p, G);
    for (size_t i = 0; i < 10; i++) {
        gainCard(copper, G, 0, p); // to discard
        gainCard(copper, G, 1, p); // to deck
        gainCard(copper, G, 2, p); // to hand
    }

    // should be 3 additional victory points
    TEST_ASSERT_EQUAL(initScore + 3, scoreFor(p, G));
}

// Test that the supply is not affected by playing this card
TEST(cardEffectGardens, supplyNotAffected)
{
    // capture the initial Supply Count object
    int initialSupplyCount[treasure_map + 1];
    memcpy(initialSupplyCount, G->supplyCount, SUPPLY_SIZE);

    playCard(handPos, 0, 0, 0, G);

    // capture it again
    int actualSupplyCount[treasure_map + 1];
    memcpy(actualSupplyCount, G->supplyCount, SUPPLY_SIZE);

    // assert the memory matches completely
    TEST_ASSERT_TRUE(!memcmp(initialSupplyCount, actualSupplyCount, SUPPLY_SIZE));
}

// test that other players are not affected
TEST(cardEffectGardens, otherPlayersNotAffected)
{
    // Bunch of variables for initial state
    int hand1[MAX_PLAYERS][MAX_HAND];
    int handCount1[MAX_PLAYERS] = {0};
    int deck1[MAX_PLAYERS][MAX_DECK];
    int deckCount1[MAX_PLAYERS] = {0};
    int discard1[MAX_PLAYERS][MAX_DECK];
    int discardCount1[MAX_PLAYERS] = {0};

    // Bunch of variables for final state
    int hand2[MAX_PLAYERS][MAX_HAND];
    int handCount2[MAX_PLAYERS] = {0};
    int deck2[MAX_PLAYERS][MAX_DECK];
    int deckCount2[MAX_PLAYERS] = {0};
    int discard2[MAX_PLAYERS][MAX_DECK];
    int discardCount2[MAX_PLAYERS] = {0};

    // Copy initial conditions for each player
    for (size_t i = 0; i < G->numPlayers; i++) {
        if (p == i) continue; // ignore current player

        // grab the counts and arrays for hand, deck, discard
        handCount1[i] = G->handCount[i];
        memcpy(hand1[i], G->hand[i], HAND_SIZE);
        deckCount1[i] = G->deckCount[i];
        memcpy(deck1[i], G->deck[i], DECK_SIZE);
        discardCount1[i] = G->discardCount[i];
        memcpy(discard1[i], G->discard[i], DECK_SIZE);
    }

    playCard(handPos, 0, 0, 0, G);


    // Copy initial conditions for each player
    for (size_t i = 0; i < G->numPlayers; i++) {
        if (p == i) continue; // ignore current player

        // grab the counts and arrays for hand, deck discard
        handCount2[i] = G->handCount[i];
        memcpy(hand2[i], G->hand[i], HAND_SIZE);
        deckCount2[i] = G->deckCount[i];
        memcpy(deck2[i], G->deck[i], DECK_SIZE);
        discardCount2[i] = G->discardCount[i];
        memcpy(discard2[i], G->discard[i], DECK_SIZE);

        // for each player assert test conditions
        TEST_ASSERT_EQUAL(handCount1[i], handCount2[i]);
        TEST_ASSERT_EQUAL(deckCount1[i], deckCount2[i]);
        TEST_ASSERT_EQUAL(discardCount1[i], discardCount2[i]);
        TEST_ASSERT_TRUE(!memcmp(hand1, hand2, HAND_SIZE));
        TEST_ASSERT_TRUE(!memcmp(deck1, deck2, DECK_SIZE));
        TEST_ASSERT_TRUE(!memcmp(discard1, discard2, DECK_SIZE));
    }
}

// tests that numbuys are not affected
TEST(cardEffectGardens, buysNotAffected)
{
    int expectedBuys = G->numBuys;

    playCard(handPos, 0, 0, 0, G);

    TEST_ASSERT_EQUAL(expectedBuys, G->numBuys);
}

// tests that action isn't decremented (this is not an action card)
TEST(cardEffectGardens, actionsNotDecremented)
{
    int expectedActions = G->numActions;

    playCard(handPos, 0, 0, 0, G);

    TEST_ASSERT_EQUAL(expectedActions, G->numActions);
}

// tests that player turn remains the same
TEST(cardEffectGardens, playerTurnContinues)
{
    int expectedPlayer = G->whoseTurn;

    playCard(handPos, 0, 0, 0, G);

    TEST_ASSERT_EQUAL(expectedPlayer, G->whoseTurn);
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(cardEffectGardens);
static void RunAllTests(void)
{
    RUN_TEST_CASE(cardEffectGardens, victoryPointsForEveryTen);
    RUN_TEST_CASE(cardEffectGardens, multipleCardsStack);
    RUN_TEST_CASE(cardEffectGardens, includesDiscardDeckHand);
	RUN_TEST_CASE(cardEffectGardens, supplyNotAffected);
	RUN_TEST_CASE(cardEffectGardens, otherPlayersNotAffected);
	RUN_TEST_CASE(cardEffectGardens, buysNotAffected);
	RUN_TEST_CASE(cardEffectGardens, actionsNotDecremented);
	RUN_TEST_CASE(cardEffectGardens, playerTurnContinues);
}

int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
