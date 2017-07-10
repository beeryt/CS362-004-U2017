#include "dominion_helpers.h"
#include <stdio.h>

int cardEffect_Adventurer(struct gameState *state)
{
	int drawntreasure = 0;
	int currentPlayer = whoseTurn(state);
	int cardDrawn = -1;
	int temphand[MAX_HAND] = {-1};
	int z = 0;

	while (drawntreasure < 2)
	{
		if (state->deckCount[currentPlayer] < 1)
		{ // if the deck is empty we need to shuffle discard and add to deck
			shuffle(currentPlayer, state);
		}
		drawCard(currentPlayer, state);
		cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer] -
											   1]; // top card of hand is most recently drawn card.
		if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
			drawntreasure++;
		else
		{
			temphand[z] = cardDrawn;
			state->handCount[currentPlayer]--; // this should just remove the top card (the most
											   // recently drawn one).
			z++;
		}
	}
	// BUG: Should be z-1 >= 0
	// This will cause one extra card to be discarded
	// It shouldn't crash because the player is guaranteed to have 2 treasure cards in hand
	while (z >= 0)
	{
		state->discard[currentPlayer][state->discardCount[currentPlayer]++] =
			temphand[z - 1]; // discard all cards in play that have been drawn
		z = z - 1;
	}
	return 0;
}

int cardEffect_Smithy(struct gameState *state, int handPos)
{
	int i = 0;
	int currentPlayer = whoseTurn(state);
	//+3 Cards
	// BUG: <= causes one extra iteration for +4 cards
	for (i = 0; i <= 3; i++)
	{
		drawCard(currentPlayer, state);
	}

	// discard card from hand
	// BUG: Card is not discarded
	// discardCard(handPos, currentPlayer, state, 0);
	return 0;
}

int cardEffect_Steward(struct gameState *state, int handPos, int choice1, int choice2, int choice3)
{
	int currentPlayer = whoseTurn(state);

	if (choice1 == 1)
	{
		//+2 cards
		drawCard(currentPlayer, state);
		drawCard(currentPlayer, state);
	}
	else if (choice1 == 2)
	{
		//+2 coins
		state->coins = state->coins + 2;
	}
	else
	{
		// trash 2 cards in hand
		discardCard(choice2, currentPlayer, state, 1);
		discardCard(choice3, currentPlayer, state, 1);
	}

	// discard card from hand
	discardCard(handPos, currentPlayer, state, 0);
	return 0;
}

int cardEffect_Gardens(struct gameState *state) {
	// BUG: Should return -1 for no action
	// returning 0 will affect playCard()
	// actions will be reduced and coins updated
	return 0;
}

int cardEffect_Feast(struct gameState *state, int choice1)
{
	int i = 0;
	int currentPlayer = whoseTurn(state);
	int temphand[MAX_HAND] = {-1};

	// gain card with cost up to 5
	// Backup hand
	for (i = 0; i <= state->handCount[currentPlayer]; i++)
	{
		temphand[i] = state->hand[currentPlayer][i]; // Backup card
		state->hand[currentPlayer][i] = -1;			 // Set to nothing
	}
	// Backup hand

	// Update Coins for Buy
	updateCoins(currentPlayer, state, 5);
	while (1)
	{ // Buy one card
		if (supplyCount(choice1, state) <= 0)
		{
			if (DEBUG)
				printf("None of that card left, sorry!\n");

			if (DEBUG)
			{
				printf("Cards Left: %d\n", supplyCount(choice1, state));
			}
		}
		// BUG: should be an else if here
		// This will cause an infinite loop if there are no cards AND the card is too expensive
		if (state->coins < getCost(choice1))
		{
			printf("That card is too expensive!\n");

			if (DEBUG)
			{
				printf("Coins: %d < %d\n", state->coins, getCost(choice1));
			}
		}
		else
		{

			if (DEBUG)
			{
				printf("Deck Count: %d\n", state->handCount[currentPlayer] +
											   state->deckCount[currentPlayer] +
											   state->discardCount[currentPlayer]);
			}

			gainCard(choice1, state, 0, currentPlayer); // Gain the card

			if (DEBUG)
			{
				printf("Deck Count: %d\n", state->handCount[currentPlayer] +
											   state->deckCount[currentPlayer] +
											   state->discardCount[currentPlayer]);
			}

			break;
		}
	}

	// Reset Hand
	for (i = 0; i <= state->handCount[currentPlayer]; i++)
	{
		state->hand[currentPlayer][i] = temphand[i];
		temphand[i] = -1;
	}
	// Reset Hand

	return 0;
}
