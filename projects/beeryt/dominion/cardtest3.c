#include <unity.h>
#include <unity_fixture.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

TEST_GROUP(cardEffectSteward);
static struct gameState *G;
static int k_cards[10] = {adventurer, smithy, steward, gardens, feast, mine, cutpurse, sea_hag, tribute, treasure_map};
static int seed = 42;
static int p;
static int handPos;
static int card_under_test = steward;
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
TEST_SETUP(cardEffectSteward) {
	G = malloc(sizeof(struct gameState));
	initializeGame(MAX_PLAYERS, k_cards, seed, G);
    // add smithy to player's hand
    p = G->whoseTurn;
    gainCard(card_under_test, G, 2/*to hand*/, p); // Add the card under test
    gainCard(card_under_test, G, 2/*to hand*/, p); // Add the card under test
    gainCard(card_under_test, G, 2/*to hand*/, p); // Add the card under test
    G->numActions = 3;
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
}
TEST_TEAR_DOWN(cardEffectSteward) {
	free(G);
}

// Individual Unit Tests

// TEsts that choosing +2 cards removes cards from deck
TEST(cardEffectSteward, choicePlusTwoCardsDeck)
{
    int initialDeckCount = G->deckCount[p];
    int expectedDeckCount = initialDeckCount - 2;

    int c1 = 1; // +2 cards
    playCard(handPos, c1, 0, 0, G);

    int actualDeckCount = G->deckCount[p];

    TEST_ASSERT_EQUAL(expectedDeckCount, actualDeckCount);
}

// tests that choosing +2 cards adds cards to hand
TEST(cardEffectSteward, choicePlusTwoCardsHand)
{
    int initialCount = G->handCount[p];
    int expectedCount = initialCount + 2 - 1; // minus 1 for the played card

    int c1 = 1; // +2 cards
    playCard(handPos, c1, 0, 0, G);

    int actualCount = G->handCount[p];

    TEST_ASSERT_EQUAL(expectedCount, actualCount);
}

// tests that choosing +2 coins adds 2 coins to state->coins
TEST(cardEffectSteward, choicePlusTwoCoins)
{
    int initialCoins = G->coins;

    int c1 = 2; // +2 coins;
    playCard(handPos, c1, 0, 0, G);

    int coinIncrease = G->coins - initialCoins;

    TEST_ASSERT_EQUAL(2, coinIncrease);
}

// tests that choosing to trash only affects current player hand, not discard or deck
TEST(cardEffectSteward, choiceTrashHandOnlyHandAffected)
{
    int handCount = G->handCount[p];

    // copy memory of hand, deck, discard
    int hand[MAX_HAND];
    memcpy(hand, G->hand[p], HAND_SIZE);
    int deck[MAX_DECK];
    memcpy(deck, G->deck[p], DECK_SIZE);
    int discard[MAX_DECK];
    memcpy(discard, G->discard[p], DECK_SIZE);


    int c1 = 3; // trash 2 cards in hand
    int c2 = 0; // 1st card in hand
    int c3 = 1; // 2nd card in hand
    playCard(handPos, c1, c2, c3, G);

    // assert test conditions
    TEST_ASSERT_EQUAL(handCount - 3, G->handCount[p]); // -1 for played card
    TEST_ASSERT_FALSE(!memcmp(hand, G->hand[p], HAND_SIZE));
    TEST_ASSERT_TRUE(!memcmp(deck, G->deck[p], DECK_SIZE));
    TEST_ASSERT_TRUE(!memcmp(discard, G->discard[p], DECK_SIZE));
}

// tests that the supply was not affected by playing this card
TEST(cardEffectSteward, supplyNotAffected)
{
    // capture initial state of supplyCount
    int initialSupplyCount[treasure_map + 1];
    memcpy(initialSupplyCount, G->supplyCount, SUPPLY_SIZE);

    // Play the card in all three modes
    playCard(handPos, 1, 0, 0, G); // +2 cards
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 2, 0, 0, G); // +2 coins
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 3, 0, 1, G); // trash 2 cards

    // capture the final state of supplyCount
    int actualSupplyCount[treasure_map + 1];
    memcpy(actualSupplyCount, G->supplyCount, SUPPLY_SIZE);

    TEST_ASSERT_TRUE(!memcmp(initialSupplyCount, actualSupplyCount, SUPPLY_SIZE));
}

// tests that other players hand/deck/discard is not affected
TEST(cardEffectSteward, otherPlayersNotAffected)
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

    playCard(handPos, 1, 0, 0, G); // +2 cards
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 2, 0, 0, G); // +2 coins
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 3, 0, 1, G); // trash 2 cards

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

// tests that numbys is not altered by this card
TEST(cardEffectSteward, buysNotAffected)
{
    int expectedBuys = G->numBuys;

    playCard(handPos, 1, 0, 0, G); // +2 cards
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 2, 0, 0, G); // +2 coins
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 3, 0, 1, G); // trash 2 cards

    TEST_ASSERT_EQUAL(expectedBuys, G->numBuys);
}

// tests that actions are decremented
TEST(cardEffectSteward, numActionsDecremented)
{
    int expectedActions = G->numActions - 3; // playing card 3 times so...

    playCard(handPos, 1, 0, 0, G); // +2 cards
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 2, 0, 0, G); // +2 coins
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 3, 0, 1, G); // trash 2 cards

    TEST_ASSERT_EQUAL(expectedActions, G->numActions);
}

// test that current player does not change
TEST(cardEffectSteward, playerTurnContinues)
{
    int expectedPlayer = G->whoseTurn;

    playCard(handPos, 1, 0, 0, G); // +2 cards
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 2, 0, 0, G); // +2 coins
    handPos = findFirstCard(card_under_test, G->hand[p], G->handCount[p]);
    playCard(handPos, 3, 0, 1, G); // trash 2 cards

    TEST_ASSERT_EQUAL(expectedPlayer, G->whoseTurn);
}

// Setup Tests and Run Them
TEST_GROUP_RUNNER(cardEffectSteward);
static void RunAllTests(void)
{
    RUN_TEST_CASE(cardEffectSteward, choicePlusTwoCardsDeck);
    RUN_TEST_CASE(cardEffectSteward, choicePlusTwoCardsHand);
    RUN_TEST_CASE(cardEffectSteward, choicePlusTwoCoins);
    RUN_TEST_CASE(cardEffectSteward, choiceTrashHandOnlyHandAffected);
	RUN_TEST_CASE(cardEffectSteward, supplyNotAffected);
	RUN_TEST_CASE(cardEffectSteward, otherPlayersNotAffected);
	RUN_TEST_CASE(cardEffectSteward, buysNotAffected);
	RUN_TEST_CASE(cardEffectSteward, numActionsDecremented);
	RUN_TEST_CASE(cardEffectSteward, playerTurnContinues);
}

int main(int argc, char const *argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
