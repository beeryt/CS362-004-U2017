#include "randomtest_helper.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"

#include "rngs.h"
// struct gameState
// {
// 	int numPlayers; // number of players
// 	int supplyCount[treasure_map + 1]; // this is the amount of a specific type of card given a specific number.
// 	int embargoTokens[treasure_map + 1];
// 	int outpostPlayed;
// 	int outpostTurn;
// 	int whoseTurn;
// 	int phase;
// 	int numActions; /* Starts at 1 each turn */
// 	int coins;		/* Use as you see fit! */
// 	int numBuys;	/* Starts at 1 each turn */
// 	int hand[MAX_PLAYERS][MAX_HAND];
// 	int handCount[MAX_PLAYERS];
// 	int deck[MAX_PLAYERS][MAX_DECK];
// 	int deckCount[MAX_PLAYERS];
// 	int discard[MAX_PLAYERS][MAX_DECK];
// 	int discardCount[MAX_PLAYERS];
// 	int playedCards[MAX_DECK];
// 	int playedCardCount;
// };

int randInclusive(int min, int max)
{
    return (int)(Random() * (max - min + 1)) + min;
}

void distributeCards(struct gameState* state, int card, int totalCount)
{
    int supplyCount = randInclusive(1, totalCount);
    state->supplyCount[card] = supplyCount;
    // Randomly place the rest of the cards in players' decks
    for (size_t j = 0; j < totalCount - supplyCount; j++) {
        int player = randInclusive(0, state->numPlayers - 1);
        state->deck[player][state->deckCount[player]++] = card;
    }
}

int randomSupplyCard(struct gameState* state)
{
    // int timeout = 10000;
    // while (timeout--)
    // {
        int c = randInclusive(curse, treasure_map);
        // if (state->supplyCount[c] > 0)
        //     if (state->coins >= getCost(c))
                return c;
    // }
}

void randomGameFull()
{
    struct gameState *state = malloc(sizeof(struct gameState));
    randomizeGameState(state, 0);
    // Start at the beginning of a turn
    endTurn(state);
    int turn = 0;
    while (!isGameOver(state))
    {
        int timeout = 100;
        while (state->numActions && timeout-- && numHandCards(state))
        {
            printf("%d\n", timeout);
            int pos = randInclusive(0, numHandCards(state));
            int c1 = randomSupplyCard(state);
            int c2 = randomSupplyCard(state);
            int c3 = randomSupplyCard(state);
            playCard(pos, c1, c2, c3, state);
        }

        timeout = 100;
        while (state->numBuys && timeout--)
        {
            int card = randInclusive(curse, treasure_map);
            buyCard(card, state);
        }
        endTurn(state);
        turn++;
    }


}

void randomizeGameState(struct gameState* state, int kcard)
{
    memset(state, 0, sizeof(struct gameState));

    state->numPlayers = randInclusive(2, MAX_PLAYERS);
    state->whoseTurn = randInclusive(0, state->numPlayers);
    state->phase = randInclusive(0, 1); // action or buy
    state->numActions = randInclusive(0, 1); // assumes no cards boost this
    state->numBuys = (state->phase == 0) ? 1 : randInclusive(0, 1);
    state->coins = 0;

    for (size_t i = 0; i < state->numPlayers; i++) {
        state->deckCount[i] = 0;
        state->handCount[i] = 0;
        state->discardCount[i] = 0;
    }

    int kcards[10];
    int j = 0;
    int k = 0;

    for (int i = 0; i < 10; i++)
    {
        int card = randInclusive(adventurer, treasure_map);
        for (j = 0; j < k; j++)
            if (card == kcards[j]) break;

        if (j == k) kcards[k++] = card;
        else --i;
    }
    if (kcard) kcards[0] = kcard; // replace one with the kcard under test

    // Fill each player with starting deck
    for (int i = 0; i < state->numPlayers; i++)
    {
        for (int j = 0; j < 7; j++)
            state->deck[i][state->deckCount[i]++] = copper;
        for (int j = 0; j < 3; j++)
            state->deck[i][state->deckCount[i]++] = estate;
    }

    // initialize all supply to non-existent
    for (size_t i = 0; i < treasure_map + 1; i++) {
        state->supplyCount[i] = -1;
    }

    // Distribute the rest of the playable carsd among players and supply
    int numVictory = (state->numPlayers == 2) ? 8 : 12;
    distributeCards(state, curse, 10*(state->numPlayers - 1));
    distributeCards(state, estate, numVictory);
    distributeCards(state, duchy, numVictory);
    distributeCards(state, province, numVictory);
    distributeCards(state, copper, 60 - 7*state->numPlayers);
    distributeCards(state, silver, 40);
    distributeCards(state, gold, 30);
    for (int i = 0; i < 10; i++)
    {
        if (kcards[i] == great_hall || kcards[i] == gardens)
            distributeCards(state, kcards[i], numVictory);
        else distributeCards(state, kcards[i], 10);
    }

    // Select up to 2 piles to be depleted (still playable game state)
    int depleted = randInclusive(0, 2);
    for (size_t i = 0; i < depleted;) {
        int index = randInclusive(curse, treasure_map);
        if (index == province) continue; // game ender!!!
        if (-1 == state->supplyCount[index]) continue;
        if (0 == state->supplyCount[index]) continue;
        state->supplyCount[index] = 0;
        i++;
    }

    // Randomize where each players' cards are
    for (size_t i = 0; i < state->numPlayers; i++) {
        int handCount = 5;
        if (i == state->whoseTurn) handCount = randInclusive(0,5);

        // Shuffle the deck
        shuffle(i, state);
        // Draw to hand
        for (size_t j = 0; j < handCount; j++) {
            int draw = state->deck[i][state->deckCount[i]--];
            state->hand[i][state->handCount[i]++] = draw;
        }
        // Randomize discard
        int discard = randInclusive(0, state->deckCount[i]);
        for (size_t j = 0; j < discard; j++) {
            int draw = state->deck[i][state->deckCount[i]--];
            state->discard[i][state->discardCount[i]++] = draw;
        }
    }

    state->playedCardCount = 0; // unused
    state->outpostPlayed = 0; // unused
    state->outpostTurn = 0; // unused
}
